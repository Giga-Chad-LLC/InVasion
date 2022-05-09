#include <iostream>
#include "game-events-dispatcher.h"


namespace invasion::server {
void GameEventsDispatcher::start() {
    std::cout << "Game events dispatcher started" << std::endl;
}
void GameEventsDispatcher::stop() {
    std::cout << "Game events dispatcher stopped" << std::endl;
}
}
