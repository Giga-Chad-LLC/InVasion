#include <iostream>
#include "game-events-dispatcher.h"


namespace invasion::server {
void GameEventsDispatcher::start() {
    if (m_isActive.load()) {
        return;
    }

    m_isActive.store(true);
    std::cout << "Game events dispatcher started" << std::endl;
}
void GameEventsDispatcher::stop() {
    if (!m_isActive.load()) {
        return;
    }
    m_isActive.store(false);
    std::cout << "Game events dispatcher stopped" << std::endl;
}
}
