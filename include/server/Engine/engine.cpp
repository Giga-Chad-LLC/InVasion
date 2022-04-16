#include "engine.h"
// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/PlayersPositionsResponseInteractor/players-positions-response-interactor.h"
#include "interactors/ShootInteractor/shoot-interactor.h"
#include "interactors/BulletsPositionsResponseInteractor/bullets-positions-response-interactor.h"
// request-models
#include "move-request-model.pb.h"
#include "shoot-request-model.pb.h"
#include "player.pb.h"
// response-models
#include "player-position-response-model.pb.h"
#include "players-positions-response-model.pb.h"


namespace invasion::session { 
// finds client by its player id in a game session (returns `nullptr`, if not found)
std::shared_ptr<Client> RequestQueueManager::getConnectedClientByPlayerId(const std::vector <std::shared_ptr<Client>> &connectedClients, uint32_t playerId) {
    for (auto &client : connectedClients) {
        if (client->m_clientIdInGameSession == playerId) {
            return client;
        }
    }

    return nullptr;
}


void RequestQueueManager::manageRequestQueue(SafeQueue<NetworkPacketRequest> &requestQueue, SafeQueue<NetworkPacketResponse> &responseQueue, game_models::GameSession &gameSession, const std::vector <std::shared_ptr<Client>> &connectedClients) {
    std::thread([requestQueue = &requestQueue, responseQueue = &responseQueue, gameSession = &gameSession, connectedClients = &connectedClients]() {
        while (true) {
            NetworkPacketRequest request;
            if (requestQueue->consumeSync(request)) {
                // work with this object
                switch (request.getMessageType()) {
                    case RequestModel_t::UpdateGameStateRequestModel: {
						gameSession->updateGameState();
                        NetworkPacketResponse playersPositionsResponse;
                        NetworkPacketResponse bulletsPositionsResponse;

                        // send players positions 
                        {
                            interactors::PlayersPositionsResponseInteractor interactor;
                            response_models::PlayersPositionsResponseModel responseModel = interactor.execute(*gameSession);


                            // serialize
                            std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
                            responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());
                            
                            playersPositionsResponse = NetworkPacketResponse(std::move(buffer_ptr),
                                                            ResponseModel_t::PlayersPositionsResponseModel,
                                                            responseModel.ByteSizeLong());
                            
                            // responseQueue->produce(std::move(response));
                        }
                        // send bullets positions
                        {
                            interactors::BulletsPositionsResponseInteractor interactor;
                            response_models::BulletsPositionsResponseModel responseModel = interactor.execute(*gameSession);
                            // std::cout << "Bullets: " << responseModel.bullets().size() << "\n";
                            // serialize
                            std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
                            responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());

                            bulletsPositionsResponse = NetworkPacketResponse(std::move(buffer_ptr), ResponseModel_t::BulletsPositionsResponseModel, responseModel.ByteSizeLong());
                            // responseQueue->produce(std::move(response));
                        }

                        responseQueue->produceSome(std::move(playersPositionsResponse), std::move(bulletsPositionsResponse));

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
                        std::unique_ptr<char[]> bytes_ptr = request.getPureBytes();
                        NetworkPacketResponse response(std::move(bytes_ptr), ResponseModel_t::PlayerActionResponseModel, request.bytesSize());
                        
                        responseQueue->produce(std::move(response));
                        break;
                    }
                    case RequestModel_t::ShootRequestModel: {
                        request_models::ShootRequestModel shootAction;
                        shootAction.ParseFromArray(request.getStoredBytes(), request.bytesSize());

                        interactors::ShootInteractor interactor;
                        response_models::ShootingStateResponse responseModel = interactor.execute(shootAction, *gameSession);
                        
                        // serialize
                        std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
                        responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());

                        NetworkPacketResponse response(std::move(buffer_ptr),
                                                        ResponseModel_t::ShootingStateResponseModel,
                                                        responseModel.ByteSizeLong());
                        
                        // responseQueue->produce(std::move(response));
                        std::shared_ptr<Client> client = RequestQueueManager::getConnectedClientByPlayerId(*connectedClients, responseModel.player_id());
                        if (client) {
                            client->m_clientResponseQueue.produce(std::move(response));
                        }

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