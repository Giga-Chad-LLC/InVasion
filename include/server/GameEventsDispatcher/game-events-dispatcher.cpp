#include <iostream>
#include <utility>
#include "game-events-dispatcher.h"
#include "server/Session/session.h"

// interactors
#include "interactors/MoveInteractor/move-interactor.h"
#include "interactors/PlayersPositionsResponseInteractor/players-positions-response-interactor.h"
#include "interactors/ShootInteractor/shoot-interactor.h"
#include "interactors/BulletsPositionsResponseInteractor/bullets-positions-response-interactor.h"
#include "interactors/DamagedPlayersResponseInteractor/damaged-players-response-interactor.h"
#include "interactors/KilledPlayersResponseInteractor/killed-players-response-interactor.h"
#include "interactors/RespawnPlayerInteractor/respawn-player-interactor.h"
// request-models
#include "move-request-model.pb.h"
#include "shoot-request-model.pb.h"
#include "respawn-player-request-model.pb.h"
// response-models
#include "player-position-response-model.pb.h"
#include "game-state-response-model.pb.h"
#include "respawn-player-response-model.pb.h"


namespace invasion::server {
void GameEventsDispatcher::start(
    std::shared_ptr <Session> session,
    std::shared_ptr <game_models::GameSession> gameSession,
    std::shared_ptr <SafeQueue <std::shared_ptr <NetworkPacketRequest>>> requestQueue
) {
    if (m_isActive.load()) {
        return;
    }

    m_isActive.store(true);
    std::cout << "Game events dispatcher started" << std::endl;

    m_thread = std::move(std::thread([this, session, gameSession, requestQueue]() mutable {
        while (m_isActive.load()) {
            std::shared_ptr <NetworkPacketRequest> request;

            if (requestQueue->consume(request)) {
                dispatchEvent(session, gameSession, request);
            }
        }

        std::cout << "Events dispatcher thread exits" << std::endl;
    }));
}

void GameEventsDispatcher::stop() {
    if (!m_isActive.load()) {
        return;
    }
    m_isActive.store(false);
    m_thread.join();
    std::cout << "Game events dispatcher stopped" << std::endl;
}


void GameEventsDispatcher::dispatchEvent(
    std::shared_ptr <Session> session,
    std::shared_ptr <game_models::GameSession> gameSession,
    std::shared_ptr <NetworkPacketRequest> request
) {
    switch (request->getMessageType()) {
        case RequestModel_t::UpdateGameStateRequestModel: {
            gameSession->updateGameState();
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

            // damaged players
            {
                interactors::DamagedPlayersResponseInteractor interactor;
                interactor.execute(responseModel, *gameSession);
            }

            // killed players
            {
                interactors::KilledPlayersResponseInteractor interactor;
                interactor.execute(responseModel, *gameSession);
            }

            // serialize            

            auto response = std::make_shared<NetworkPacketResponse> (
                NetworkPacket::serialize(responseModel), // instead of std::move(std::unique_ptr)
                ResponseModel_t::GameStateResponseModel,
                responseModel.ByteSizeLong()
            );
            
            // responseQueue->produce(std::move(response));
            session->putDataToAllClients(response);

            break;
        }
        default: {
            std::cout << "Request type: " << static_cast <uint32_t> (request->getMessageType()) << "\n";
            break;
        }
    }
}

}
