#ifndef PLAYER_LIFE_STATE_H_
#define PLAYER_LIFE_STATE_H_


namespace invasion::game_models {
	
class PlayerLifeState {
public:
	explicit PlayerLifeState(int initialHitPoints);

	void applyDamage(int damage, int playerId);
	int getInitialHitPoints() const;
	int getHitPoints() const;
	bool isInDeadState() const;
	bool isInDamagedState() const;
	void removeDamagedState();

	void applyHealing(int healPoints);

	int killedBy() const;
	int damagedBy() const;

	bool isInActiveState() const;
	void setActiveState(bool state);

	void reset();

private:
	const int m_initialHitPoints;
	int m_hitPoints;
	bool m_isDead;
	int m_killedBy;
	bool m_isDamagedOnLastUpdate;
	int m_damagedBy;
	int m_isActive;
};

} // namespace invasion::game_models



#endif // PLAYER_LIFE_STATE_H_