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
 
#include <cstdio>
#include <cstring>
#include "client.h"

#define MIN(a,b) (a<b ? a : b)

feal::EventId_t EvtEndTimer::getId(void)
{
    return getIdOfType<EvtEndTimer>();
}

feal::EventId_t EvtDelayTimer::getId(void)
{
    return getIdOfType<EvtDelayTimer>();
}

void Client::initActor(void)
{
    printf("Client::initActor\n");
    timers.init(this);
    dgram.init(this);
}

void Client::startActor(void)
{
    printf("Client::startActor\n");
    timers.startTimer<EvtEndTimer>(std::chrono::seconds(45));
    send_to_server();
}

void Client::pauseActor(void)
{
    printf("Client::pauseActor\n");
}

void Client::shutdownActor(void)
{
    printf("Client::shutdownActor\n");
    feal::EventBus::getInstance().stopBus();
    printf("Client shutdown complete\n");
}

void Client::send_to_server(void)
{
    feal::errenum se;
    serveraddr.family = feal::ipaddr::INET;
    serveraddr.port = 12001;
    strcpy(serveraddr.addr, "127.0.0.1");
    se = dgram.create_sock((feal::family_t) serveraddr.family);
    if (se != feal::FEAL_OK) printf("create sock: %d\n", se);
    timers.startTimer<EvtDelayTimer>(std::chrono::seconds(1));
}

void Client::send_something(void)
{
    char buf[30];
    int32_t bytes;
    memset(&buf, 0, sizeof(buf));
    sprintf(buf, "Client %d", n++);
    printf("Trying to send \"%s\" to %s:%d\n", buf, serveraddr.addr, serveraddr.port);
    feal::errenum se = dgram.send_to((void*) buf, MIN(strlen(buf) + 1, sizeof(buf)), &bytes, &serveraddr);
    if (se != feal::FEAL_OK) printf("Error sending \"Client n\": %d\n", se);
}

void Client::handleEvent(std::shared_ptr<EvtEndTimer> pevt)
{
    if (!pevt) return;
    printf("Client::EvtEndTimer Elapsed\n");
    timers.stopTimer<EvtDelayTimer>();
    dgram.close_and_reset();
    shutdown();
}

void Client::handleEvent(std::shared_ptr<EvtDelayTimer> pevt)
{
    if (!pevt) return;
    printf("Client::EvtDelayTimer\n");
    send_something();
    timers.startTimer<EvtDelayTimer>(std::chrono::seconds(2));
}

void Client::handleEvent(std::shared_ptr<feal::EvtDgramReadAvail> pevt)
{
    if (!pevt) return;
    printf("Client::EvtDgramReadAvail\n");
    char buf[30];
    int32_t bytes;
    memset(&buf, 0, sizeof(buf));
    feal::ipaddr recvaddr;
    feal::errenum se = dgram.recv_from((void*) buf, sizeof(buf), &bytes, &recvaddr);
    if (se != feal::FEAL_OK) printf("Error receiving: %d\n", se);
    else printf("Received %d bytes: \"%s\" from %s:%d\n", bytes, buf, recvaddr.addr, recvaddr.port);
}

void Client::handleEvent(std::shared_ptr<feal::EvtDgramWriteAvail> pevt)
{
    if (!pevt) return;
    printf("Client::EvtDgramWriteAvail\n");
    send_something();
}

void Client::handleEvent(std::shared_ptr<feal::EvtSockErr> pevt)
{
    if (!pevt) return;
    printf("Client::EvtSockErr\n");
    timers.stopTimer<EvtDelayTimer>();
    dgram.close_and_reset();
    send_to_server();
}

