//
// Copyright (c) 2022 ruben2020 https://github.com/ruben2020
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
 
#ifndef _SERVERUND_H
#define _SERVERUND_H

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

class EvtRetryTimer : public feal::Event
{
public:
EvtRetryTimer() = default;
EvtRetryTimer( const EvtRetryTimer & ) = default;
EvtRetryTimer& operator= ( const EvtRetryTimer & ) = default;
~EvtRetryTimer() = default;
feal::EventId_t getId(void);
};

class EvtDgramReadAvail : public feal::EventComm
{
public:
EvtDgramReadAvail() = default;
EvtDgramReadAvail( const EvtDgramReadAvail & ) = default;
EvtDgramReadAvail& operator= ( const EvtDgramReadAvail & ) = default;
~EvtDgramReadAvail() = default;
feal::EventId_t getId(void);
};

class EvtDgramWriteAvail : public feal::EventComm
{
public:
EvtDgramWriteAvail() = default;
EvtDgramWriteAvail( const EvtDgramWriteAvail & ) = default;
EvtDgramWriteAvail& operator= ( const EvtDgramWriteAvail & ) = default;
~EvtDgramWriteAvail() = default;
feal::EventId_t getId(void);
};

class EvtSockErr : public feal::EventComm
{
public:
EvtSockErr() = default;
EvtSockErr( const EvtSockErr & ) = default;
EvtSockErr& operator= ( const EvtSockErr & ) = default;
~EvtSockErr() = default;
feal::EventId_t getId(void);
};


class Serverund : public feal::Actor
{

public:

Serverund() = default;
~Serverund() = default;

void initActor(void);
void startActor(void);
void pauseActor(void);
void shutdownActor(void);

void handleEvent(std::shared_ptr<EvtEndTimer> pevt);
void handleEvent(std::shared_ptr<EvtRetryTimer> pevt);
void handleEvent(std::shared_ptr<EvtDgramReadAvail> pevt);
void handleEvent(std::shared_ptr<EvtDgramWriteAvail> pevt);
void handleEvent(std::shared_ptr<EvtSockErr> pevt);

protected:

feal::Timers<Serverund> timers;
feal::Datagram<Serverund> dgram;
virtual void start_listening(void);

private:


};

#endif // _SERVERUND_H
