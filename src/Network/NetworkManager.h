#ifndef NETWORKMANAGER_H_
#define NETWORKMANAGER_H_

#include <unordered_map>
#include <sys/epoll.h>

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
    void OnNewClient(int socket);
    void OnDisconnectClient(int socket);
private:
    std::unordered_map<int, NetworkSession*> sessionList;
    int sfd, s;
    int efd;
    struct epoll_event event;
    struct epoll_event *events;
};
}
#endif /* NETWORKMANAGER_H_ */
