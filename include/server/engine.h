#ifndef INVASION_SERVER_ENGINE_H
#define INVASION_SERVER_ENGINE_H

#include <thread>
#include <memory>
// server
#include "safe-queue.h"
#include "network-packet.h"
// game-models
#include "game-models/GameSession/game-session.h"
// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/UpdateGameStateInteractor/update-game-state-interactor.h"
// request-models
#include "move-request-model.pb.h"
#include "player.pb.h"
// response-models
#include "player-position-response-model.pb.h"
#include "players-positions-response-model.pb.h"



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
                            interactors::UpdateGameStateInteractor interactor;
                            response_models::PlayersPositionsResponseModel responseModel = interactor.execute(*gameSession);
                            // std::cout << gameSession->getPlayers().size() << std::endl;
                            // std::cout << "position: " << responseModel.position().x() << ' ' << responseModel.position().y() << std::endl;
                            // std::cout << "velocity: " << responseModel.velocity().x() << ' ' << responseModel.velocity().y() << std::endl;

                            // serialize
                            std::unique_ptr<char> buffer_ptr(new char[responseModel.ByteSizeLong()]);
                            responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());
                            
							NetworkPacketResponse response(std::move(buffer_ptr),
                                                           ResponseModel_t::PlayersPositionsResponseModel,
                                                           responseModel.ByteSizeLong());
                            // std::cout << responseModel.players().size() << std::endl;
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
