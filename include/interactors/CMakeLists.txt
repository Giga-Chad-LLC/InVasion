cmake_minimum_required(VERSION 3.15)

project(InVasionInteractors CXX)

set(INTERACTORS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/MoveInteractor/move-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayersPositionsResponseInteractor/players-positions-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RotateWeaponInteractor/rotate-weapon-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShootInteractor/shoot-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/InitialStateResponseInteractor/initial-state-response-interactor.cpp

	PARENT_SCOPE
)