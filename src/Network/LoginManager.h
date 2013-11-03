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

    /**
     * Add new login to be check, this will add to work queue the login to
     * check, then a thread will check the login later. To check login
     * use return value as an checkLoginId and get result of the result
     * of the check with CheckLogin method.
     * @param name
     * @param serverId
     * @return a unique id, checkLoginId
     */
    int AskCheckLogin(const std::string& name, const std::string& serverId);
    /**
     * Get result of a login check
     * @param checkLoginId the id returned by AskCheckLogin
     * @return 1 if check success, 0 if it fail and -1 if check had not been done yet.
     */
    int CheckLogin(int checkLoginId);
    /**
     * Start WORKER_THREAD_COUNT threads that will check login
     */
    void Start();
    /**
     * Stop threads that cheking login, when all login had been check
     */
    void Stop();
private:
    /**
     * Thread main method.
     */
    void work();
    /**
     * Connect to mojang server and check if client is premium and can connect
     * to the server
     * @param login username of player
     * @param id server id, sended to client before
     * @return 1 if OK, 0 if mojang server response NO, -1 on other error
     */
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
