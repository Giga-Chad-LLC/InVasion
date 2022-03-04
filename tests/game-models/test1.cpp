#include <iostream>
#include <vector>

#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/GameWorldManager/game-world-manager.h"

#include "interactors/MoveInteractor/move-interactor.h"

#include "doctest.h"


namespace doctest {

TEST_CASE("creating physics") {
	using namespace invasion::game_models;
	using namespace invasion::interactors;
	using namespace std;

	int totalSimulationTime = 40;
    int currentTime = 0;
    int dt = 1;

	MoveInteractor interactor;
	GameSession session;
	Player& player = session.getPlayer(session.addPlayer());

	// creating request
	MoveRequestModel req;
	req.set_player_id(player.getId());
	req.set_current_event(MoveRequestModel::StartMovingUp);

	// start moving
	interactor.execute(req, session);

	while(currentTime < totalSimulationTime) {
		cout << "time: " << currentTime << " velocity: " << player.getVelocity() << endl;

		// stop event
		if(currentTime == 20) {
			std::cout << "start stopping..." << std::endl;
			req.set_current_event(MoveRequestModel::StopMovingUp);
			interactor.execute(req, session);
		}

		session.updateGameState();
		currentTime += dt;
	}

	cout << "time: " << currentTime << " velocity: " << player.getVelocity() << endl;
}


} // namespace doctest
