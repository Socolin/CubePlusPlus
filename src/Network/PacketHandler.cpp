#include "NetworkSession.h"
#include "NetworkPacket.h"
#include "NetworkEncryption.h"
#include "Opcode.h"
#include "../Config/Config.h"

#include <iostream>
#include <sstream>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>

#include "Block/TileEntities/TileEntitySign.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Entity/EntityPlayer.h"
#include "Window/Window.h"

#define DEBUG_STR(str) std::wcout << L"DEBUG: str: size:"<< str.length() << L" value:\"" << str << L"\"" << std::endl;
#define DEBUG_SHORT(value) std::cout << #value << "(short):" << value << std::endl;
#define DEBUG_INT(value) std::cout << #value << "(int):" << value << std::endl;
#define DEBUG_CHAR(value) std::cout << #value << "(char):" << (int)value << std::endl;


namespace Network
{
void NetworkSession::handleKeepAlive() throw (NetworkException)
{
    int value = readInt();
    if (value == 0)
        return;
    if (value != lastKeepAliveId)
    {
        std::cout << lastKeepAliveId << " " << value << std::endl;
        disconnect("bad keepalive message");
    }
    lastKeepAliveId = 0;
}
void NetworkSession::handleChatMessage() throw (NetworkException)
{
    std::wstring message = readString(100);
    World::WorldManager::Instance().HandleChatMessage(player, message);
}
void NetworkSession::handleHandShake() throw (NetworkException)
{
    unsigned char protocolVersion = readByte();
    if (protocolVersion != CURRENT_VERSION_PROTOCOL)
        throw NetworkException("Bad protocol version, use 1.5");

    username = readString(16);
    std::wstring serverHost = readString(255);
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
    int target = readInt();
    bool leftClick = readByte();
    player->UseEntity(target, leftClick);
}
void NetworkSession::handlePlayer() throw (NetworkException)
{
    readByte();
}
void NetworkSession::handlePlayerPosition() throw (NetworkException)
{
    double newX = readDouble();
    double newY = readDouble();
    /*double Stance = */
    readDouble();
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
    /*double Stance = */
    readDouble();
    double newZ = readDouble();
    double newYaw = readFloat();
    double newPitch = readFloat();
    readByte();
    player->Rotate(newYaw, newPitch);
    player->MoveTo(newX, newY, newZ);
}

void NetworkSession::handlePlayerDigging() throw (NetworkException)
{
    char state = readByte();
    int x = readInt();
    unsigned char y = readByte();
    int z = readInt();
    char face = readByte();
    std::cout << "handlePlayerDigging state:" << (int)state << " x:" << x << " y:"<< (int)y << " z:" << z << " face:" << (int)face << std::endl;
    player->DigBlock(state, x, y, z, face);
}

void NetworkSession::handlePlayerBlockPlacement() throw (NetworkException)
{
    int x = readInt();
    unsigned char y = readByte();
    int z = readInt();
    char face = readByte();

    // Metadata, not used
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


    char cursorPositionX = readByte();
    char cursorPositionY = readByte();
    char cursorPositionZ = readByte();
    std::cout << "handlePlayerBlockPlacement x:" << x << " y:"<< (int)y << " z:" << z << " face:" << (int)face << " blockId:" << blockId
              << " cx:" << (int)cursorPositionX
              << " cy:" << (int)cursorPositionY
              << " cz:" << (int)cursorPositionZ
              << std::endl;

    player->PlaceBlock(x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ);
}
void NetworkSession::handleHeldItemChange() throw (NetworkException)
{
    int slotId = readShort();
    if (slotId > 8 || slotId < 0)
        throw NetworkException("handleHeldItemChange: SlotID > 8 || < 0");
    player->GetHandsInventory()->setHandSlot(slotId);
    player->ItemInHandHasChange();
}
void NetworkSession::handleAnimation() throw (NetworkException)
{
    int entityId = readInt();
    char animationId = readByte();
    std::cout << "handleAnimation: EID:" << entityId << " animationId:" << (int)animationId << std::endl;
    player->PlayAnimation(animationId);
}
void NetworkSession::handleEntityAction() throw (NetworkException)
{
    readInt();
    char action = readByte();
    player->DoAction(action);
}
void NetworkSession::handleCloseWindow() throw (NetworkException)
{
    char windowId = readByte();
    player->CloseWindow(windowId);
}
void NetworkSession::handleClickWindow() throw (NetworkException)
{
    i_windowId windowId = readByte();
    i_slot slotId = readShort();
    char button = readByte();
    short action = readShort();
    char mode = readByte();
    const Inventory::ItemStack* itemStack = readSlot();
    DEBUG_CHAR(windowId);
    DEBUG_SHORT(slotId);
    DEBUG_CHAR(button);
    DEBUG_SHORT(action);
    DEBUG_CHAR(mode);
    player->ClickOnWindow(windowId, slotId, button, action, mode, itemStack);
    delete itemStack;
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
    int x = readInt();
    i_height y = readShort();
    int z = readInt();

    DEBUG_INT(x);
    DEBUG_SHORT(y);
    DEBUG_INT(z);
    World::World* world = player->getWorld();
    if (world)
    {
        Block::TileEntity* tileEntity = world->GetTileEntity(x, y, z);
        if (tileEntity != nullptr)
        {
            if (tileEntity->getType() == Block::TILEENTITY_TYPE_SIGN)
            {
                Block::TileEntitySign* sign = (Block::TileEntitySign*)tileEntity;
                if (sign->Editable())
                {
                    for (int i = 0; i < 4; i++)
                        sign->SetLine(i, readString(15));
                    world->MarkForNetworkUpdateTileEntity(x, y, z);
                    return;
                }
            }
        }
    }
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
    readString(255);
}
Inventory::ItemStack* NetworkSession::readSlot() throw (NetworkException)
{
    short blockId = readShort();
    if (blockId < -1)
        throw NetworkException("readSlot bad blockID");
    if (blockId != -1)
    {
        int data = readByte();
        int count = readShort();
        short nbtDataLength = readShort();
        if (nbtDataLength != -1)
        {
            for (int i = 0; i < nbtDataLength; i++)
                readByte();
        }
        return new Inventory::ItemStack(blockId, data, count);
    }
    else
    {
        return nullptr;
    }
}
void NetworkSession::handleCreativeInventoryAction() throw (NetworkException)
{
    short slotId = readShort();
    DEBUG_SHORT(slotId);
    if (slotId < -1 || slotId > 44)
        throw NetworkException("handleCreativeInventoryAction: slotId < 0 || slotId > 44");

    Inventory::ItemStack* receivedSlot = readSlot();
    if (player->GetGameMode() == World::EntityPlayer::GAMEMODE_CREATVE)
    {
        if (slotId == -1)
        {
            if (receivedSlot != nullptr)
            {
                player->DropItem(receivedSlot);
            }
        }
        else
        {
            if (slotId == player->GetHandsInventory()->getHandSlotId() + 36)
                player->ItemInHandHasChange();
            player->GetInventoryWindow()->SetSlot(slotId, receivedSlot);
        }
    }
    else
    {
        delete receivedSlot;
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
        NetworkPacket packet(OP_LOGIN_REQUEST);
        std::wstring levelType(L"flat");
        packet  << (int)1 << levelType << (char)1 << (char)0 << (char)0 << (char)0 << (char)20;
        SendPacket(packet);

        World::WorldManager& worldManager = World::WorldManager::Instance();
        player = worldManager.LoadAndJoinWorld(username, this);
        if (player != NULL)
            state = STATE_INGAME;
        else
            disconnect("Bad state handleClientStatuses");
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
    {
        delete sharedSecretKey.first;
        throw NetworkException("sharedSecretKey.second != 128");
    }

    // TODO: check it
    buffer_t verifyToken = readBuffer();
    DEBUG_SHORT(verifyToken.second)
    if (verifyToken.second != 128)
    {
        delete sharedSecretKey.first;
        delete verifyToken.first;
        throw NetworkException("verifyToken.second != 128");
    }

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

    }
    catch (CryptoPP::Exception&)
    {
        throw NetworkException("PK_DecryptorFilter");
    }

    if (sDecryptedSharedSecret.length() != 16)
    {
        delete sharedSecretKey.first;
        delete verifyToken.first;
        throw NetworkException("sharedSecretKey.second != 128");
    }

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

    delete[] sharedSecretKey.first;
    delete[] verifyToken.first;
}
void NetworkSession::handlePing() throw (NetworkException)
{
    char magic = readByte();
    DEBUG_CHAR(magic)

    std::string motd;
    (Config::Config::getConfig()).lookupValue("server.general.motd", motd);
    int maxplayers = 0;
    (Config::Config::getConfig()).lookupValue("server.general.maxplayers", maxplayers);
    std::ostringstream oss;
    oss << maxplayers;
    std::string str_maxplayers = oss.str();

    int currentPlayers = World::WorldManager::Instance().getPlayerCount();
    std::ostringstream currentPlayerStream;
    currentPlayerStream << currentPlayers;
    std::string str_currentPlayers = currentPlayerStream.str();

    std::wstring answer(L"\u00a7\u0031\u0001");
    answer += CURRENT_VERSION_PROTOCOL_WSTR;
    answer += L"\u00011.5\u0001";
    std::copy(motd.begin(), motd.end(), std::back_inserter(answer));
    answer += L"\u0001";
    std::copy(str_currentPlayers.begin(), str_currentPlayers.end(), std::back_inserter(answer));
    answer += L"\u0001";
    std::copy(str_maxplayers.begin(), str_maxplayers.end(), std::back_inserter(answer));

    NetworkPacket packet;
    packet << (unsigned char)OP_KICK;

    short strLenght = answer.size();
    unsigned char tmpBuffer[2];
    tmpBuffer[0] = (strLenght >> 8) & 0xff;
    tmpBuffer[1] = strLenght & 0xff;
    packet.append(tmpBuffer, 2);
    const wchar_t* datastr = answer.c_str();
    for (short i = 0; i < strLenght; i++)
    {
        if(datastr[i] == 1)
        {
            tmpBuffer[0] = 0;
            tmpBuffer[1] = 0;
        }
        else
        {
            tmpBuffer[0] = (datastr[i] >> 8) & 0xff;
            tmpBuffer[1] = datastr[i] & 0xff;
        }
        packet.append(tmpBuffer, 2);
    }

    SendPacket(packet);
}
void NetworkSession::handleDisconnect() throw (NetworkException)
{
    readString(128);
    disconnect("ask by client");
}

void NetworkSession::UpdateTick()
{
    if (lastKeepAliveId != 0)
    {
        lastKeepAliveTick--;
        if (lastKeepAliveTick <= 0)
        {
            disconnect("time out");
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

    while (SendPendingData());
}
}
