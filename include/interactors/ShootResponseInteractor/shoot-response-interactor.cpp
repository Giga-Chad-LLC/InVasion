#include <utility>
#include <memory>

#include "shoot-response-interactor.h"
// response-models
#include "weapon-state-response-model.pb.h"
// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Weapon/weapon.h"



namespace invasion::interactors {
using namespace invasion::game_models;
using namespace response_models;
using namespace request_models;
	
WeaponStateResponseModel ShootResponseInteractor::execute(const ShootRequestModel& req, GameSession& session) const {
	std::shared_ptr<Player> player_ptr = session.getPlayer(req.player_id());
	Weapon& weapon = player_ptr->getWeapon();
	
	game_models::Vector2D direction(req.weapon_direction().x(), req.weapon_direction().y());
	weapon.setDirection(std::move(direction));
	
	WeaponStateResponseModel response;
	
	response.set_player_id(player_ptr->getId());

	// default values for reloading related state
	response.set_is_reloading_required(false);
	response.set_is_reloading(false);
	response.mutable_weapon_direction()->set_x(direction.getX());
	response.mutable_weapon_direction()->set_y(direction.getY());

	// if player is alive and active
	const bool dead = player_ptr->getLifeState().isInDeadState();
	const bool active = player_ptr->getLifeState().isInActiveState();

	if(!dead && active) {
		if(weapon.isAbleToShoot()) {
			const game_models::Vector2D position = player_ptr->getPosition();
			const int bulletId = session.createIdForNewBullet();

			std::shared_ptr<game_models::Bullet> bullet = weapon.shoot(position, bulletId);
			// adding bullet in session storage
			session.addBullet(bullet);
		}
		else if(weapon.isReloading()) {
			response.set_is_reloading(true);
		}
		else {
			// magazine is empty and weapon is not in reloading state
			response.set_is_reloading_required(true);
		}
	}

	response.set_left_magazine(weapon.getLeftMagazine());
	response.set_left_ammo(weapon.getLeftAmmo());

	return response;
}

} // namespace invasion::interactors
