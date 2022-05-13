cmake_minimum_required(VERSION 3.15)

project(InVasionServer CXX)

set(GAME-SERVER-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkPacket/network-packet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Server/server.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Session/session.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/GameEventsDispatcher/game-events-dispatcher.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Client/client.cpp

	PARENT_SCOPE
)