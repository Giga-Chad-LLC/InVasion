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
	explicit Player(
		Vector2D shapeColliderSize,
		Vector2D hitboxColliderSize,
		Vector2D position,
		double mass,
		double maxSpeed,
		int playerId,
		PlayerTeamId teamId,
		int initialHitPoints,
		int initialAmmo,
		double damage
	);
	virtual ~Player() = default; // for polymorphism

	int getId() const;
	PlayerTeamId getTeamId() const;
	PlayerLifeState& getLifeState();
	PlayerGameSessionStats& getGameSessionStats();
	Weapon& getWeapon();
	void respawn(Vector2D position);


	inline static const double MAX_SPEED = 100.0;
	inline static const double MASS = 60.0;
	inline static const int INITIAL_AMMO = 180;
	inline static const double DAMAGE = 15.0;
	inline static const double INITIAL_HIT_POINTS = 100.0;
	inline static const Vector2D HITBOX_POSITION_OFFSET = Vector2D(0, -8);

private:
	inline static const Vector2D SHAPE_COLLIDER_SIZE = Vector2D(12, 6);
	inline static const Vector2D HITBOX_COLLIDER_SIZE = Vector2D(10, 14.5);
	
	const int m_id;
	const PlayerTeamId m_teamId;
	PlayerLifeState m_lifeState;
	PlayerGameSessionStats m_gameSessionStats;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_
