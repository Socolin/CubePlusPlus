#include "NetworkSession.h"
#include "NetworkPacket.h"
#include "NetworkEncryption.h"
#include "Opcode.h"

#include <iostream>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>

#define DEBUG_STR(str) std::wcout << L"DEBUG: str: size:"<< str.length() << L" value:\"" << str << L"\"" << std::endl;
#define DEBUG_SHORT(value) std::cout << "short:" << value << std::endl;
#define DEBUG_INT(value) std::cout << "int:" << value << std::endl;
#define DEBUG_CHAR(value) std::cout << "char:" << (int)value << std::endl;


namespace Network
{
void NetworkSession::handleKeepAlive() throw (NetworkException)
{
	int value = readInt();
	DEBUG_INT(value)
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

	std::wstring userName = readString(16);
	std::wstring serverHost = readString(512);
	readInt();

	DEBUG_STR(userName)
	DEBUG_STR(serverHost)

	unsigned char packetId = OP_ENCRYPTION_KEY_REQUEST;

	std::wstring serverId(L"-");

	NetworkEncryption* encrypt = NetworkEncryption::GetInstance();
	const std::pair<char*, short>& certificate = encrypt->GetCertificate();

	int rnd = rand();
	const std::pair<char*, short> token = std::make_pair((char*)&rnd, (short)4);

	NetworkPacket packet;
	packet << packetId << serverId << certificate << token;
	SendPacket(packet);
}
void NetworkSession::handleUseEntity() throw (NetworkException)
{
	readInt();
	readInt();
	readByte();
}
void NetworkSession::handlePlayerPosition() throw (NetworkException)
{
	readDouble();
	readDouble();
	readDouble();
	readDouble();
	readByte();
}
void NetworkSession::handlePlayerLook() throw (NetworkException)
{
	readFloat();
	readFloat();
	readByte();
}
void NetworkSession::handlePlayerPositionAndLook() throw (NetworkException)
{
	readDouble();
	readDouble();
	readDouble();
	readDouble();
	readFloat();
	readFloat();
	readByte();
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

	if (payload == 0 && state != STATE_INGAME)
	{
		state = STATE_INGAME;
		NetworkPacket packet;
		std::wstring levelType(L"flat");
		packet << (unsigned char)OP_LOGIN_REQUEST << (int)1 << levelType << (char)1 << (char)0 << (char)0 << (char)0 << (char)20;
		SendPacket(packet);


		NetworkPacket packetRespawn;
		packetRespawn << (unsigned char)OP_RESPAWN << (int)0 << (char)0 << (char)1 << (short) 256 << levelType;
		SendPacket(packetRespawn);

		NetworkPacket packetInitialPosition;
		packetInitialPosition << (unsigned char)OP_PLAYER_POSITION_AND_LOOK << (double)0 << (double)0  << (double)0 << (double)0 << (float)0 << (float)0 << (char)0;
		SendPacket(packetInitialPosition);

		NetworkPacket packetSpawn;
		packetSpawn << (unsigned char)OP_SPAWN_POSITION << (int)0 << (int)0 << (int)0;
		SendPacket(packetSpawn);


		NetworkPacket packetPlayer;
		packetPlayer << (unsigned char)OP_PLAYER << (char) 1;
		SendPacket(packetPlayer);
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

	NetworkPacket packet(5);
	unsigned char packetId = OP_ENCRYPTION_KEY_RESPONSE;
	short zero = 0;
	packet << packetId << zero << zero;
	SendPacket(packet);

	cryptedMode = true;

	memcpy(aesDecryptBuffer,sDecryptedSharedSecret.c_str(),16);
	aesDecryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16,aesDecryptBuffer,1);

	memcpy(aesEncryptBuffer,sDecryptedSharedSecret.c_str(),16);
	aesEncryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16,aesEncryptBuffer,1);
}
void NetworkSession::handlePing() throw (NetworkException)
{
	char magic = readByte();
	DEBUG_CHAR(magic)

	NetworkPacket packet;
	std::wstring kickReason(L"Test");
	packet << (unsigned char)OP_KICK  << kickReason;
	SendPacket(packet);
}
void NetworkSession::handleDisconnect() throw (NetworkException)
{
	readString(128);
}
}
