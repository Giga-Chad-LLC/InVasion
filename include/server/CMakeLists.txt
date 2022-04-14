cmake_minimum_required(VERSION 3.15)

project(InVasionServer CXX)

set(GAME-SERVER-SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/Engine/engine.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/NetworkPacket/network-packet.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Receiver/receiver.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Sender/sender.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Client/client.cpp

	PARENT_SCOPE
)