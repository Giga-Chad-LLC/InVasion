#ifndef INITIAL_STATE_RESPONSE_INTERACTOR_H
#define INITIAL_STATE_RESPONSE_INTERACTOR_H

#include "../../../cmake-build-debug/initial-state-response-schema.pb.h"
#include "game-models/GameSession/game-session.h"

namespace invasion::interactors {
    struct InitialStateResponseInteractor {
        static game_schemes::InitialStateResponseSchema execute(game_models::GameSession &session, int playerId);
    };
}

#endif //INITIAL_STATE_RESPONSE_INTERACTOR_H
