#include <optional>
#include <utility>

#include "engine.h"
// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/PlayersPositionsResponseInteractor/players-positions-response-interactor.h"
#include "interactors/ShootInteractor/shoot-interactor.h"
#include "interactors/BulletsPositionsResponseInteractor/bullets-positions-response-interactor.h"
#include "interactors/DamagedPlayersResponseInteractor/damaged-players-response-interactor.h"
// request-models
#include "move-request-model.pb.h"
#include "shoot-request-model.pb.h"
#include "player.pb.h"
// response-models
#include "player-position-response-model.pb.h"
#include "game-state-response-model.pb.h"


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


void RequestQueueManager::manageRequestQueue(SafeQueue<std::shared_ptr<NetworkPacketRequest>> &requestQueue, SafeQueue<std::shared_ptr<NetworkPacketResponse>> &responseQueue, game_models::GameSession &gameSession, const std::vector <std::shared_ptr<Client>> &connectedClients) {
    std::thread([requestQueue = &requestQueue, responseQueue = &responseQueue, gameSession = &gameSession, connectedClients = &connectedClients]() {
        while (true) {
            std::shared_ptr<NetworkPacketRequest> request;
            if (requestQueue->consumeSync(request)) {
                // work with this object
                switch (request->getMessageType()) {
                    case RequestModel_t::UpdateGameStateRequestModel: {
						gameSession->updateGameState();

						{
							interactors::DamagedPlayersResponseInteractor interactor;
							std::optional<response_models::DamagedPlayersResponseModel> optionalModel = interactor.execute(*gameSession);

							if(optionalModel.has_value()) {
								response_models::DamagedPlayersResponseModel responseModel = *std::move(optionalModel);
								
								// serialize
								std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
								responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());
								
								auto response = std::make_shared<NetworkPacketResponse> (std::move(buffer_ptr),
																ResponseModel_t::DamagedPlayersResponseModel,
																responseModel.ByteSizeLong());
								
								responseQueue->produce(std::move(response));
							}
						}

						// sending players & bullets positions
						{
							// TODO: GameStateResponseModel rename in PositionsResponseModel
							response_models::GameStateResponseModel responseModel;

							// players positions 
							{
								interactors::PlayersPositionsResponseInteractor interactor;
								interactor.execute(responseModel, *gameSession);
							}

							// bullets positions
							{
								interactors::BulletsPositionsResponseInteractor interactor;
								interactor.execute(responseModel, *gameSession);
							}

							// serialize
							std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
							responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());
							
							auto response = std::make_shared<NetworkPacketResponse> (std::move(buffer_ptr),
															ResponseModel_t::GameStateResponseModel,
															responseModel.ByteSizeLong());
							
							responseQueue->produce(std::move(response));
						}

                        break;
                    }
                    case RequestModel_t::MoveRequestModel: {
                        request_models::MoveRequestModel move;
                        move.ParseFromArray(request->getStoredBytes(), request->bytesSize());
                        interactors::MoveInteractor interactor;
                        interactor.execute(move, *gameSession);
                        break;
                    }
                    case RequestModel_t::ShootRequestModel: {
                        request_models::ShootRequestModel shootAction;
                        shootAction.ParseFromArray(request->getStoredBytes(), request->bytesSize());

                        interactors::ShootInteractor interactor;
                        response_models::ShootingStateResponse responseModel = interactor.execute(shootAction, *gameSession);
                        
                        // serialize
                        std::unique_ptr<char[]> buffer_ptr(new char[responseModel.ByteSizeLong()]);
                        responseModel.SerializeToArray(buffer_ptr.get(), responseModel.ByteSizeLong());

                        auto response = std::make_shared<NetworkPacketResponse> (std::move(buffer_ptr),
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
                        std::cout << "Unknown message type: " << static_cast<int> (request->getMessageType())
                                    << std::endl;
                        break;
                    }
                }
            }
        }
    }).detach();
}

}