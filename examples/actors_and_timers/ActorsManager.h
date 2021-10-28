// Please see the "examples/LICENSE" file for
// the copyright notice and license text.

#ifndef _FEAL_ACTORSMANAGER_H
#define _FEAL_ACTORSMANAGER_H

#include <memory>
#include "feal.h"

class EventTimerShutdown : public feal::Event
{
public:
EventTimerShutdown() = default;
EventTimerShutdown( const EventTimerShutdown & ) = default;
EventTimerShutdown& operator= ( const EventTimerShutdown & ) = default;
~EventTimerShutdown() = default;
feal::EventId_t getId(void);
};

class ActorsManager : public feal::Actor
{

public:

ActorsManager() = default;
~ActorsManager() = default;


virtual void initActor(void);
virtual void startActor(void);
virtual void pauseActor(void);
virtual void shutdownActor(void);

void handleEvent(std::shared_ptr<EventTimerShutdown> pevt);

private:
std::vector<std::shared_ptr<feal::Actor>> actors;
feal::Timers<ActorsManager> timers;

};


#endif // _FEAL_ACTORSMANAGER_H
