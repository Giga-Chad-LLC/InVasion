
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"

namespace inVasion::session {
    inline void makeEngine(SafeQueue<PlayerAction> &queueToEngine, SafeQueue<PlayerAction> &queueFromEngine) {
        std::thread([queueToEngine = &queueToEngine, queueFromEngine = &queueFromEngine]() {
            int imitatorTickController = 0;
            while (true) {
                PlayerAction removedElement;
                imitatorTickController++;
                if (imitatorTickController % 100000 == 0 && queueToEngine->consume(removedElement)) {

                    // work with this object;


                    queueFromEngine->produce(std::move(removedElement));
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
