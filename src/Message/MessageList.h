#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

#include "Database/DatabaseManager.h"
#include "Util/Singleton.h"
#include <map>
#include <vector>

namespace Message
{

class MessageList : public Util::Singleton<MessageList>
{
    friend Util::Singleton<MessageList>;
public:
    virtual ~MessageList();
    void InitInstance() override;
    static std::wstring GetMessage(const std::string& key);
private:
    MessageList();
    void InitializeVector(Database::DatabaseManager& db);
    void InitializeMessageList(Database::DatabaseManager& db);
    void InitializeLang(Database::DatabaseManager& db);
    std::vector<std::map<std::string, std::wstring>> messageList;
    std::string configLang;
    int requestedLanguage;
};

} /* namespace Message */

#endif /* MESSAGELIST_H_ */
