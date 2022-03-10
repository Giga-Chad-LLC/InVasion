
#ifndef INVASION_SERVER_RECEIVER_H
#define INVASION_SERVER_RECEIVER_H

#include <thread>
#include "user.h"
#include "safe-queue.h"
#include <player.pb.h>
#include "request-response.h"
namespace inVasion::session {
    class ReceiverFromUser {

    public:
        ReceiverFromUser(std::shared_ptr<User> cur_client, SafeQueue<RequestObject> *queueServerFromClients) {
            std::thread([client = std::move(cur_client), q = queueServerFromClients]() {
                while (client->channel) {
                    RequestObject request;
//                    std::uint32_t size;  // get the message data length in bytes
                    client->channel.read(reinterpret_cast<char *> (&request.sizeMessage), sizeof(request.sizeMessage));
                    client->channel.read(reinterpret_cast<char *> (&request.typeRequest), sizeof(request.typeRequest));
//                    char arr[request.sizeMessage];
                    client->channel.read(reinterpret_cast<char *> (&request.arrBytes), request.sizeMessage);
                    q->produce(std::move(request));

                }
                std::cout << "Client disconnected" << std::endl;
            }).detach();
        }
    };
}


#endif //INVASION_SERVER_RECEIVER_H
