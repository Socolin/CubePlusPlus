#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <SFML/System/Unicode.hpp>
#include <vector>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/socketft.h>

namespace Network
{
#define INITIAL_BUFFER_SIZE 512
#define MAX_TICK_FOR_KEEPALIVE 1200
#define CURRENT_VERSION_PROTOCOL 49

typedef std::pair<char*, unsigned short> buffer_t;
class NetworkPacket;
class NetworkSession
{
public:
	NetworkSession(int socket);
	virtual ~NetworkSession();

	void ReceiveInBuffer() throw (NetworkException);
	void ReceiveData() throw (NetworkException);

	void SendPacket(NetworkPacket& packet);

	void handleBadPacket() throw (NetworkException)
	{
	}

	void handleKeepAlive() throw (NetworkException);
	void handleHandShake() throw (NetworkException);
	void handleEncryptionKeyRequest() throw (NetworkException);
	void handleEncryptionKeyResponse() throw (NetworkException);
	void handleClientStatuses () throw (NetworkException);

	void updateTick(uint32_t tick)
	{
		if (lastKeepAliveTick - tick > MAX_TICK_FOR_KEEPALIVE)
		{
			// Disconnect
		}
	}
private:
	void readData(int length, char* data) throw (NetworkException);
	char readByte() throw (NetworkException);
	short readShort() throw (NetworkException);
	int readInt() throw (NetworkException);
	buffer_t readBuffer() throw (NetworkException);
	std::wstring readString(int maxSize) throw (NetworkException);

	int socket;
	std::vector<char> buffer;
	int lastKeepAliveTick;
	uint32_t startPosInBuffer;
	uint32_t bufferSize;
	uint32_t maxBufferSize;
	bool cryptedMode;
	buffer_t sharedSecretKey;

	// Décodage
	CryptoPP::RSAES<CryptoPP::PKCS1v15>::Decryptor rsaDecryptor;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption* aesDecryptor;
	byte aesDecryptBuffer[CryptoPP::AES::BLOCKSIZE];
	CryptoPP::StreamTransformationFilter* cfbDecryptor;
	std::string sDecryptOutput;

	// Encodage
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption* aesEncryptor;
	CryptoPP::StreamTransformationFilter* cfbEncryptor;
	byte aesEncryptBuffer[CryptoPP::AES::BLOCKSIZE];
};
}

#endif /* NETWORKSESSION_H_ */
