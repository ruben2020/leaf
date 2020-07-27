#include "feal.h"

void feal::initAll(feal::actor_vec_t& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        (**it).init();
    }
}

void feal::startAll(feal::actor_vec_t& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        (**it).start();
    }
}

void feal::pauseAll(feal::actor_vec_t& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        (**it).pause();
    }
}

void feal::shutdownAll(feal::actor_vec_t& vec)
{
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        (**it).shutdown();
    }
}

void feal::receiveEventAll(feal::actor_vec_t& vec, std::shared_ptr<feal::Event> pevt)
{
    if (!pevt) return;
    for (auto it = vec.begin(); it != vec.end(); ++it)
    {
        (**it).receiveEvent(pevt);
    }
}
