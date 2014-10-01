#include "LoginManager.h"

#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <unistd.h>
#include <netdb.h>

#include "Logging/Logger.h"

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
            usleep(10000);
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
        delete threadList[i];
    }
}


int LoginManager::checkLogin(const std::string& login, const std::string& id)
{
    // Here I don't use LOG_DEBUG/LOG_INFO because these are not thread safe yet.

    const char* CRLF = "\r\n";

    // Connect to session/minecraft.net
    struct addrinfo resolveInfo;
    struct addrinfo *connectInfo = nullptr;

    memset(&resolveInfo, 0, sizeof resolveInfo);
    resolveInfo.ai_family = AF_UNSPEC;
    resolveInfo.ai_socktype = SOCK_STREAM;

    int res = getaddrinfo("session.minecraft.net", "80", &resolveInfo, &connectInfo);
    if (res == -1)
    {
        freeaddrinfo(connectInfo);
        perror("[LOGIN]getaddrinfo");
        return -1;
    }

    int sockfd = socket(connectInfo->ai_family, connectInfo->ai_socktype, connectInfo->ai_protocol);
    if (sockfd == -1)
    {
        perror("[LOGIN]socket");
        return -1;
    }

    res = connect(sockfd, connectInfo->ai_addr, connectInfo->ai_addrlen);
    if (res == -1)
    {
        close(sockfd);
        perror("[LOGIN]connect");
        return -1;
    }

    freeaddrinfo(connectInfo);
    connectInfo = nullptr;

    // Send HTTP/1.1 request to server
    std::stringstream request;

    request << "GET /game/checkserver.jsp?user=" << login << "&serverId=" << id << " HTTP/1.1" << CRLF;
    request << "Host: session.minecraft.net"<< CRLF;
    request  << CRLF;

    const std::string& requestStr = request.str();
    res = write(sockfd, requestStr.c_str(), requestStr.length());
    if (res == -1)
    {
        close(sockfd);
        perror("[LOGIN]write");
        return -1;
    }

    // Parse result
    char buffer[512];

    int len = read(sockfd, buffer, 512);
    if (len == -1)
    {
        close(sockfd);
        perror("[LOGIN]read");
        return -1;
    }
    bool findCRLF = false;
    bool findCR = false;
    int pos = 0;
    bool requestNotComplete = true;
    while (requestNotComplete)
    {
        if (buffer[pos] == '\r')
        {
            findCR = true;
        }
        else if (findCR && buffer[pos] == '\n')
        {
            if (findCRLF)
            {
                pos++;
                break;
            }
            findCRLF = true;
        }
        else
        {
            findCR = false;
            findCRLF = false;
        }
        pos++;
        if (pos >= len)
        {
            len = read(sockfd, buffer, 512);
            if (len == -1)
            {
                close(sockfd);
                perror("[LOGIN]read2");
                return -1;
            }
            pos = 0;
        }

    }
    close(sockfd);

    if (pos < len)
    {
        if (buffer[pos] == 'Y')
            return 1;
        else
            return 0;
    }
    return -1;

    return -1;
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

