#include <utility>
#include <memory>

#include "shoot-interactor.h"
// response-models
#include "shooting-state-response-model.pb.h"
// game-models
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player.h"
#include "game-models/Bullet/bullet.h"
#include "game-models/Weapon/weapon.h"



namespace invasion::interactors {
	
response_models::ShootingStateResponse ShootInteractor::execute(
	const request_models::ShootRequestModel& req, 
	game_models::GameSession& session
) const {
	game_models::Player& player = session.getPlayer(req.player_id());
	game_models::Weapon& weapon = player.getWeapon();
	
	game_models::Vector2D direction(req.weapon_direction().x(), req.weapon_direction().y());
	weapon.setDirection(std::move(direction));
	
	response_models::ShootingStateResponse response;
	
	response.set_player_id(player.getId());
	// default values for reloading related state
	response.set_is_reloading_required(false);
	response.set_is_reloading(false);
	response.mutable_weapon_direction()->set_x(direction.getX());
	response.mutable_weapon_direction()->set_y(direction.getY());

	if(weapon.isAbleToShoot()) {
		const game_models::Vector2D position = player.getPosition();
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

	response.set_left_magazine(weapon.getLeftMagazine());
	response.set_left_ammo(weapon.getLeftAmmo());

	return response;
}

} // namespace invasion::interactors
