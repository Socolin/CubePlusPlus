#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <SFML/System/Unicode.hpp>
#include <vector>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/socketft.h>

namespace World
{
class EntityPlayer;
}

namespace Network
{
#define INITIAL_BUFFER_SIZE 512
#define MAX_TICK_FOR_KEEPALIVE 1200
#define CURRENT_VERSION_PROTOCOL 49

enum eSessionState // TODO: use flag
{
	STATE_NEVER,
	STATE_EVERYTIME, //TODO: remove this
	STATE_NOTLOGGED,
	STATE_LOGGING,
	STATE_LOGGED,
	STATE_INGAME
};

typedef union
{
	int i;
	float f;
} IntToFloat;

typedef union
{
	long i;
	double f;
} LongToDouble;

typedef std::pair<char*, unsigned short> buffer_t;
class NetworkPacket;
class NetworkSession
{
public:
	NetworkSession(int socket);
	virtual ~NetworkSession();

	void ReceiveInBuffer() throw (NetworkException);
	void ReceiveData() throw (NetworkException);

	void SendPacket(const NetworkPacket& packet) const;

	void handleBadPacket() throw (NetworkException)
	{
	}

	void disconnect();

	void handleKeepAlive() throw (NetworkException);
	void handleHandShake() throw (NetworkException);
	void handleChatMessage() throw (NetworkException);
	void handleUseEntity() throw (NetworkException);
	void handlePlayer() throw (NetworkException);
	void handlePlayerPosition() throw (NetworkException);
	void handlePlayerLook() throw (NetworkException);
	void handlePlayerPositionAndLook() throw (NetworkException);
	void handlePlayerDigging() throw (NetworkException);
	void handlePlayerBlockPlacement() throw (NetworkException);
	void handleHeldItemChange() throw (NetworkException);
	void handleAnimation() throw (NetworkException);
	void handleEntityAction() throw (NetworkException);
	void handleCloseWindow() throw (NetworkException);
	void handleClickWindow() throw (NetworkException);
	void handleConfirmTransaction() throw (NetworkException);
	void handleCreativeInventoryAction() throw (NetworkException);
	void handleEnchantItem() throw (NetworkException);
	void handleUpdateSign() throw (NetworkException);
	void handlePlayerAbilities() throw (NetworkException);
	void handleTabComplete() throw (NetworkException);
	void handleClientSettings () throw (NetworkException);
	void handleClientStatuses () throw (NetworkException);
	void handlePluginMessage () throw (NetworkException);
	void handleEncryptionKeyRequest() throw (NetworkException);
	void handleEncryptionKeyResponse() throw (NetworkException);
	void handleDisconnect() throw (NetworkException);
	void handlePing () throw (NetworkException);

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
	long readLong() throw (NetworkException);
	buffer_t readBuffer() throw (NetworkException);
	std::wstring readString(int maxSize) throw (NetworkException);
	float readFloat() throw (NetworkException);
	double readDouble() throw (NetworkException);

	int socket;
	std::vector<char> buffer;
	eSessionState state;
	int lastKeepAliveTick;
	uint32_t startPosInBuffer;
	uint32_t bufferSize;
	uint32_t maxBufferSize;
	bool cryptedMode;

	// Décodage
	CryptoPP::RSAES<CryptoPP::PKCS1v15>::Decryptor rsaDecryptor;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption* aesDecryptor;
	byte aesDecryptBuffer[CryptoPP::AES::BLOCKSIZE];

	// Encodage
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption* aesEncryptor;
	byte aesEncryptBuffer[CryptoPP::AES::BLOCKSIZE];

	std::wstring username;
	World::EntityPlayer* player;
};
}

#endif /* NETWORKSESSION_H_ */
