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
 
#ifndef _CLIENT_H
#define _CLIENT_H

#include "feal.h"

class EvtEndTimer : public feal::Event
{
public:
EvtEndTimer() = default;
EvtEndTimer( const EvtEndTimer & ) = default;
EvtEndTimer& operator= ( const EvtEndTimer & ) = default;
~EvtEndTimer() = default;
feal::EventId_t getId(void);
};

class EvtDelayTimer : public feal::Event
{
public:
EvtDelayTimer() = default;
EvtDelayTimer( const EvtDelayTimer & ) = default;
EvtDelayTimer& operator= ( const EvtDelayTimer & ) = default;
~EvtDelayTimer() = default;
feal::EventId_t getId(void);
};

class Client : public feal::Actor
{

public:

Client() = default;
~Client() = default;

void initActor(void);
void startActor(void);
void pauseActor(void);
void shutdownActor(void);

void handleEvent(std::shared_ptr<EvtEndTimer> pevt);
void handleEvent(std::shared_ptr<EvtDelayTimer> pevt);
void handleEvent(std::shared_ptr<feal::EvtDgramReadAvail> pevt);
void handleEvent(std::shared_ptr<feal::EvtDgramWriteAvail> pevt);
void handleEvent(std::shared_ptr<feal::EvtSockErr> pevt);

protected:

feal::Timers<Client> timers;
feal::Datagram<Client> dgram;
feal::ipaddr serveraddr;
virtual void send_to_server(void);

private:
int n=0;

void send_something(void);

};

#endif // _CLIENT_H

