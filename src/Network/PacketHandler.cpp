#include "NetworkSession.h"
#include "NetworkPacket.h"
#include "NetworkEncryption.h"
#include "Opcode.h"

#include <iostream>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>

#include "World/WorldManager.h"
#include "Entity/EntityPlayer.h"

#define DEBUG_STR(str) std::wcout << L"DEBUG: str: size:"<< str.length() << L" value:\"" << str << L"\"" << std::endl;
#define DEBUG_SHORT(value) std::cout << "short:" << value << std::endl;
#define DEBUG_INT(value) std::cout << "int:" << value << std::endl;
#define DEBUG_CHAR(value) std::cout << "char:" << (int)value << std::endl;


namespace Network
{
void NetworkSession::handleKeepAlive() throw (NetworkException)
{
	int value = readInt();
	if (value != lastKeepAliveId)
	{
	    disconnect();
	}
	lastKeepAliveId = 0;
}
void NetworkSession::handleChatMessage() throw (NetworkException)
{
	std::wstring message = readString(100);
	DEBUG_STR(message)
}
void NetworkSession::handleHandShake() throw (NetworkException)
{
	unsigned char protocolVersion = readByte();
	if (protocolVersion != CURRENT_VERSION_PROTOCOL)
		throw NetworkException("Bad protocol version, use 1.4.5");

	username = readString(16);
	std::wstring serverHost = readString(512);
	readInt();

	DEBUG_STR(username)
	DEBUG_STR(serverHost)

	std::wstring serverId(L"-");

	NetworkEncryption* encrypt = NetworkEncryption::GetInstance();
	const std::pair<char*, short>& certificate = encrypt->GetCertificate();

	int rnd = rand();
	const std::pair<char*, short> token = std::make_pair((char*)&rnd, (short)4);

	NetworkPacket packet(OP_ENCRYPTION_KEY_REQUEST);
	packet << serverId << certificate << token;
	SendPacket(packet);
}
void NetworkSession::handleUseEntity() throw (NetworkException)
{
	readInt();
	readInt();
	readByte();
}
void NetworkSession::handlePlayer() throw (NetworkException)
{
	readByte();
}
void NetworkSession::handlePlayerPosition() throw (NetworkException)
{
	double newX = readDouble();
	double newY = readDouble();
	/*double Stance = */readDouble();
	double newZ = readDouble();
	readByte();
	player->MoveTo(newX, newY, newZ);
}
void NetworkSession::handlePlayerLook() throw (NetworkException)
{
	double newYaw = readFloat();
	double newPitch = readFloat();
	readByte();
	player->Rotate(newYaw, newPitch);
}
void NetworkSession::handlePlayerPositionAndLook() throw (NetworkException)
{
    double newX = readDouble();
    double newY = readDouble();
    /*double Stance = */readDouble();
    double newZ = readDouble();
    double newYaw = readFloat();
    double newPitch = readFloat();
    readByte();
    player->Rotate(newYaw, newPitch);
    player->MoveTo(newX, newY, newZ);
}

void NetworkSession::handlePlayerDigging() throw (NetworkException)
{
	readByte();
	readInt();
	readByte();
	readInt();
	readByte();
}

void NetworkSession::handlePlayerBlockPlacement() throw (NetworkException)
{
	readInt();
	readByte();
	readInt();

	short blockId = readShort();
	if (blockId != -1)
	{
		readByte();
		readShort();
		short nbtDataLength = readShort();
		if (nbtDataLength != -1)
		{
			for (int i = 0; i < nbtDataLength; i++)
				readByte();
		}
	}


	readByte();
	readByte();
	readByte();
}
void NetworkSession::handleHeldItemChange() throw (NetworkException)
{
	readShort();
}
void NetworkSession::handleAnimation() throw (NetworkException)
{
	readInt();
	readByte();
}
void NetworkSession::handleEntityAction() throw (NetworkException)
{
	readInt();
	readByte();
}
void NetworkSession::handleCloseWindow() throw (NetworkException)
{
	readByte();
}
void NetworkSession::handleClickWindow() throw (NetworkException)
{
	readByte();
	readShort();
	readByte();
	readShort();
	readByte();
	short blockId = readShort();
	if (blockId != -1)
	{
		readByte();
		readShort();
		short nbtDataLength = readShort();
		if (nbtDataLength != -1)
		{
			for (int i = 0; i < nbtDataLength; i++)
				readByte();
		}
	}


}
void NetworkSession::handleConfirmTransaction() throw (NetworkException)
{
	readByte();
	readShort();
	readByte();
}
void NetworkSession::handleEnchantItem() throw (NetworkException)
{
	readByte();
	readByte();
}
void NetworkSession::handleUpdateSign() throw (NetworkException)
{
	readInt();
	readByte();
	readInt();
	readString(15);
	readString(15);
	readString(15);
	readString(15);
}
void NetworkSession::handlePlayerAbilities() throw (NetworkException)
{
	readByte();
	readByte();
	readByte();
}
void NetworkSession::handleTabComplete() throw (NetworkException)
{
	readByte();
	readByte();
	readByte();
}
void NetworkSession::handleCreativeInventoryAction() throw (NetworkException)
{
	readShort();
	short blockId = readShort();
	if (blockId != -1)
	{
		readByte();
		readShort();
		short nbtDataLength = readShort();
		if (nbtDataLength != -1)
		{
			for (int i = 0; i < nbtDataLength; i++)
				readByte();
		}
	}
}
void NetworkSession::handleClientSettings() throw (NetworkException)
{
	std::wstring locale = readString(16);
	char viewDistance = readByte();
	char chatFlag = readByte();
	char difficulty = readByte();
	bool showCape = readByte();
	DEBUG_STR(locale)
	DEBUG_CHAR(viewDistance)
	DEBUG_CHAR(chatFlag)
	DEBUG_CHAR(difficulty)
	DEBUG_CHAR(showCape)

}
void NetworkSession::handleClientStatuses() throw (NetworkException)
{
	char payload = readByte();
	DEBUG_CHAR(payload)

	if (payload == 0 && (state & STATE_INGAME) == 0)
	{
		state = STATE_INGAME;
		NetworkPacket packet(OP_LOGIN_REQUEST);
		std::wstring levelType(L"flat");
		packet  << (int)1 << levelType << (char)1 << (char)0 << (char)0 << (char)0 << (char)20;
		SendPacket(packet);

		World::WorldManager* worldManager = World::WorldManager::GetInstance();
		player = worldManager->LoadAndJoinWorld(username, this);
	}
}
void NetworkSession::handlePluginMessage() throw (NetworkException)
{
	readString(128);
	readBuffer();
}
void NetworkSession::handleEncryptionKeyRequest() throw (NetworkException)
{
	short size1 = readShort();
	short size2 = readShort();

	if (size1 != 0 || size2 != 0)
		throw NetworkException("handleEncryptionKeyRequest");
	DEBUG_SHORT(size1)
	DEBUG_SHORT(size2)
}

void NetworkSession::handleEncryptionKeyResponse() throw (NetworkException)
{
	buffer_t sharedSecretKey;
	sharedSecretKey = readBuffer();
	DEBUG_SHORT(sharedSecretKey.second)
	if (sharedSecretKey.second != 128)
		throw NetworkException("sharedSecretKey.second != 128");

	// TODO: check it
	buffer_t verifyToken = readBuffer();
	DEBUG_SHORT(verifyToken.second)
	if (verifyToken.second != 128)
		throw NetworkException("verifyToken.second != 128");

	NetworkEncryption* encrypt = NetworkEncryption::GetInstance();

	CryptoPP::RSAES<CryptoPP::PKCS1v15>::Decryptor rsaDecryptor(*encrypt->getPrivateKey());

	std::string sEncryptedSharedSecret(sharedSecretKey.first,sharedSecretKey.second);
	std::string sDecryptedSharedSecret ("");

	try
	{
		CryptoPP::StringSource StrSrc(
				sEncryptedSharedSecret,
				true,
				new CryptoPP::PK_DecryptorFilter(encrypt->getAutoSeed(), rsaDecryptor, new CryptoPP::StringSink(sDecryptedSharedSecret))
		);

	} catch (CryptoPP::Exception&)
	{
		std::cout << "Houston we have a problem\n";
	}

	if (sDecryptedSharedSecret.length() != 16)
		throw NetworkException("sharedSecretKey.second != 128");

	NetworkPacket packet(OP_ENCRYPTION_KEY_RESPONSE, 5);
	short zero = 0;
	packet << zero << zero;
	SendPacket(packet);

	cryptedMode = true;

	memcpy(aesDecryptBuffer,sDecryptedSharedSecret.c_str(),16);
	aesDecryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16,aesDecryptBuffer,1);

	memcpy(aesEncryptBuffer,sDecryptedSharedSecret.c_str(),16);
	aesEncryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16,aesEncryptBuffer,1);

	state = STATE_LOGGING;
}
void NetworkSession::handlePing() throw (NetworkException)
{
	char magic = readByte();
	DEBUG_CHAR(magic)

	NetworkPacket packet(OP_KICK);
	std::wstring kickReason(L"Test");
	packet << kickReason;
	SendPacket(packet);
}
void NetworkSession::handleDisconnect() throw (NetworkException)
{
	readString(128);
	disconnect();
}

void NetworkSession::UpdateTick()
{
    if (lastKeepAliveId != 0)
    {
        lastKeepAliveTick--;
        if (lastKeepAliveTick <= 0)
        {
            disconnect();
        }
    }
    else
    {
        lastSendKeepAliveTick--;
        if (lastSendKeepAliveTick > INTERVAL_SEND_KEEPALIVE)
        {
            lastKeepAliveTick = MAX_TICK_FOR_KEEPALIVE;
            lastSendKeepAliveTick = INTERVAL_SEND_KEEPALIVE;
            lastKeepAliveId = rand();
            lastKeepAliveId |= 0x1; // always != 0
            NetworkPacket keepAlivePacket(OP_KEEPALIVE);
            keepAlivePacket << lastKeepAliveId;
            SendPacket(keepAlivePacket);
        }
    }
}
}
