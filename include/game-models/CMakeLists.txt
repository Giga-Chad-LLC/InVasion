cmake_minimum_required(VERSION 3.15)

project(InVasionGameModels CXX)

set(GAME-MODELS-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Vector2D/vector2d.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BoxCollider/box-collider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Object/object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KinematicObject/kinematic-object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StaticObject/static-object.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Player/player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Player/player-life-state.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Player/player-game-session-stats.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Stormtrooper/stormtrooper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sentinel/sentinel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/UnspecializedPlayer/unspecialized-player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AbilityEndowedPlayer/ability-endowed-player.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Support/support.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Medic/medic.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayersFactory/players-factory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Bullet/bullet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Weapon/weapon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameWorldStorage/game-world-storage.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameSession/game-session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameSession/game-session-stats.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PlayerManager/player-manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BulletManager/bullet-manager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StaticSupply/static-supply.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AidKit/aid-kit.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AmmoCrate/ammo-crate.cpp

	PARENT_SCOPE
)