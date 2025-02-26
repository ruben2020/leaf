//
// Copyright (c) 2022-2025 ruben2020 https://github.com/ruben2020
// SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
//
 
#ifndef _CLIENTHANDLER_H
#define _CLIENTHANDLER_H

#include "feal.h"

class Server;


class EvtClientDisconnected : public feal::Event
{
friend class ClientHandler;

public:
EvtClientDisconnected() = default;
EvtClientDisconnected( const EvtClientDisconnected & ) = default;
EvtClientDisconnected& operator= ( const EvtClientDisconnected & ) = default;
~EvtClientDisconnected() = default;

feal::handle_t fd = -1;

private:

};

FEAL_EVENT_DEFAULT_DECLARE(EvtDataReadAvail, EventComm)
FEAL_EVENT_DEFAULT_DECLARE(EvtDataWriteAvail, EventComm)
FEAL_EVENT_DEFAULT_DECLARE(EvtClientShutdown, EventComm)


class ClientHandler : public feal::Actor
{

public:

ClientHandler()=default;
~ClientHandler() = default;
void setParam(feal::Stream<Server>* p, feal::handle_t fd, char *s);

void initActor(void);
void startActor(void);
void pauseActor(void);
void shutdownActor(void);

void handleEvent(std::shared_ptr<EvtDataReadAvail> pevt);
void handleEvent(std::shared_ptr<EvtDataWriteAvail> pevt);
void handleEvent(std::shared_ptr<EvtClientShutdown> pevt);

private:
feal::Stream<Server>* stream = nullptr;
feal::handle_t sockfd = -1;
std::string addrstr;

};


#endif // _CLIENTHANDLER_H

