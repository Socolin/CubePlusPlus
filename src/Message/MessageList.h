#ifndef MESSAGELIST_H_
#define MESSAGELIST_H_

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
    static std::wstring GetMessage(int langId, const std::string& key);
private:
    MessageList();
    void Initialize();
    std::vector<std::map<std::string, std::wstring>> messageList;
    std::string lang;
};

} /* namespace Message */

#endif /* MESSAGELIST_H_ */
