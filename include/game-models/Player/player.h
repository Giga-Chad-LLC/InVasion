#ifndef PLAYER_H_
#define PLAYER_H_


#include "player-team-id-enum.h"
#include "player-life-state.h"
#include "player-game-session-stats.h"
#include "player-specialization-enum.h"
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
		PlayerSpecialization specialization,
		PlayerTeamId teamId,
		int initialHitPoints,
		int initialAmmo,
		int initialMagazine,
		int damage,
		bool hasAbility = false
	);
	virtual ~Player() = default; // for polymorphism

	int getId() const;
	PlayerSpecialization getSpecialization() const;
	PlayerTeamId getTeamId() const;
	PlayerLifeState& getLifeState();
	PlayerGameSessionStats& getGameSessionStats();
	Weapon& getWeapon();
	void respawn(Vector2D position);

	bool hasAbility() const;


	inline static const double MAX_SPEED = 95.0;
	inline static const double MASS = 60.0;
	inline static const int INITIAL_AMMO = 180;
	inline static const int INITIAL_MAGAZINE = 30;
	inline static const int DAMAGE = 15;
	inline static const int INITIAL_HIT_POINTS = 100;
	inline static const Vector2D HITBOX_POSITION_OFFSET = Vector2D(0, -8);

protected:
	inline static const Vector2D SHAPE_COLLIDER_SIZE = Vector2D(12, 6);
	inline static const Vector2D HITBOX_COLLIDER_SIZE = Vector2D(10, 14.5);

private:	
	const int m_id;
	const bool m_hasAbility;
	const PlayerSpecialization m_specialization;
	const PlayerTeamId m_teamId;
	PlayerLifeState m_lifeState;
	PlayerGameSessionStats m_gameSessionStats;
	Weapon m_weapon;
};


} // namespace invasion::game_models


#endif // PLAYER_H_
