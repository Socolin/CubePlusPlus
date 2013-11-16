#include "NetworkSession.h"
#include "NetworkPacket.h"
#include "NetworkEncryption.h"
#include "Opcode.h"
#include "../Config/Config.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cryptopp/filters.h>
#include <cryptopp/osrng.h>
#include <cryptopp/files.h>
#include <cryptopp/cryptlib.h>

#include "Block/TileEntities/TileEntitySign.h"
#include "World/WorldManager.h"
#include "World/World.h"
#include "Entity/EntityPlayer.h"
#include "Window/Window.h"
#include "LoginManager.h"
#include "Logging/Logger.h"
#include "Util/StringUtil.h"
#include "Message/MessageList.h"

#ifndef NDEBUG
# define DEBUG_STR(value)        LOG_DEBUG << "\t" << #value << "String: size: "<< value.length() << " value:\"" << value << "\"" << std::endl;
# define DEBUG_SHORT(value)      LOG_DEBUG << "\t" << #value << "\tShort: " << value << std::endl;
# define DEBUG_BOOL(value)       LOG_DEBUG << "\t" << #value << "\tBool: " << value << std::endl;
# define DEBUG_INT(value)        LOG_DEBUG << "\t" << #value << "\tInt: " << value << std::endl;
# define DEBUG_CHAR(value)       LOG_DEBUG << "\t" << #value << "\tChar: " << (int)value << std::endl;
#else
# define DEBUG_STR(value)
# define DEBUG_SHORT(value)
# define DEBUG_BOOL(value)
# define DEBUG_INT(value)
# define DEBUG_CHAR(value)
#endif

namespace Network
{
void NetworkSession::handleKeepAlive() throw (NetworkException)
{
    int value = readInt();
    if (value == 0)
        return;
    if (value != lastKeepAliveId)
    {
        kick(std::wstring(L"Bad keepalive message"));
    }
    lastKeepAliveId = 0;
}
void NetworkSession::handleChatMessage() throw (NetworkException)
{
    std::wstring message = readString(100);

    DEBUG_STR(message);

    World::WorldManager::Instance().HandleChatMessage(player, message);
}
void NetworkSession::handleHandShake() throw (NetworkException)
{
    World:: WorldManager& worldManager = World:: WorldManager::Instance();

    unsigned char protocolVersion = readByte();

    if (protocolVersion != CURRENT_VERSION_PROTOCOL)
    {
        kick(Message::MessageList::GetMessage("network.kick.badprotocol"));
        return;
    }

    username = readString(16);

    if (worldManager.IsFull() && !worldManager.IsAdmin(username))
    {
        kick(Message::MessageList::GetMessage("network.kick.serverisfull"));
        return;
    }

    if (worldManager.IsBan(username))
    {
        kick(Message::MessageList::GetMessage("network.kick.isban"));
        return;
    }

    if(!worldManager.IsWhitelisted(username))
    {
        kick(Message::MessageList::GetMessage("network.kick.notinwhitelist"));
        return;
    }

    std::wstring serverHost = readString(255);
    int serverPort = readInt();

    DEBUG_CHAR(protocolVersion)
    DEBUG_STR(username)
    DEBUG_STR(serverHost)
    DEBUG_INT(serverPort)

    NetworkEncryption& encrypt = NetworkEncryption::Instance();
    buffer_t certificate = encrypt.GetCertificate();

    if (!worldManager.IsOnlineMode())
    {
        serverId = L"-";
    }
    else
    {
        std::stringstream stream;
        stream << rand() << rand();
        serverIdStr = stream.str();
        Util::StringToWString(serverId, serverIdStr);
    }


    int rnd = rand();
    const std::pair<char*, short> token = std::make_pair((char*)&rnd, (short)4);

    NetworkPacket packet(OP_ENCRYPTION_KEY_REQUEST);
    packet << serverId << certificate << token;
    SendPacket(packet);
}
void NetworkSession::handleUseEntity() throw (NetworkException)
{
    int user = readInt();
    int target = readInt();
    bool leftClick = readByte();

    DEBUG_INT(user);
    DEBUG_INT(target);
    DEBUG_BOOL(leftClick);

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

    DEBUG_CHAR(state);
    DEBUG_INT(x);
    DEBUG_CHAR(y);
    DEBUG_INT(z);
    DEBUG_CHAR(face);

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

    DEBUG_CHAR(face);
    DEBUG_INT(x);
    DEBUG_CHAR(y);
    DEBUG_INT(z);
    DEBUG_CHAR(cursorPositionX);
    DEBUG_CHAR(cursorPositionY);
    DEBUG_CHAR(cursorPositionZ);

    player->PlaceBlock(x, y, z, face, cursorPositionX, cursorPositionY, cursorPositionZ);
}
void NetworkSession::handleHeldItemChange() throw (NetworkException)
{
    int slotId = readShort();
    if (slotId > 8 || slotId < 0)
        throw NetworkException("handleHeldItemChange: SlotID > 8 || < 0");

    DEBUG_INT(slotId)
    player->GetHandsInventory()->setHandSlot(slotId);
    player->ItemInHandHasChange();
}
void NetworkSession::handleAnimation() throw (NetworkException)
{
    int entityId = readInt();
    char animationId = readByte();
    DEBUG_INT(entityId);
    DEBUG_CHAR(animationId);
    player->PlayAnimation(animationId);
}
void NetworkSession::handleEntityAction() throw (NetworkException)
{
    readInt();
    char action = readByte();

    DEBUG_CHAR(action);

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
    char windowId = readByte();
    char enchantmentSlot = readByte();

    DEBUG_CHAR(windowId);
    DEBUG_CHAR(enchantmentSlot)
}
void NetworkSession::handleUpdateSign() throw (NetworkException)
{
    int x = readInt();
    i_height y = readShort();
    int z = readInt();

    DEBUG_INT(x);
    DEBUG_SHORT(y);
    DEBUG_INT(z);
    World::World* world = player->GetWorld();
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
    char flag = readByte();
    readByte();
    readByte();

    DEBUG_CHAR(flag);
    //bool flying = flag & 0x2;
}
void NetworkSession::handleTabComplete() throw (NetworkException)
{
    const std::wstring& message = readString(255);
    DEBUG_STR(message);
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
    if (player != nullptr)
        player->ShowCape(showCape);

}
void NetworkSession::handleClientStatuses() throw (NetworkException)
{
    char payload = readByte();
    DEBUG_CHAR(payload)

    if (payload == 0 && (state & STATE_INGAME) == 0)
    {
        World::WorldManager& worldManager = World::WorldManager::Instance();
        if (!worldManager.IsOnlineMode())
        {
            LOG_INFO << "Player join [" << username << "] ip:[" << ip << "]" << std::endl;
            player = worldManager.LoadAndJoinWorld(username, this);
            if (player != NULL)
                state = STATE_INGAME;
            else
                kick(std::wstring(L"Error 25"));
        }
        else
        {
            std::string name;
            Util::WStringToString(username, name);

            uint8_t digest[40] =
            { 0 };

            CryptoPP::SHA1 sha1Encoder;
            sha1Encoder.Update((uint8_t*)serverIdStr.c_str(), serverIdStr.size());
            sha1Encoder.Update((uint8_t*)decryptedSecretKey.c_str(), decryptedSecretKey.size());
            const buffer_t& certificate = NetworkEncryption::Instance().GetCertificate();
            sha1Encoder.Update((uint8_t*)certificate.first, certificate.second);
            sha1Encoder.Final(digest);

            std::ostringstream hashOutput;
            if (digest[0] & 0x80)
            {
                unsigned char carry = 1;
                hashOutput << '-';
                // Print as negative value... WTF
                for (int i = 19; i >= 0; i--)
                {
                    unsigned short value = (unsigned char)~digest[i];
                    value += carry;
                    if (value & 0xff00)
                    {
                        value = value & 0xff;
                    }
                    else
                    {
                        carry = 0;
                    }
                    digest[i] = value;
                }
            }

            hashOutput << std::hex;
            hashOutput.fill('0');
            for (const unsigned char * ptr = digest; ptr < digest + 20; ptr++)
                hashOutput << std::setw(2) << (unsigned int)*ptr;
            hashOutput << std::dec;

            std::string result = hashOutput.str();
            while (result[0] == '0')
                result = result.substr(1, result.size() - 1);
            waitLoginId = LoginManager::Instance().AskCheckLogin(name, result);
            state = STATE_WAIT_LOGGIN;
        }
    }
}
void NetworkSession::handlePluginMessage() throw (NetworkException)
{
    const std::wstring& channel = readString(128);
    DEBUG_STR(channel);
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
    buffer_t sharedSecretKey = readBuffer();
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

    NetworkEncryption& encrypt = NetworkEncryption::Instance();

    CryptoPP::RSAES<CryptoPP::PKCS1v15>::Decryptor rsaDecryptor(*encrypt.getPrivateKey());

    std::string sEncryptedSharedSecret(sharedSecretKey.first,sharedSecretKey.second);

    try
    {
        CryptoPP::StringSource StrSrc(
            sEncryptedSharedSecret,
            true,
            new CryptoPP::PK_DecryptorFilter(encrypt.getAutoSeed(), rsaDecryptor, new CryptoPP::StringSink(decryptedSecretKey))
        );
    }
    catch (CryptoPP::Exception&)
    {
        throw NetworkException("PK_DecryptorFilter");
    }

    if (decryptedSecretKey.length() != 16)
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

    memcpy(aesDecryptBuffer, decryptedSecretKey.c_str(), 16);
    aesDecryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption((byte*)decryptedSecretKey.c_str(), (unsigned int)16, aesDecryptBuffer, 1);

    memcpy(aesEncryptBuffer, decryptedSecretKey.c_str(), 16);
    aesEncryptor = new CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption((byte*)decryptedSecretKey.c_str(), (unsigned int)16, aesEncryptBuffer, 1);

    state = STATE_LOGGING;

    delete[] sharedSecretKey.first;
    delete[] verifyToken.first;
}
void NetworkSession::handlePing() throw (NetworkException)
{
    char magic = readByte();
    DEBUG_CHAR(magic)

    const World::WorldManager& worldMgr = World::WorldManager::Instance();
    const std::string& desc = worldMgr.GetDescription();
    int maxplayers = worldMgr.GetMaxPlayerCount();
    std::ostringstream oss;
    oss << maxplayers;
    std::string str_maxplayers = oss.str();

    int currentPlayers = World::WorldManager::Instance().GetPlayerCount();
    std::ostringstream currentPlayerStream;
    currentPlayerStream << currentPlayers;
    std::string str_currentPlayers = currentPlayerStream.str();

    std::wstring answer(L"\u00a7\u0031\u0001");
    answer += CURRENT_VERSION_PROTOCOL_WSTR;
    answer += L"\u00011.5.2\u0001";
    std::copy(desc.begin(), desc.end(), std::back_inserter(answer));
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
    disconnect(std::wstring(L"Ask by client"));
}

bool NetworkSession::UpdateTick()
{
    if (isDisconnected())
        return false;
    if (state == STATE_WAIT_LOGGIN)
    {
        int ret = LoginManager::Instance().CheckLogin(waitLoginId);
        if (ret < 0)
            return true;
        if (ret == 1)
        {
            World::WorldManager& worldManager = World::WorldManager::Instance();
            player = worldManager.LoadAndJoinWorld(username, this);
            LOG_INFO << "Player join [" << username << "] ip:[" << ip << "]" << std::endl;
            if (player != NULL)
            {
                LOG_INFO << "Player join [" << username << "] ip:[" << ip << "]" << std::endl;
                state = STATE_INGAME;
            }
            else
            {
                kick(L"Error 42");
                return false;
            }
        }
        else
        {
            kick(std::wstring(L"Bad login, or minecraft.net is down"));
            return false;
        }
        return true;
    }
    if (state != STATE_INGAME)
        return true;
    if (lastKeepAliveId != 0)
    {
        if (lastKeepAliveTick <= 0)
        {
            kick(std::wstring(L"Time out"));
        }
        else
            lastKeepAliveTick--;
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
    while (sendBuffer.Send(socket) > 0);

    return true;
}
}
