#include "NetworkSession.h"
#include "NetworkPacket.h"
#include "NetworkEncryption.h"
#include "Opcode.h"

#include <iostream>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>

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

	if (payload == 0)
	{
		NetworkPacket packet;
		std::wstring levelType(L"flat");
		packet << (unsigned char)OP_LOGIN_REQUEST << (int)1 << levelType << (char)1 << (char)0 << (char)0 << (char)0 << (char)20;
		SendPacket(packet);

		NetworkPacket packetSpawn;
		packetSpawn << (unsigned char)OP_SPAWN_POSITION << (int)0 << (int)0 << (int)0;
		SendPacket(packetSpawn);
	}
}
void NetworkSession::handleEncryptionKeyRequest() throw (NetworkException)
{
	short size1 = readShort();
	short size2 = readShort();

	if (size1 != 0 || size2 != 0)
		throw NetworkSession("handleEncryptionKeyRequest");
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
	cfbDecryptor = new CryptoPP::StreamTransformationFilter(*aesDecryptor, new CryptoPP::StringSink(sDecryptOutput));

	memcpy(aesEncryptBuffer,sDecryptedSharedSecret.c_str(),16);
	aesEncryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption((byte*)sDecryptedSharedSecret.c_str(),(unsigned int)16,aesEncryptBuffer,1);
	cfbEncryptor = new CryptoPP::StreamTransformationFilter(*aesEncryptor, new CryptoPP::SocketSink(socket));
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
}
