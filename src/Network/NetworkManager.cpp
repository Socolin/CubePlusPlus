#include "NetworkManager.h"
#include "NetworkSession.h"
#include "NetworkException.h"

#include <iostream>
#include <stdio.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>

#include "Logging/Logger.h"

namespace Network
{

NetworkManager::NetworkManager()
    : sfd(0), s(0), efd(0), events(nullptr)
{
    event = epoll_event {0,epoll_data_t{nullptr}};
}

NetworkManager::~NetworkManager()
{
    free(events);
    for (auto sessionItr : sessionList)
    {
        delete sessionItr.second;
    }
}

static int create_and_bind(unsigned short port)
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY );
    serv_addr.sin_port = htons(port);

    int result = bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

    if (result != 0)
        return -1;

    return listenfd;
}

static int make_socket_non_blocking(int sfd)
{
    int flags, s;

    flags = fcntl(sfd, F_GETFL, 0);
    if (flags == -1)
    {
        perror("fcntl");
        return -1;
    }

    flags |= O_NONBLOCK;
    s = fcntl(sfd, F_SETFL, flags);
    if (s == -1)
    {
        perror("fcntl");
        return -1;
    }

    return 0;
}

bool NetworkManager::StartServer(unsigned short port)
{
    sfd = create_and_bind(port);
    if (sfd == -1)
    {
        LOG_ERROR << "Could not bind on port" << port << std::endl;
        return false;
    }
    s = make_socket_non_blocking(sfd);
    if (s == -1)
        return false;

    s = listen(sfd, SOMAXCONN);
    if (s == -1)
    {
        perror("listen");
        return false;
    }

    efd = epoll_create1(0);
    if (efd == -1)
    {
        perror("epoll_create");
        return false;
    }

    event.data.fd = sfd;
    event.events = EPOLLIN | EPOLLET;
    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
    if (s == -1)
    {
        perror("epoll_ctl");
        return false;
    }

    /* Buffer where events are returned */
    events = (epoll_event*) calloc(MAXEVENTS, sizeof event);

    return true;
}

void NetworkManager::ReceiveData()
{
    for (int socket : closedSocket)
    {
        epoll_ctl(efd, EPOLL_CTL_DEL, socket, &event);
        NetworkSession* session = sessionList[socket];
        if (session)
        {
            session->CloseForDelete();
            delete session;
        }
        else
        {
            close(socket);
        }
        sessionList.erase(socket);
    }
    closedSocket.clear();
    // Receive data and do somthing with it
    int countEvent = epoll_wait(efd, events, MAXEVENTS, 1);
    for (int i = 0; i < countEvent; i++)
    {
        if ((events[i].events & EPOLLERR) || (events[i].events & EPOLLHUP)
                || (!(events[i].events & EPOLLIN)))
        {
            // Déconnexion d'un client
            OnDisconnectClient(events[i].data.fd);
            continue;
        }
        else if (sfd == events[i].data.fd)
        {
            /* We have a notification on the listening socket, which
             means one or more incoming connections. */
            while (true)
            {
                struct sockaddr in_addr;
                socklen_t in_len;
                int infd;
                char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

                in_len = sizeof in_addr;
                infd = accept(sfd, &in_addr, &in_len);
                if (infd == -1)
                {
                    if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                    {
                        /* We have processed all incoming
                         connections. */
                        break;
                    }
                    else
                    {
                        perror("accept");
                        break;
                    }
                }

                s = getnameinfo(&in_addr, in_len, hbuf, sizeof hbuf, sbuf,
                                sizeof sbuf, NI_NUMERICHOST | NI_NUMERICSERV);
                if (s == 0)
                {
                    LOG_DEBUG << "Accepted connection on descriptor " << infd << "(host=" << hbuf << ", port=" << sbuf << ")" << std::endl;
                }

                /* Make the incoming socket non-blocking and add it to the
                 list of fds to monitor. */
                s = make_socket_non_blocking(infd);
                if (s == -1)
                    continue;

                event.data.fd = infd;
                event.events = EPOLLIN | EPOLLET;
                s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
                if (s == -1)
                {
                    perror("epoll_ctl");
                    continue;
                }
                OnNewClient(infd, hbuf);
            }
            continue;
        }
        else
        {
            /* We have data on the fd waiting to be read. Read and
             display it. We must read whatever data is available
             completely, as we are running in edge-triggered mode
             and won't get a notification again for the same
             data. */

            NetworkSession* session = sessionList[events[i].data.fd];

            if (events[i].events & EPOLLIN)
            {
                if (session->isDisconnected())
                {
                    OnDisconnectClient(events[i].data.fd);
                }
                else
                {
                    try
                    {
                        session->ReceiveData();
                    }
                    catch (NetworkException &e)
                    {
                        LOG_ERROR << "Client network err:" << e.what() << std::endl;
                        OnDisconnectClient(events[i].data.fd);
                    }
                }
            }
            else if (events[i].events & EPOLLRDHUP)
            {
                OnDisconnectClient(events[i].data.fd);
            }
            else if (events[i].events & EPOLLERR)
            {
                OnDisconnectClient(events[i].data.fd);
            }
        }
    }
    for (auto sessionItr : sessionList)
    {
        Network::NetworkSession* session = sessionItr.second;
        if (session != nullptr)
        {
            if (!session->UpdateTick())
            {
                OnDisconnectClient(sessionItr.first);
            }
        }
    }
}

void NetworkManager::StopServer()
{
    for (auto session : sessionList)
    {
        OnDisconnectClient(session.first);
    }
    sessionList.clear();
    free(events);
    close(sfd);
    close(efd);
}

void NetworkManager::OnNewClient(int socket, const std::string& ip)
{
    NetworkSession* session = new NetworkSession(socket, ip);
    sessionList[socket] = session;
}

void NetworkManager::OnDisconnectClient(int socket)
{
    auto sessionItr = sessionList.find(socket);
    if (sessionItr != sessionList.end())
    {
        if (!sessionItr->second->isDisconnected())
            sessionItr->second->disconnect(std::wstring(L"OnDisconnectClient"));
        closedSocket.push_back(socket);
    }
}


}
