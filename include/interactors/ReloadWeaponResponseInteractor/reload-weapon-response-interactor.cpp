#include <memory>
#include <cassert>

#include "reload-weapon-response-interactor.h"

// game-models
#include "game-models/GameSession/game-session.h"
#include "game-models/Player/player.h"
#include "game-models/Weapon/weapon.h"
#include "game-models/Vector2D/vector2d.h"
// request-models
#include "reload-weapon-request-model.pb.h"
// response-models
#include "weapon-state-response-model.pb.h"
// util-models
#include "vector2d.pb.h"


namespace invasion::interactors {
using namespace game_models;
using namespace request_models;
using namespace response_models;
	

WeaponStateResponseModel ReloadWeaponResponseInteractor::execute(
	const ReloadWeaponRequestModel& req, std::shared_ptr<GameSession> session) const {
	const int playerId = req.player_id();
	std::shared_ptr<Player> player = session->getPlayer(playerId);

	Weapon& weapon = player->getWeapon();
	
	// sleeps for Weapon::RELOAD_DURATION_MS
	weapon.reload();

	const int leftMagazine = weapon.getLeftMagazine();
	const int leftAmmo = weapon.getLeftAmmo();
	const bool isReloadingRequired = false;
	const bool isReloading = weapon.isReloading(); // must be false

	assert(!isReloading);

	WeaponStateResponseModel response;
	
	response.set_player_id(playerId);
	response.set_left_magazine(leftMagazine);
	response.set_left_ammo(leftAmmo);
	
	response.set_is_reloading_required(isReloadingRequired);
	response.set_is_reloading(isReloading);


	const Vector2D weaponDirection = weapon.getDirection();

	response.mutable_weapon_direction()->set_x(weaponDirection.getX());
	response.mutable_weapon_direction()->set_y(weaponDirection.getY());

	return response;
}

} // namespace invasion::interactors
