#ifndef INVASION_SERVER_NETWORK_PACKET_H
#define INVASION_SERVER_NETWORK_PACKET_H

#include <memory>

namespace invasion::session {
    enum class RequestModel_t : std::uint32_t {
        MoveRequestModel = 0,
	    UpdateGameStateRequestModel = 1,
	    PlayerActionRequestModel = 2,
        UnknownRequestModel
    };
    enum class ResponseModel_t : std::uint32_t {
        PlayerPositionResponseModel = 1000,
	    PlayerActionResponseModel = 1002,
        UnknownResponseModel
    };

    class NetworkPacket {
    protected:
        int playerId;
        std::unique_ptr<char> bytes;
        std::uint32_t bytesLength;
        NetworkPacket(std::unique_ptr<char> &&bytes_, std::uint32_t bytesLength_): bytes(std::move(bytes_)), bytesLength(bytesLength_) {}
        NetworkPacket(): bytes(nullptr), bytesLength(0U) {}

    public:
        char* getStoredBytes() const {
            return bytes.get();
        }
        int getPlayerId() const noexcept{
            return playerId;
        }
        std::uint32_t bytesSize() const {
            return bytesLength;
        }
    };

    class NetworkPacketRequest : public NetworkPacket {
        RequestModel_t messageType;
        
    public:
        NetworkPacketRequest(std::unique_ptr<char> &&bytes_ptr,
            RequestModel_t messageType_, std::uint32_t bytesLength_): NetworkPacket(std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}
        NetworkPacketRequest(): NetworkPacket(), messageType(RequestModel_t::UnknownRequestModel) {}
        void setPlayerId(int id) noexcept{
            playerId = id;
        }
        RequestModel_t getMessageType() const {
            return messageType;
        }

        inline static RequestModel_t getMessageTypeById(std::uint32_t type) {
            if (type == static_cast<std::uint32_t> (RequestModel_t::PlayerActionRequestModel)) {
                return RequestModel_t::PlayerActionRequestModel;
            }
            else if (type == static_cast<std::uint32_t> (RequestModel_t::MoveRequestModel)) {
                return RequestModel_t::MoveRequestModel;
            }
            
            return RequestModel_t::UnknownRequestModel;
        } 
    };

    class NetworkPacketResponse : public NetworkPacket {
        ResponseModel_t messageType;
        
    public:
        NetworkPacketResponse(std::unique_ptr<char> &&bytes_ptr,
            ResponseModel_t messageType_, std::uint32_t bytesLength_): NetworkPacket(std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}
        NetworkPacketResponse(): NetworkPacket(nullptr, 0U), messageType(ResponseModel_t::UnknownResponseModel) {}
        NetworkPacketResponse(const NetworkPacketResponse& other) {
            *this = other;
        }
        NetworkPacketResponse(NetworkPacketResponse&& other) {
            *this = std::move(other);
        }

        NetworkPacketResponse& operator=(const NetworkPacketResponse& other) {
            bytesLength = other.bytesLength;
            messageType = other.messageType;
            char* newBytes = new char[other.bytesLength];

            std::memcpy(newBytes, other.getStoredBytes(), other.bytesLength);
            bytes.reset(newBytes);

            return *this;
        }

        NetworkPacketResponse& operator=(NetworkPacketResponse&& other) {
            bytes.swap(other.bytes);
            bytesLength = std::move(other.bytesLength);
            messageType = std::move(other.messageType);
            
            return *this;
        }

        ResponseModel_t getMessageType() const {
            return messageType;
        }

        inline static ResponseModel_t getMessageTypeById(std::uint32_t type) {
            if (type == static_cast<std::uint32_t> (ResponseModel_t::PlayerActionResponseModel)) {
                return ResponseModel_t::PlayerActionResponseModel;
            }
            else if (type == static_cast<std::uint32_t> (ResponseModel_t::PlayerPositionResponseModel)) {
                return ResponseModel_t::PlayerPositionResponseModel;
            }
            
            return ResponseModel_t::UnknownResponseModel;
        } 
    };
}

#endif