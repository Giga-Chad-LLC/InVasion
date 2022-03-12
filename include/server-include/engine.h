
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
#include "interactors/UpdateGameStateInteractor/update-game-state-interactor.h"

namespace invasion::session {
    inline void HandlerQueues(SafeQueue<NetworkPacketRequest> &queueServerFromClients, SafeQueue<NetworkPacketResponse> &queueClientsFromServer,
                              game_models::GameSession &curGameSession) {
        std::thread([queueServerFromClients = &queueServerFromClients, queueClientsFromServer = &queueClientsFromServer, gameSession = &curGameSession]() {
            while (true) {
                NetworkPacketRequest request;
                if (queueServerFromClients->consumeSync(request)) {
                    // work with this object
                    switch (request.getMessageType()) {
                        case RequestModel_t::MoveRequestModel: {
                            // do engine-stuff here
                            request_models::MoveRequestModel move;
                            move.ParseFromArray(request.getStoredBytes(), request.bytesSize());
                            interactors::MoveInteractor interactor;
                            interactor.execute(move, *gameSession);

                            //on each request from user we send answer from server
                            interactors::UpdateGameStateInteractor gameUpdateInteractor;
                            response_models::PlayerPositionResponseModel playerPositionResponse = gameUpdateInteractor.execute(*gameSession);
                            std::unique_ptr<char> buffer_ptr(new char[playerPositionResponse.ByteSizeLong()]);
                            
                            std::cout << "position: " << playerPositionResponse.position().x() << ' ' << playerPositionResponse.position().y() << std::endl;
                            std::cout << "velocity: " << playerPositionResponse.velocity().x() << ' ' << playerPositionResponse.velocity().y() << std::endl;

                            playerPositionResponse.SerializeToArray(buffer_ptr.get(), playerPositionResponse.ByteSizeLong());
                            NetworkPacketResponse response(std::move(buffer_ptr),
                                                           ResponseModel_t::PlayerPositionResponseModel,
                                                           playerPositionResponse.ByteSizeLong());
                            queueClientsFromServer->produce(std::move(response));
                            break;
                        }
                        case RequestModel_t::PlayerActionRequestModel: { //  leave it for now, then we will delete this request/response-model
                            PlayerAction action; // buttons
                            action.ParseFromArray(request.getStoredBytes(), request.bytesSize());

                            std::cout << "Action button: " << action.key_pressed() << std::endl;
                            std::unique_ptr<char> bytes_ptr = request.getPureBytes();
                            NetworkPacketResponse response(std::move(bytes_ptr), ResponseModel_t::PlayerActionResponseModel, request.bytesSize());
                            
                            queueClientsFromServer->produce(std::move(response));
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
