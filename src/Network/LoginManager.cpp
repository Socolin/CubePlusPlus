#include "LoginManager.h"

#include <SFML/Network.hpp>
#include <sstream>

namespace Network
{

LoginManager::LoginManager()
    : running(true), currentId(0), workToDo(1024), workDone(1024)
{
}

LoginManager::~LoginManager()
{
}

int LoginManager::AskCheckLogin(const std::string& name, const std::string& serverId)
{
    int id = currentId++;
    ToDoData* data = new ToDoData();
    data->id = id;
    data->name = name;
    data->serverId = serverId;
    workToDo.push(data);
    return id;
}

int LoginManager::CheckLogin(int id)
{
    ResultData* data = nullptr;
    while (workDone.pop(data))
    {
        if (data->id == id)
        {
            return data->success ? 1 : 0;
        }
        loginResult[data->id] = data->success;
        delete data;
        data = nullptr;
    }
    auto loginItr = loginResult.find(id);
    if (loginItr != loginResult.end())
    {
        bool success = (*loginItr).second;
        loginResult.erase(loginItr);
        return success ? 1 : 0;
    }
    return -1;
}

void LoginManager::work()
{
    ToDoData* data;
    bool poped = false;
    while ((poped = workToDo.pop(data)) || running)
    {
        if (poped)
        {
            int ret = checkLogin(data->name, data->serverId);
            ResultData* result = new ResultData{data->id, ret == 1};
            delete data;
            data = nullptr;
            workDone.push(result);
        }
        else
        {
            usleep(1000);
        }
    }
}
void LoginManager::Start()
{
    for (int i = 0; i < WORKER_THREAD_COUNT; ++i) {
        threadList[i] = new std::thread(&LoginManager::work, this);
    }
}

void LoginManager::Stop()
{
    running = false;
    for (int i = 0; i < WORKER_THREAD_COUNT; ++i) {
        threadList[i]->join();
    }
}


int LoginManager::checkLogin(const std::string& login, const std::string& id)
{
    sf::Http http("http://session.minecraft.net");
    sf::Http::Request request;
    request.setMethod(sf::Http::Request::Get);
    std::stringstream stream;
    stream << "/game/checkserver.jsp?user=" << login << "&serverId=" << id;
    request.setUri(stream.str());
    request.setHttpVersion(1, 1); // HTTP 1.1
    sf::Http::Response response = http.sendRequest(request, sf::seconds(1));

    if (response.getStatus() == sf::Http::Response::Ok)
    {
        if (response.getBody() == "YES")
            return 1;
        return 0;
    }
    else if (response.getStatus() == sf::Http::Response::ConnectionFailed)
    {
        return -1;
    }
    return -2;
}

ToDoData::ToDoData()
    : id(0)
{
}

ToDoData::ToDoData(const ToDoData& data)
{
    id = data.id;
    name = data.name;
    serverId = data.serverId;
}

ToDoData::~ToDoData()
{
}
} /* namespace Network */

