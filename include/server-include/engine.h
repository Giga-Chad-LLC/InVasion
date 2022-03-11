
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
    inline void makeEngine(SafeQueue<NetworkPacketRequest> &queueServerFromClients, SafeQueue<NetworkPacketResponse> &queueClientsFromServer,
                           game_models::GameSession &curGameSession) {
        std::thread([queueServerFromClients = &queueServerFromClients, queueClientsFromServer = &queueClientsFromServer, gameSession = &curGameSession]() {
            int imitatorTickController = 0;
            while (true) {
                NetworkPacketRequest request;
                imitatorTickController++;
                if (queueServerFromClients->consume(request)) {
                    // work with this object
                    switch (request.getMessageType()) {
                        case RequestModel_t::MoveRequestModel: {
                            // PlayerAction action;
                            // do engine-stuff here
                            request_models::MoveRequestModel action;
                            action.ParseFromArray(request.getStoredBytes(), request.bytesSize());
                            interactors::MoveInteractor interactor;
                            // action.set_player_id(request.getPlayerId());
                            interactor.execute(action, *gameSession);


                            /*
                             *
                             * controller(100); // ms
                             *
                             * controller.start([]() {
                             * ...
                             * });
                             *
                             *
                             *
                             *
                             * */
                            //on each request from user we send answer from server
                            interactors::UpdateGameStateInteractor updaterGame;
                            auto responseFromInteractor = updaterGame.execute(*gameSession);
                            char *buffer = new char[request.bytesSize()];
                            // std::memcpy(buffer, request.getStoredBytes(), request.bytesSize());
                            responseFromInteractor.SerializeToArray(buffer, request.bytesSize());
                            NetworkPacketResponse response(std::move(std::unique_ptr<char>(buffer)),
                                                           ResponseModel_t::PlayerActionResponseModel,
                                                           request.bytesSize());
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
                            std::cout << "Length: " << request.bytesSize() << std::endl;
                            std::unique_ptr<char> start = request.getPureBytes();

                            // for (int i = 0; i < request.bytesSize(); i++) {
                            //     std::cout << static_cast<int> (start.get()[i]) << ' ';
                            // }
                            // std::cout << std::endl;
                            break;
                        }
                    }
                }
            }
        }).detach();
    }
}
#endif //INVASION_SERVER_ENGINE_H
