
#ifndef INVASION_SERVER_SENDER_H
#define INVASION_SERVER_SENDER_H

#include "client.h"
#include "player.pb.h"
#include "player-id-response-model.pb.h"
#include "players-positions-response-model.pb.h"
#include "player-position-response-model.pb.h"
#include <thread>
#include <cstring>
#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include "network-packet.h"

namespace invasion::session {
    class ClientResponsesSender {
    public:
        ClientResponsesSender(std::shared_ptr<Client> client) {
            std::thread([client = client]() {
                while (true) {
                    NetworkPacketResponse response;
                    if (client->m_clientResponseQueue.consumeSync(response)) {
                        if (response.getMessageType() == ResponseModel_t::PlayerIdResponseModel) {
                            response_models::PlayerIdResponseModel model;
                            model.ParseFromArray(response.getStoredBytes(), response.bytesSize());
                            std::cout << "Client ID in session: " << model.playerid() << ", length: " << response.bytesSize() << std::endl;
                            
                            
                            std::shared_ptr<char> buffer = response.serializeToByteArray();
                            char* arr = new char[response.bytesSize() + 4];
                            std::memcpy(arr, buffer.get(), response.bytesSize() + 4);

                            for (int i = 0; i < response.bytesSize() + 4; i++) {
                                std::cout << (int)(arr[i]) << ' ';
                            }
                            std::cout << std::endl;
                        }
                        else if (response.getMessageType() == ResponseModel_t::PlayersPositionsResponseModel) {
                            // std::cout << "Array bytes length: " << response.bytesSize() << std::endl;
                            // response_models::PlayersPositionsResponseModel model;
                            // model.ParseFromArray(response.getStoredBytes(), response.bytesSize());

                            // auto players = model.players();
                            // for (auto& item : players) {
                            //     std::cout << item.playerid() << std::endl;
                            // }
                            // std::cout << "PP: " << response.bytesSize() << std::endl;
                        }

                        // std::cout << "Size: " << response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())) << std::endl;
                        std::cout << response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())) << std::endl;
                        std::shared_ptr<char> buffer = response.serializeToByteArray(); 
                        client->m_channel.write(buffer.get(), response.bytesSize() + sizeof(static_cast<std::uint32_t> (response.getMessageType())));
                    }
                }
            }).detach();
        }
    };
}

#endif //INVASION_SERVER_SENDER_H
