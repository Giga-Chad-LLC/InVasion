#include "player-life-state.h"

namespace invasion::game_models {
	

PlayerLifeState::PlayerLifeState(double initialHitPoints)
	: m_initialHitPoints(initialHitPoints),
	  m_hitPoints(initialHitPoints),
	  m_isDead(false),
	  m_killedBy(-1),
	  m_isDamagedOnLastUpdate(false),
	  m_damagedBy(-1),
	  m_isActive(true) {}


void PlayerLifeState::applyDamage(const double damage, const int playerId) {
	if(m_hitPoints <= damage) {
		m_hitPoints = 0.0;
		m_isDead = true;
		m_killedBy = playerId;
	}
	else {
		m_hitPoints -= damage;
		m_damagedBy = playerId;
	}
	m_isDamagedOnLastUpdate = true;
}


double PlayerLifeState::getHitPoints() const {
	return m_hitPoints;
}


bool PlayerLifeState::isInDeadState() const {
	return m_isDead;
}


bool PlayerLifeState::isInDamagedState() const {
	return m_isDamagedOnLastUpdate;
}

void PlayerLifeState::removeDamagedState() {
	m_isDamagedOnLastUpdate = false;
}


int PlayerLifeState::killedBy() const {
	return m_killedBy;
}

int PlayerLifeState::damagedBy() const {
	return m_damagedBy;
}


bool PlayerLifeState::isInActiveState() const {
	return m_isActive;
}


void PlayerLifeState::setActiveState(bool state) {
	m_isActive = state;
}


void PlayerLifeState::reset() {
	m_hitPoints = m_initialHitPoints;
	m_isDead = false;
	m_isDamagedOnLastUpdate = false;
	m_killedBy = m_damagedBy = -1;
	m_isActive = true;
}


} // namespace invasion::game_models
