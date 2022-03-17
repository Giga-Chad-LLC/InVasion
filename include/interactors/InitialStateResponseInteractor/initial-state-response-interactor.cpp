#include "initial-state-response-interactor.h"

game_schemes::InitialStateResponseSchema
invasion::interactors::InitialStateResponseInteractor::execute(game_models::GameSession &session, int playerId) {
    game_schemes::InitialStateResponseSchema responseForPlayer;

    // collecting information about opponents and allies, setting the appropriate values in the fields

    responseForPlayer.set_playerid(playerId);
    return responseForPlayer;
}
