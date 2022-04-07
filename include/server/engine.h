
#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
#include "safe-queue.h"
#include "player.pb.h"
#include "network-packet.h"
#include "game-models/GameSession/game-session.h"
#include "move-request-model.pb.h"
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/UpdateGameStateInteractor/update-game-state-interactor.h"

namespace invasion::session {
    inline void manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue,
                              game_models::GameSession &gameSession) {
        std::thread([requestQueue = &requestQueue, responseQueue = &responseQueue, gameSession = &gameSession]() {
            while (true) {
                NetworkPacketRequest request;
                if (requestQueue->consumeSync(request)) {
                    // work with this object
                    switch (request.getMessageType()) {
                        case RequestModel_t::UpdateGameStateRequestModel: {
                            interactors::UpdateGameStateInteractor gameUpdateInteractor;
                            response_models::PlayerPositionResponseModel playerPositionResponse = gameUpdateInteractor.execute(*gameSession);

                            // std::cout << "position: " << playerPositionResponse.position().x() << ' ' << playerPositionResponse.position().y() << std::endl;
                            // std::cout << "velocity: " << playerPositionResponse.velocity().x() << ' ' << playerPositionResponse.velocity().y() << std::endl;

                            // serialize
                            std::unique_ptr<char> buffer_ptr(new char[playerPositionResponse.ByteSizeLong()]);
                            playerPositionResponse.SerializeToArray(buffer_ptr.get(), playerPositionResponse.ByteSizeLong());
                            NetworkPacketResponse response(std::move(buffer_ptr),
                                                           ResponseModel_t::PlayerPositionResponseModel,
                                                           playerPositionResponse.ByteSizeLong());

                            responseQueue->produce(std::move(response));
                            break;
                        }
                        case RequestModel_t::MoveRequestModel: {
                            request_models::MoveRequestModel move;
                            move.ParseFromArray(request.getStoredBytes(), request.bytesSize());
                            interactors::MoveInteractor interactor;
                            interactor.execute(move, *gameSession);
                            break;
                        }
                        case RequestModel_t::PlayerActionRequestModel: { //  leave it for now, then we will delete this request/response-model
                            PlayerAction action; // buttons
                            action.ParseFromArray(request.getStoredBytes(), request.bytesSize());

                            std::cout << "Action button: " << action.key_pressed() << std::endl;
                            std::unique_ptr<char> bytes_ptr = request.getPureBytes();
                            NetworkPacketResponse response(std::move(bytes_ptr), ResponseModel_t::PlayerActionResponseModel, request.bytesSize());
                            
                            responseQueue->produce(std::move(response));
                            break;
                        }
                        // case
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
