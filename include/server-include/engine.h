
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"
#include "request-response.h"
namespace inVasion::session {
    inline void makeEngine(SafeQueue<RequestObject> &queueServerFromClients, SafeQueue<ResponseObject> &queueClientsFromServer) {
        std::thread([queueServerFromClients = &queueServerFromClients, queueClientsFromServer = &queueClientsFromServer]() {
            while (true) {
                RequestObject removedElement;
                if (queueServerFromClients->consumeSync(removedElement)) {

                    // work with this object;
                    ResponseObject response;
                    response.arrBytes = removedElement.arrBytes;
                    queueClientsFromServer->produce(std::move(response));
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
