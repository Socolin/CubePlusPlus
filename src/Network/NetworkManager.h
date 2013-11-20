#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <unordered_map>
#include <sys/epoll.h>
#include <vector>

namespace Network
{

#define MAXEVENTS 1024

class NetworkSession;

class NetworkManager
{
public:
    NetworkManager();
    virtual ~NetworkManager();

    bool StartServer(unsigned short port);
    void ReceiveData();
    void StopServer();
private:
    NetworkSession* OnNewClient(int socket, const std::string& ip);
    void OnDisconnectClient(NetworkSession* session);
private:
    std::unordered_map<int, NetworkSession*> sessionList;
    std::vector<int> closedSocket;
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
};
}
#endif /* NETWORKMANAGER_H_ */
