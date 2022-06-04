cmake_minimum_required(VERSION 3.15)

project(InVasionInteractors CXX)

set(INTERACTORS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/MoveInteractor/move-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayersPositionsResponseInteractor/players-positions-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RotateWeaponInteractor/rotate-weapon-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ShootResponseInteractor/shoot-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BulletsPositionsResponseInteractor/bullets-positions-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DamagedPlayersResponseInteractor/damaged-players-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KilledPlayersResponseInteractor/killed-players-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RespawnPlayerInteractor/respawn-player-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameSessionStatsInteractor/game-session-stats-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ChangePlayerSpecializationInteractor/change-player-specialization-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ApplyAbilityInteractor/apply-ability-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UseSupplyInteractor/use-supply-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HandshakeResponseInteractor/handshake-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UpdatePlayerHitpointsResponseInteractor/update-player-hitpoints-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UpdatePlayerAmmoResponseInteractor/update-player-ammo-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ReloadWeaponResponseInteractor/reload-weapon-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WeaponDirectionResponseInteractor/weapon-direction-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RetrieveWeaponDirectionResponseInteractor/retrieve-weapon-direction-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SetPlayerUsernameInteractor/set-player-username-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FormJSONPlayerStatsResponseInteractor/form-json-player-stats-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UsernameResponseInteractor/username-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ClientConnectedResponseInteractor/client-connected-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ClientDisconnectedResponseInteractor/client-disconnected-response-interactor.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameOverResponseInteractor/game-over-response-interactor.cpp
	PARENT_SCOPE
)