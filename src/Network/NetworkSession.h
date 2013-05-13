#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <vector>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/socketft.h>

namespace World
{
class EntityPlayer;
}
namespace Inventory
{
class ItemStack;
}

namespace Network
{
#define INITIAL_BUFFER_SIZE 512
#define MAX_TICK_FOR_KEEPALIVE 1200
#define INTERVAL_SEND_KEEPALIVE 20
#define CURRENT_VERSION_PROTOCOL 60
#define CURRENT_VERSION_PROTOCOL_WSTR L"60"
#define MAX_STRING_SIZE 256

enum eSessionState
{
    STATE_NEVER     = 0x0,
    STATE_DISCONECT = 0x0,
    STATE_NOTLOGGED = 0x1,
    STATE_LOGGING   = 0x2,
    STATE_LOGGED    = 0x4,
    STATE_INGAME    = 0x8,

    STATE_EVERYTIME = STATE_NOTLOGGED | STATE_LOGGING | STATE_LOGGED | STATE_INGAME
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

    void SendPacket(const NetworkPacket& packet);

    void handleBadPacket() throw (NetworkException)
    {
    }

    void disconnect(const char* reason = "unk");

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

    inline void SendUpdateTime(long currentTick, long ageOfWorld);
    inline void SendSetExperience(short level, short totalXP, float pct);
    inline void SendUpdateHealth(short health, short food, float foodSaturation);
    inline void SendSetPositionAndLook(double x, double y, double stance, double z, float yaw, float pitch, bool onGround);
    inline void SendSetAbilities(char walkingSpeed, char flyingSpeed, char abilityFlag);

    void UpdateTick();
    inline bool isDisconnected()
    {
        return state == STATE_DISCONECT;
    }
private:
    void readData(int length, char* data) throw (NetworkException);
    char readByte() throw (NetworkException);
    short readShort() throw (NetworkException);
    int readInt() throw (NetworkException);
    long readLong() throw (NetworkException);
    buffer_t readBuffer() throw (NetworkException);
    std::wstring readString(const int maxSize) throw (NetworkException);
    float readFloat() throw (NetworkException);
    double readDouble() throw (NetworkException);
    Inventory::ItemStack* readSlot()  throw (NetworkException);

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

    unsigned char sendBuffer[512];
    size_t lastSendKeepAliveTick;
    int lastKeepAliveId;
};
}

#include "NetworkSession.hxx"

#endif /* NETWORKSESSION_H_ */
