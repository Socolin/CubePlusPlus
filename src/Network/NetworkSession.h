#ifndef NETWORKSESSION_H_
#define NETWORKSESSION_H_

#include "NetworkException.h"
#include <vector>
#include <cryptopp/modes.h>
#include <cryptopp/rsa.h>
#include <cryptopp/aes.h>
#include <cryptopp/socketft.h>

#include "Util/RingBuffer.h"

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
#define MAX_STRING_SIZE 256

enum eSessionState
{
    STATE_NEVER     = 0x0,
    STATE_NOTLOGGED = 0x1,
    STATE_LOGGING   = 0x2,
    STATE_LOGGED    = 0x4,
    STATE_INGAME    = 0x8,
    STATE_KICKED    = 0x10,
    STATE_WAIT_LOGGIN= 0x20,
    STATE_ERROR     = 0x40,
    STATE_DISCONNECT = 0x80,

    STATE_EVERYTIME = STATE_NOTLOGGED | STATE_LOGGING | STATE_LOGGED | STATE_INGAME | STATE_WAIT_LOGGIN,
    STATES_DISCONNECT = STATE_DISCONNECT | STATE_KICKED | STATE_ERROR
};

typedef union
{
    int i;
    float f;
} IntToFloat;

typedef union
{
    long long i;
    double f;
} LongToDouble;

typedef std::pair<char*, unsigned short> buffer_t;
class NetworkPacket;
class NetworkSession
{
public:
    NetworkSession(int socket, const std::string& ip);
    virtual ~NetworkSession();

    void ReceiveInBuffer() throw (NetworkException);
    void ReceiveData() throw (NetworkException);

    void SendPacket(const NetworkPacket& packet);

    void handleBadPacket() throw (NetworkException)
    {
    }

    void CloseForDelete();
    void SendKickMessage(const std::wstring& message);

    void disconnect(std::wstring message);
    void kick(std::wstring message);

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

    inline void SendUpdateTime(long long currentTick, long long ageOfWorld);
    inline void SendSetExperience(short level, short totalXP, float pct);
    inline void SendUpdateHealth(short health, short food, float foodSaturation);
    inline void SendSetPositionAndLook(double x, double y, double stance, double z, float yaw, float pitch, bool onGround);
    inline void SendSetAbilities(char walkingSpeed, char flyingSpeed, char abilityFlag);
    inline void SendChangeGameState(char reason, char gameMode);

    bool UpdateTick();
    inline bool isDisconnected()
    {
        return (state & (STATES_DISCONNECT));
    }

    bool IsSendBufferHalfFull();
    int GetSocket() const;
    void SendData();
    
    int GetProtocolVersion();
    std::wstring GetProtocolVersionWstr();
    std::wstring GetMinecraftServerVersion();

private:
    void readData(int length, char* data) throw (NetworkException);
    char readByte() throw (NetworkException);
    short readShort() throw (NetworkException);
    int readInt() throw (NetworkException);
    long long readLong() throw (NetworkException);
    buffer_t readBuffer() throw (NetworkException);
    std::wstring readString(const int maxSize) throw (NetworkException);
    float readFloat() throw (NetworkException);
    double readDouble() throw (NetworkException);
    Inventory::ItemStack* readSlot()  throw (NetworkException);

private:
    int socket;
    std::string ip;
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
    std::wstring serverId;
    World::EntityPlayer* player;

    Util::RingBuffer<19> sendBuffer;

    size_t lastSendKeepAliveTick;
    int lastKeepAliveId;

    int waitLoginId;
    std::string decryptedSecretKey;
    std::string serverIdStr;
        
    int current_version_protocol;
    std::string current_version_protocol_str;
    std::string minecraft_server_version;
};
}

#include "NetworkSession.hxx"

#endif /* NETWORKSESSION_H_ */
