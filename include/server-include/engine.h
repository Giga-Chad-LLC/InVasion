
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"

namespace inVasion::session {
    inline void makeEngine(SafeQueue<PlayerAction> &queueServerFromClients, SafeQueue<PlayerAction> &queueClientsFromServer) {
        std::thread([queueServerFromClients = &queueServerFromClients, queueClientsFromServer = &queueClientsFromServer]() {
            while (true) {
                PlayerAction removedElement;
                if (queueServerFromClients->consumeSync(removedElement)) {

                    // work with this object;


                    queueClientsFromServer->produce(std::move(removedElement));
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
