#ifndef INTERACTOR_H_
#define INTERACTOR_H_

#include "game-models/Player/player.h"

namespace invasion::interactors {
	

struct MoveRequestModel {
	enum class MoveEvent;

	MoveRequestModel(MoveEvent initial_state) 
		: m_event(initial_state) {}

	void setEvent(MoveEvent new_state) {
		m_event = new_state;
	}

	MoveEvent getEvent() const {
		return m_event;
	}

	enum class MoveEvent {
		StartMovingUp,
		// StartMovingRight,
		// StartMovingDown,
		// StartMovingLeft,

		StopMovingUp,
		// StopMovingRight,
		// StopMovingDown,
		// StopMovingLeft,
	};

private:
	MoveEvent m_event;
};


struct MoveInteractor {
	void execute(const MoveRequestModel& req, game_models::Player& player) const;
};

} // namespace invasion::interactors


#endif // INTERACTOR_H_