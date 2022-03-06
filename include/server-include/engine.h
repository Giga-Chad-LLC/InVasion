
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"

namespace inVasion::session {
    inline void makeEngine(SafeQueue<PlayerAction> &queueReceive, SafeQueue<PlayerAction> &queueSend) {
        std::thread([queueReceive = &queueReceive, queueSend = &queueSend]() {
            while (true) {
                PlayerAction removedElement;
                if (queueReceive->consumeSync(removedElement)) {

                    // work with this object;
                //TODO: i catch bug, when i do a+s || a+w, player dont stop

                    queueSend->produce(std::move(removedElement));
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
