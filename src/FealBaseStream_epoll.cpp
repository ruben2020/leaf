/*
 * Copyright 2021 ruben2020 https://github.com/ruben2020
 *
 * Dual-licensed under the Apache License, Version 2.0, and
 * the GNU General Public License, Version 2.0;
 * you may not use this file except in compliance
 * with either one of these licenses.
 *
 * You can find copies of these licenses in the included
 * LICENSE-APACHE2 and LICENSE-GPL2 files.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under these licenses is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the license texts for the specific language governing permissions
 * and limitations under the licenses.
 *
 * SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-only
 *
 */
 
#include "feal.h"

int  feal::BaseStream::accept_new_conn(void){return FEAL_SOCKET_ERROR;}
void feal::BaseStream::client_read_avail(socket_t client_sockfd){(void)(client_sockfd);}
void feal::BaseStream::client_write_avail(socket_t client_sockfd){(void)(client_sockfd);}
void feal::BaseStream::client_shutdown(socket_t client_sockfd){(void)(client_sockfd);}
void feal::BaseStream::server_shutdown(void){}
void feal::BaseStream::connected_to_server(socket_t fd){(void)(fd);}
void feal::BaseStream::connection_read_avail(void){}
void feal::BaseStream::connection_write_avail(void){}
void feal::BaseStream::connection_shutdown(void){}

void feal::BaseStream::serverLoop(void)
{
    int nfds = 0;
    struct epoll_event events[max_events];
    epfd = epoll_create(1);
    if (epoll_ctl_add(epfd, sockfd, (EPOLLIN | EPOLLRDHUP | EPOLLHUP)) == -1)
    {
        FEALDEBUGLOG("epoll_ctl_add error");
        return;
    }
    for (;;)
    {
        nfds = epoll_wait(epfd, events, max_events, 500);
        if (nfds == -1)
        {
            if (errno == EINTR) continue;
            else break;
        }
        for (int i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == sockfd)
            {
                if (((events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0)||
                    (accept_new_conn() == -1))
                {
                    do_full_shutdown();
                    server_shutdown();
                    break;
                }
                continue;
            }
            else if ((events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0)
            {
                do_client_shutdown(events[i].data.fd);
                client_shutdown(events[i].data.fd);
                continue;
            }
            else if ((events[i].events & EPOLLIN) == EPOLLIN)
            {
                client_read_avail(events[i].data.fd);
                continue;
            }
            else if ((events[i].events & EPOLLOUT) == EPOLLOUT)
            {
                client_write_avail(events[i].data.fd);
                epoll_ctl_mod(epfd, events[i].data.fd, 
                    (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP));
                continue;
            }
        }
    }
}

int feal::BaseStream::do_client_read_start(feal::socket_t client_sockfd)
{
    return epoll_ctl_add(epfd, client_sockfd, 
        (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP));
}

int feal::BaseStream::do_client_shutdown(feal::socket_t client_sockfd)
{
    epoll_ctl(epfd, EPOLL_CTL_DEL, client_sockfd, NULL);
    return shutdown(client_sockfd, SHUT_RDWR);
}

int feal::BaseStream::do_full_shutdown(void)
{
    int res = 0;
    if (epfd != -1)
    {
        epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, NULL);
        close(epfd);
    }
    if (sockfd != -1) res = shutdown(sockfd, SHUT_RDWR);
    epfd = -1;
    sockfd = -1;
    waitingforconn = false;
    return res;
}

void feal::BaseStream::do_connect_in_progress(void)
{
    epfd = epoll_create(1);
    waitingforconn = true;
    if (epoll_ctl_add(epfd, sockfd, 
        (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP | EPOLLOUT)) == -1)
    {
        FEALDEBUGLOG("epoll_ctl error");
        return;
    }
}

void feal::BaseStream::do_connect_ok(void)
{
    epfd = epoll_create(1);
    waitingforconn = false;
    if (epoll_ctl_add(epfd, sockfd, 
        (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP)) == -1)
    {
        FEALDEBUGLOG("epoll_ctl error");
        return;
    }
    connected_to_server(sockfd);
}

void feal::BaseStream::do_send_avail_notify(feal::socket_t fd)
{
    if (epoll_ctl_mod(epfd, fd, 
        (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP | EPOLLOUT)) == -1)
    {
        FEALDEBUGLOG("epoll_ctl error");
        return;
    }
}

void feal::BaseStream::connectLoop(void)
{
    int nfds = 0;
    struct epoll_event events[max_events];
    for (;;)
    {
        nfds = epoll_wait(epfd, events, max_events, 500);
        if (nfds == -1)
        {
            if (errno == EINTR) continue;
            else break;
        }
        for (int i = 0; i < nfds; i++)
        {
            if ((events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) != 0)
            {
                do_full_shutdown();
                connection_shutdown();
                break;
            }
            else if ((events[i].events & EPOLLIN) == EPOLLIN)
            {
                connection_read_avail();
                continue;
            }
            else if ((events[i].events & EPOLLOUT) == EPOLLOUT)
            {
                if (waitingforconn)
                {
                    waitingforconn = false;
                    connected_to_server(events[i].data.fd);
                }
                else
                {
                    connection_write_avail();
                }
                epoll_ctl_mod(epfd, sockfd, 
                    (EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLHUP));
                continue;
            }
        }
    }
}

#if defined (__linux__)
int feal::BaseStream::epoll_ctl_add(int epfd, socket_t fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
}

int feal::BaseStream::epoll_ctl_mod(int epfd, socket_t fd, uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    return epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}
#endif
