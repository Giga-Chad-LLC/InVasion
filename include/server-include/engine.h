
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"
#include "network_packet.h"
#include "game-models/GameSession/game-session.h"
#include "move-request-model.pb.h"
#include "interactors/MoveInteractor/move-interactor.h"
namespace invasion::session {
    inline void makeEngine(SafeQueue<NetworkPacketRequest> &queueReceive, SafeQueue<NetworkPacketResponse> &queueSend,
                           game_models::GameSession &curGameSession) {
        std::thread([queueReceive = &queueReceive, queueSend = &queueSend, gameSession = &curGameSession]() {
            int imitatorTickController = 0;
            while (true) {
                NetworkPacketRequest request;
                imitatorTickController++;
                if (queueReceive->consume(request)) {
                    // work with this object
                    switch (request.getMessageType()) {
                        case RequestModel_t::PlayerActionRequestModel: {
//                            PlayerAction action;
                            // do engine-stuff here
                            request_models::MoveRequestModel action;
                            action.ParseFromArray(request.getStoredBytes(), request.bytesSize());
                            interactors::MoveInteractor interactor;
                            action.set_player_id(request.getPlayerId());
                            interactor.execute(action, *gameSession);

                            char *buffer = new char[request.bytesSize()];
                            std::memcpy(buffer, request.getStoredBytes(), request.bytesSize());

                            NetworkPacketResponse response(std::move(std::unique_ptr<char>(buffer)),
                                                           ResponseModel_t::PlayerActionResponseModel,
                                                           request.bytesSize());
                            queueSend->produce(std::move(response));
                            break;
                        }
                        default: {
                            std::cout << "Unknown message type: " << static_cast<int> (request.getMessageType())
                                      << std::endl;
                            break;
                        }
                    }
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
