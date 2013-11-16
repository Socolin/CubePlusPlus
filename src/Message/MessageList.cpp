#include "MessageList.h"

#include <sstream>

#include "Config/Config.h"
#include "Database/DatabaseManager.h"
#include "Logging/Logger.h"
#include "Util/StringUtil.h"

namespace Message
{

MessageList::MessageList()
    : configLang("english")
    , requestedLanguage(0)
{
    Config::Config::GetConfig().lookupValue("server.general.lang", configLang);
}

MessageList::~MessageList()
{
}

void MessageList::InitInstance()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();
    InitializeVector(db);
    InitializeMessageList(db);
    InitializeLang(db);
}

std::wstring MessageList::GetMessage(const std::string& key)
{
    auto itr = Instance().messageList[Instance().requestedLanguage].find(key);
    if( itr != Instance().messageList[Instance().requestedLanguage].end())
    {
        return (*itr).second;
    }
    if(Instance().requestedLanguage != 0)
    {
        LOG_ERROR << "Message with key : " << key << " not found for selected language : " << Instance().configLang << ", searching english message instead" << std::endl;
        auto itrEng = Instance().messageList[0].find(key);
        if(itrEng != Instance().messageList[0].end())
        {
            return (*itrEng).second;
        }
    }
    LOG_ERROR << "No message found for key : " << key << std::endl;
    return (L"");
}

void MessageList::InitializeVector(Database::DatabaseManager* db)
{
    sql::ResultSet* result = db->querry("SELECT MAX(`id`) FROM `message_lang`");

    if(result == nullptr)
    {
        LOG_ERROR << "Table message_lang is empty" << std::endl;
        return;
    }
    result->first();
    int vectSize = result->getInt(TableMessageLang::id) + 1;
    messageList.resize(vectSize);
    LOG_DEBUG << "Available language : " << vectSize << std::endl;
    delete result;
}

void MessageList::InitializeMessageList(Database::DatabaseManager* db)
{
    sql::ResultSet* result = db->querry("SELECT * FROM `message` ORDER BY `langId` ASC");

    if(result == nullptr)
    {
        LOG_ERROR << "No Text found in database" << std::endl;
        return;
    }
    LOG_INFO << "Loading message text" << std::endl;
    LOG_DEBUG << Logging::BOLD_BLUE
              << "id" << "\t"
              << "langId" << "\t"
              << "key"
              << std::endl;
    int langItr = 0;
    std::map<std::string, std::wstring> tempMessage;

    while (result->next())
    {
        int id = result->getInt(TableMessage::id);
        int langId = result->getInt(TableMessage::langId);
        std::string key = result->getString(TableMessage::key);
        std::string text = result->getString(TableMessage::text);

        LOG_DEBUG << id << "\t\t"
                  << langId << "\t"
                  << key
                  << std::endl;

        std::wstring wText;
        Util::StringToWString(wText, text);

        if(langId > langItr)
        {
            messageList[langItr] = tempMessage;
            tempMessage.clear();
            langItr = langId;
        }
        tempMessage[key] = wText;
    }
    messageList[langItr] = tempMessage;
    delete result;
}

void MessageList::InitializeLang(Database::DatabaseManager* db)
{
    std::ostringstream querry;
    querry << "SELECT DISTINCT `id` FROM `message_lang` WHERE UPPER(`lang`) LIKE UPPER('%" << configLang << "%')";
    sql::ResultSet* result = db->querry(querry.str());

    if(result == nullptr || !result->first())
    {
        LOG_ERROR << "Requested language '" << configLang << "' not available, english will be used" << std::endl;
        requestedLanguage = 0;
    }
    else
    {
        result->first();
        requestedLanguage = result->getInt(TableMessageLang::id);
    }
    delete result;
}

} /* namespace Message */
