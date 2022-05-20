#include "client.h"

namespace invasion::session {
    Client::Client(tcp::socket &&socket, uint32_t playerId) : m_channel(std::move(socket)),
                                                              m_clientIdInGameSession(playerId) {}
}