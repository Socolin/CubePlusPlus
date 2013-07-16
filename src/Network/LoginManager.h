#ifndef LOGINMANAGER_H_
#define LOGINMANAGER_H_

#include "Util/Singleton.h"

#include <string>
#include <thread>
#include <unordered_map>
#include <boost/lockfree/queue.hpp>

namespace Network
{

class ToDoData
{
public:
    ToDoData();
    ToDoData(const ToDoData& data);
    virtual ~ToDoData();
    int id;
    std::string name;
    std::string serverId;
};
class ResultData
{
public:
    ResultData() : id(0), success(false) {}
    ResultData(int id, bool success): id(id), success(success) {}
    int id;
    bool success;
};

#define WORKER_THREAD_COUNT 4
class LoginManager : public Util::Singleton<LoginManager>
{
    friend class Util::Singleton<LoginManager>;
public:
    LoginManager();
    virtual ~LoginManager();

    int CheckLogin(int id);
    int AskCheckLogin(const std::string& name, const std::string& serverId);
    void Start();
    void Stop();
private:
    void work();
    int checkLogin(const std::string& login, const std::string& id);
private:
    std::thread* threadList[WORKER_THREAD_COUNT];
    bool running;
    int currentId;

    boost::lockfree::queue<ToDoData*> workToDo;
    boost::lockfree::queue<ResultData*> workDone;
    std::unordered_map<int, bool> loginResult;
};

} /* namespace Network */
#endif /* LOGINMANAGER_H_ */
