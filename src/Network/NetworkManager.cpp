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

namespace Network
{

NetworkManager::NetworkManager()
{
    sfd = 0;
    s = 0;
    efd = 0;
    events = nullptr;
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
    int listenfd = 0;

    struct sockaddr_in serv_addr;

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY );
    serv_addr.sin_port = htons(port);

    int result = bind(listenfd, (struct sockaddr*) &serv_addr,
                      sizeof(serv_addr));
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
        std::cerr << "Could not bind" << std::endl;
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
    /* The event loop */
    int n, i;

    n = epoll_wait(efd, events, MAXEVENTS, 1);
    for (i = 0; i < n; i++)
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
                    printf("Accepted connection on descriptor %d "
                           "(host=%s, port=%s)\n", infd, hbuf, sbuf);
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
                OnNewClient(infd);
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
                        std::cerr << "Client network err:" << e.what() << std::endl;
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
}

void NetworkManager::OnNewClient(int socket)
{
    NetworkSession* session = new NetworkSession(socket);
    sessionList[socket] = session;
}

void NetworkManager::OnDisconnectClient(int socket)
{
    close(socket);
    epoll_ctl(efd, EPOLL_CTL_DEL, socket, &event);
    auto sessionItr = sessionList.find(socket);
    if (sessionItr != sessionList.end())
    {
        if (!sessionItr->second->isDisconnected())
            sessionItr->second->disconnect("OnDisconnectClient");
        sessionList.erase(socket);
        //delete sessionItr->second;
    }
}


}
