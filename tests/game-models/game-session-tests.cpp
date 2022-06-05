#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>
#include <optional>

// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Player/player-team-id-enum.h"
#include "game-models/Player/player-specialization-enum.h"
#include "game-models/Weapon/weapon.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/GameSession/game-session.h"
#include "game-models/GameSession/game-session-stats.h"
#include "game-models/Weapon/weapon.h"
#include "game-models/Sentinel/sentinel.h"
// utils
#include "utils/TimeUtilities/time-utilities.h"
// interactors
#include "interactors/ChangePlayerSpecializationInteractor/change-player-specialization-interactor.h"
#include "interactors/ApplyAbilityInteractor/apply-ability-interactor.h"
#include "interactors/UseSupplyInteractor/use-supply-interactor.h"
#include "interactors/ReloadWeaponResponseInteractor/reload-weapon-response-interactor.h"
#include "interactors/ShootResponseInteractor/shoot-response-interactor.h"
// controllers
#include "controllers/FixedTimeoutCallbackInvoker/fixed-timeout-callback-invoker.h"
// request-models
#include "apply-ability-request-model.pb.h"
#include "use-supply-request-model.pb.h"
#include "reload-weapon-request-model.pb.h"
#include "shoot-request-model.pb.h"
// response-models
#include "player-specialization-response-model.pb.h"
#include "supply-response-model.pb.h"
#include "use-supply-response-model.pb.h"
#include "weapon-state-response-model.pb.h"
// util-models
#include "player-specialization.pb.h"
#include "supply-model.pb.h"
#include "supply-type.pb.h"

#include "doctest.h"


namespace doctest {
using namespace invasion::game_models;
using namespace invasion::controllers;
using namespace invasion::interactors;
using namespace invasion::utils;
using namespace request_models;
using namespace response_models;




TEST_CASE("Hit points retrieving") {
	GameSession session;
	const int id = session.createPlayerAndReturnId(PlayerSpecialization::Sentinel);
	std::shared_ptr<Player> player = session.getPlayer(id);

	const int hitPoints = player->getLifeState().getHitPoints();
	const int initHitPoints = player->getLifeState().getInitialHitPoints();

	CHECK(hitPoints == Sentinel::INITIAL_HIT_POINTS);
	CHECK(initHitPoints == Sentinel::INITIAL_HIT_POINTS);

	std::cout << "HP: " << hitPoints << std::endl;
	std::cout << "init HP: " << initHitPoints << std::endl;
	std::cout << "Sentinel HP: " << Sentinel::INITIAL_HIT_POINTS << std::endl;
}




TEST_CASE("Reloading with interactors using timeout invoker") {
	auto session = std::make_shared<GameSession>();

	const int id1 = session->createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	std::shared_ptr<Player> player = session->getPlayer(id1);

	Weapon& weapon = player->getWeapon();
	
	const int shots = 10;
	const int initialMagazine = weapon.getLeftMagazine();
	const int initialAmmo = weapon.getInitialAmmo();


	// shooting 
	{
		ShootResponseInteractor interactor;
		ShootRequestModel req;

		req.set_player_id(id1);
		const Vector2D direction = weapon.getDirection();
		req.mutable_weapon_direction()->set_x(direction.getX());
		req.mutable_weapon_direction()->set_y(direction.getY());

		for(int i = 0; i < shots; i++) {
			interactor.execute(req, *session);
		}
	}

	const int leftMagazine = weapon.getLeftMagazine();

	CHECK(leftMagazine + shots == initialMagazine);

	// reloading
	{
		ReloadWeaponResponseInteractor interactor;
		
		ReloadWeaponRequestModel req;
		req.set_player_id(id1);

		FixedTimeoutCallbackInvoker invoker;
		invoker.setTimeout(0, [&]() {
			long long start = TimeUtilities::getCurrentTime_ms();
			std::cout << "executing interactor..." << std::endl;
			
			interactor.execute(req, session);

			std::cout << "reloading has finished" << std::endl;

			long long finish = TimeUtilities::getCurrentTime_ms();
			std::cout << "reloading has taken: " << finish - start << "ms" << std::endl;
		});

		// to prevent invoker's dtor calling (request and session might be destroyed)
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << "finishing test..." << std::endl;
	}
}




TEST_CASE("Reloading with interactors") {
	auto session = std::make_shared<GameSession>();


	const int id1 = session->createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	std::shared_ptr<Player> player = session->getPlayer(id1);

	Weapon& weapon = player->getWeapon();
	
	const int shots = 10;
	const int initialMagazine = weapon.getLeftMagazine();
	const int initialAmmo = weapon.getInitialAmmo();


	// shooting 
	{
		ShootResponseInteractor interactor;
		ShootRequestModel req;

		req.set_player_id(id1);
		const Vector2D direction = weapon.getDirection();
		req.mutable_weapon_direction()->set_x(direction.getX());
		req.mutable_weapon_direction()->set_y(direction.getY());

		for(int i = 0; i < shots; i++) {
			interactor.execute(req, *session);
		}
	}

	const int leftMagazine = weapon.getLeftMagazine();

	CHECK(leftMagazine + shots == initialMagazine);

	// reloading
	{
		ReloadWeaponResponseInteractor interactor;
		ReloadWeaponRequestModel req;

		req.set_player_id(id1);
		interactor.execute(req, session);
	}

	const int currentLeftMagazine = weapon.getLeftMagazine();
	const int currentLeftAmmo = weapon.getLeftAmmo();

	CHECK(currentLeftMagazine == initialMagazine);
	CHECK(currentLeftAmmo + shots == initialAmmo);
	CHECK(leftMagazine + initialAmmo == currentLeftMagazine + currentLeftAmmo);
}






TEST_CASE("Supplies using") {
	GameSession session;
	const int id1 = session.createPlayerAndReturnId(PlayerSpecialization::Medic);
	const int id2 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);

	std::shared_ptr<Player> player1 = session.getPlayer(id1);
	std::shared_ptr<Player> player2 = session.getPlayer(id2);

	UseSupplyInteractor interactor;
	UseSupplyRequestModel req;

	{
		req.set_player_id(id2);

		player2->setPosition(Vector2D(100, 0));
		std::optional<UseSupplyResponseModel> opt = interactor.execute(req, session);

		CHECK(opt.has_value() == false);

		player2->setPosition(Vector2D(15, 0));
		interactor.execute(req, session);
	}

	{
		req.set_player_id(id2);
		const int HP = player2->getLifeState().getHitPoints();
		player2->getLifeState().applyDamage(20, id1);

		interactor.execute(req, session);
	}
}





TEST_CASE("Decrementing players in teams") {
	GameSession session;

	const int id1 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	const int id2 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);

	const GameSessionStats& stats = session.getGameStatistics();

	CHECK(stats.getFirstTeamPlayersCount() == 1);
	CHECK(stats.getSecondTeamPlayersCount() == 1);

	session.removePlayerById(id1);
	session.removePlayerById(id2);
	
	std::vector<int> ids;
	
	const int n = 100;
	for(int i = 0; i < n; i++) {
		const int id = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
		ids.push_back(id);
	}

	int firstTeamCount = stats.getFirstTeamPlayersCount();
	int secondTeamCount = stats.getSecondTeamPlayersCount();

	CHECK(2 * firstTeamCount == n);
	CHECK(2 * secondTeamCount == n);
	CHECK(secondTeamCount == firstTeamCount);

	for(int id : ids) {
		PlayerTeamId teamId = session.getPlayer(id)->getTeamId();
		session.removePlayerById(id);

		if(teamId == PlayerTeamId::FirstTeam) {
			CHECK(stats.getFirstTeamPlayersCount() + 1 == firstTeamCount);
			--firstTeamCount;
		}
		else if(teamId == PlayerTeamId::SecondTeam) {
			CHECK(stats.getSecondTeamPlayersCount() + 1 == secondTeamCount);
			--secondTeamCount;
		}
	}

	CHECK(stats.getFirstTeamPlayersCount() == 0);
	CHECK(stats.getSecondTeamPlayersCount() == 0);
}





TEST_CASE("Updating kills counts of teams") {
	using namespace std::chrono_literals;
	GameSession session;
	std::shared_ptr<Player> player1 = session.getPlayer(session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper));
	Weapon& weapon = player1->getWeapon();
	weapon.setDirection(Vector2D(0, 1));
	const Vector2D position = player1->getPosition();

	std::shared_ptr<Player> player2 = session.getPlayer(session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper));
	player2->setPosition(Vector2D(0, 15));

	for(int i = 0; i < 10; i++) {
		const int bulletId = session.createIdForNewBullet();		
		std::shared_ptr<Bullet> bullet = weapon.shoot(position, bulletId);

		session.addBullet(bullet);

		std::this_thread::sleep_for(30ms);
		session.updateGameState();
	}
	
	for(int i = 0; i < 10; i++) {
		std::this_thread::sleep_for(30ms);
		session.updateGameState();
	}

	// logging
	{
		std::vector<std::shared_ptr<Bullet>>& bullets = session.getBullets();
		for(const auto& bullet_ptr : bullets) {
			std::cout << "bullet id: " << bullet_ptr->getId() << " position: " << bullet_ptr->getPosition() << '\n';
		}
		std::cout << "player2 position: " << player2->getPosition() << std::endl;
		std::cout << "player2 life state: " << player2->getLifeState().isInDeadState() << std::endl;
		std::cout << "player2 HP: " << player2->getLifeState().getHitPoints() << std::endl;
	}
}




TEST_CASE("Deleting players from GameSession") {
	GameSession session;
	const int id1 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	const int id2 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	const int id3 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);
	const int id4 = session.createPlayerAndReturnId(PlayerSpecialization::Stormtrooper);

	std::vector<int> ids = {
		id1, id2, id3, id4
	};

	int prevSize = session.getPlayers().size();
	int deletedCount = 0;

	for(int id : ids) {
		CHECK(session.getPlayers().size() == prevSize - deletedCount);
		session.removePlayerById(id);
		deletedCount++;
		CHECK(session.getPlayers().size() == prevSize - deletedCount);
	}

	CHECK(session.getPlayers().size() == prevSize - deletedCount);
}


}
