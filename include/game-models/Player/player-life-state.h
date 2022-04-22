#ifndef PLAYER_LIFE_STATE_H_
#define PLAYER_LIFE_STATE_H_


namespace invasion::game_models {
	
struct PlayerLifeState {
	PlayerLifeState(double initialHitPoints);

	void applyDamage(double damage, int playerId);
	double getHitPoints() const;
	bool isInDeadState() const;
	bool isInDamagedState() const;
	void removeDamagedState();
	void reset();

private:
	const double m_initialHitPoints;
	double m_hitPoints;
	bool m_isDead;
	int m_killedBy;
	bool m_isDamagedOnLastUpdate;
	int m_damagedBy;
};

} // namespace invasion::game_models



#endif // PLAYER_LIFE_STATE_H_