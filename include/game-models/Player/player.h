#ifndef PLAYER_H_
#define PLAYER_H_


#include "player-team-id-enum.h"
#include "player-life-state.h"
#include "player-game-session-stats.h"
// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Weapon/weapon.h"


namespace invasion::game_models {
	

class Player : public KinematicObject {
public:
	explicit Player(Vector2D initialPosition, int playerId, PlayerTeamId teamId);

	int getId() const;
	PlayerTeamId getTeamId() const;
	PlayerLifeState& getLifeState();
	PlayerGameSessionStats& getGameSessionStats();
	Weapon& getWeapon();
	void respawn(Vector2D position);


	static const double MAX_SPEED;
	static const double MASS;
	static const int INITIAL_AMMO;
	static const double DAMAGE;
	static const double INITIAL_HIT_POINTS;
	static const Vector2D HITBOX_POSITION_OFFSET;

private:
	static const Vector2D SHAPE_COLLIDER_SIZE;
	static const Vector2D HITBOX_COLLIDER_SIZE;
	const int m_id;
	const PlayerTeamId m_teamId;
	PlayerLifeState m_lifeState;
	PlayerGameSessionStats m_gameSessionStats;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_
