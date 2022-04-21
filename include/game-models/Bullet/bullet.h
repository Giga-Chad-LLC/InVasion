#ifndef BULLET_H_
#define BULLET_H_

// game-models
#include "game-models/KinematicObject/kinematic-object.h"
#include "game-models/Vector2D/vector2d.h"
#include "game-models/Player/player-team-id-enum.h"


namespace invasion::game_models {


struct Bullet : KinematicObject {
	Bullet(Vector2D initialPos, int bulletId, int playerId, PlayerTeamId teamId, double damage);

	int getId() const;
	int getPlayerId() const;
	PlayerTeamId getPlayerTeamId() const;
	double getDamage() const;
	bool isInCrushedState() const;
	void setCrushedState(bool state);

	static const double MAX_SPEED;
	static const double MASS;

private:
	static const Vector2D COLLIDER_SIZE;

	const int m_id;
	const int m_playerId;
	const PlayerTeamId m_playerTeamId;
	const double m_damage;
	bool m_isCrushed;
};


} // namespace invasion::game_models



#endif // BULLET_H_