#include "MessageList.h"

#include "Config/Config.h"
#include "Database/DatabaseManager.h"
#include "Logging/Logger.h"
#include "Util/StringUtil.h"

namespace Message
{

MessageList::MessageList()
    : lang("english")
{
    Config::Config::GetConfig().lookupValue("server.general.lang", lang);
}

MessageList::~MessageList()
{
}

void MessageList::InitInstance()
{
    Initialize();
}

std::wstring MessageList::GetMessage(int langId, const std::string& key)
{
    int msgListSize = Instance().messageList.size();
    if(msgListSize <= langId)
    {
        LOG_ERROR << "GetMessage : Not a valid language" << std::endl;
        return(L"");
    }
    auto itr = Instance().messageList[langId].find(key);
    if( itr != Instance().messageList[langId].end())
    {
        return (*itr).second;
    }
    if(langId != 0)
    {
        LOG_ERROR << "Message with key : " << key << " not found for selected language : " << Instance().lang << ", searching english message instead" << std::endl;
        auto itrEng = Instance().messageList[0].find(key);
        if(itrEng != Instance().messageList[langId].end())
        {
            return (*itrEng).second;
        }
    }
    LOG_ERROR << "No message found for key : " << key << std::endl;
    return (L"");
}

void MessageList::Initialize()
{
    Database::DatabaseManager* db = Database::DatabaseManager::Instance();
    db->connect();

    sql::ResultSet* result = db->querry("SELECT * FROM `message` ORDER BY `langId` ASC");

    if(result == nullptr)
    {
        LOG_ERROR << "No Text found in database" << std::endl;
        return;
    }

    LOG_INFO << "Loading Text" << std::endl;

    LOG_DEBUG << Logging::BOLD_BLUE
              << "id" << "\t"
              << "langId" << "\t"
              << "key" << "\t"
              << "text" << "\t"
              << std::endl;

    int langItr = 0;
    std::map<std::string, std::wstring> tempMessage;

    while (result->next())
    {
        int id = result->getInt(TableMessage::id);
        int langId = result->getInt(TableMessage::langId);
        std::string key = result->getString(TableMessage::key);
        std::string text = result->getString(TableMessage::text);

        LOG_DEBUG << id << "\t"
                  << langId << "\t"
                  << key << "\t"
                  << text << "\t"
                  << std::endl;

        std::wstring wText;
        Util::StringToWString(wText, text);

        if(langId > langItr)
        {
            messageList.push_back(tempMessage);
            tempMessage.clear();
            langItr = langId;
        }
        tempMessage[key] = wText;
    }

    messageList.push_back(tempMessage);

    LOG_DEBUG << "Available langage : " << messageList.size() << std::endl;

    delete result;
}

} /* namespace Message */
