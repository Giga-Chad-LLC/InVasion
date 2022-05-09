#ifndef GAME_EVENTS_DISPATCHER_H_
#define GAME_EVENTS_DISPATCHER_H_

#include "atomic"

namespace invasion::server {
class GameEventsDispatcher {
public:
    void start();
    void stop();
private:
    std::atomic_bool m_isActive = false;
};
}

#endif