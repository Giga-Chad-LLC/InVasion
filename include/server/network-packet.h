#ifndef INVASION_SERVER_NETWORK_PACKET_H
#define INVASION_SERVER_NETWORK_PACKET_H

#include <memory>

namespace invasion::session {
    // When adding new message type, update `NetworkPacketRequest::getMessageTypeById` method
    enum class RequestModel_t : uint32_t {
        MoveRequestModel = 0,
        UpdateGameStateRequestModel = 1,
        ShootRequestModel = 2,
        PlayerActionRequestModel = 100,
        UnknownRequestModel
    };
    enum class ResponseModel_t : uint32_t {
        PlayerPositionResponseModel = 1000,
        PlayerActionResponseModel = 1002,
        PlayerIdResponseModel = 1003,
		PlayersPositionsResponseModel = 1004,
        ShootingStateResponseModel = 1005,
        UnknownResponseModel
    };

    class NetworkPacket {
    protected:
        std::unique_ptr<char[]> bytes;
        uint32_t bytesLength;

        NetworkPacket(std::unique_ptr<char[]> &&bytes_, uint32_t bytesLength_) : bytes(std::move(bytes_)),
                                                                                    bytesLength(bytesLength_) {}

        NetworkPacket() : bytes(nullptr), bytesLength(0U) {}

    public:
        // returns pointer to the beginning of the bytes
        char *getStoredBytes() const {
            return bytes.get();
        }

        // returns a `unique_ptr<char[]>` to the `bytes` array
        std::unique_ptr<char[]> getPureBytes() {
            std::unique_ptr<char[]> buffer_ptr(new char[bytesLength]);
            std::memcpy(buffer_ptr.get(), getStoredBytes(), bytesLength);
            
            return std::move(buffer_ptr);
        }

        // returns bytes data length
        uint32_t bytesSize() const {
            return bytesLength;
        }
    };

    class NetworkPacketRequest : public NetworkPacket {
        RequestModel_t messageType;

    public:
        NetworkPacketRequest(std::unique_ptr<char[]> &&bytes_ptr,
                             RequestModel_t messageType_, uint32_t bytesLength_) : NetworkPacket(
                std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}

        NetworkPacketRequest() : NetworkPacket(), messageType(RequestModel_t::UnknownRequestModel) {}

        // returns message type 
        RequestModel_t getMessageType() const {
            return messageType;
        }

        // returns message type enum object by its `uint32_t` representation
        inline static RequestModel_t getMessageTypeById(uint32_t type) {
            if (type == static_cast<uint32_t> (RequestModel_t::PlayerActionRequestModel)) {
                return RequestModel_t::PlayerActionRequestModel;
            }
            else if (type == static_cast<uint32_t> (RequestModel_t::MoveRequestModel)) {
                return RequestModel_t::MoveRequestModel;
            }
            else if (type == static_cast<uint32_t> (RequestModel_t::ShootRequestModel)) {
                return RequestModel_t::ShootRequestModel;
            }
            else if (type == static_cast<uint32_t> (RequestModel_t::UpdateGameStateRequestModel)) {
                return RequestModel_t::UpdateGameStateRequestModel;
            }

            return RequestModel_t::UnknownRequestModel;
        }

        // performs the serialization for Network Packet request-protocol: [4 bytes: length][4 bytes: type][`length` bytes: bytes] 
        std::shared_ptr<char[]> serializeToByteArray() {
            uint32_t type = static_cast<uint32_t> (messageType);

            char *buffer = new char[bytesLength + sizeof(type) + sizeof(bytesLength)];
            std::shared_ptr<char[]> buffer_ptr(buffer);

            std::memcpy(buffer_ptr.get(), reinterpret_cast<char*> (&bytesLength), sizeof(bytesLength));
            std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(bytesLength)), reinterpret_cast<char *> (&type), sizeof(type));
            std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(bytesLength) + sizeof(type)), getStoredBytes(), bytesLength);
            return buffer_ptr;
        }
    };

    class NetworkPacketResponse : public NetworkPacket {
        ResponseModel_t messageType;

    public:
        NetworkPacketResponse(std::unique_ptr<char[]> &&bytes_ptr,
                              ResponseModel_t messageType_, uint32_t bytesLength_) : NetworkPacket(
                std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}

        NetworkPacketResponse() : NetworkPacket(nullptr, 0U), messageType(ResponseModel_t::UnknownResponseModel) {}

        NetworkPacketResponse(const NetworkPacketResponse &other) {
            *this = other;
        }

        NetworkPacketResponse(NetworkPacketResponse &&other) {
            *this = std::move(other);
        }

        NetworkPacketResponse &operator=(const NetworkPacketResponse &other) {
            bytesLength = other.bytesLength;
            messageType = other.messageType;
            char *newBytes = new char[other.bytesLength];

            std::memcpy(newBytes, other.getStoredBytes(), other.bytesLength);
            bytes.reset(newBytes);

            return *this;
        }

        NetworkPacketResponse &operator=(NetworkPacketResponse &&other) {
            bytes.swap(other.bytes);
            bytesLength = std::move(other.bytesLength);
            messageType = std::move(other.messageType);

            return *this;
        }

        // performs the serialization for Network Packet response-protocol: [4 bytes: bytes length][4 bytes: message_type][bytes] 
        std::shared_ptr<char[]> serializeToByteArray() {
            uint32_t type = static_cast<uint32_t> (messageType);
            uint32_t length = bytesSize();

            char *buffer = new char[length + sizeof(type) + sizeof(length)];
            std::shared_ptr<char[]> buffer_ptr(buffer);
            
            std::memcpy(buffer_ptr.get(), reinterpret_cast<char *> (&length), sizeof(length)); // [4 bytes: length]
            std::memcpy(buffer_ptr.get() + sizeof(length), reinterpret_cast<char *> (&type), sizeof(type)); // [4 bytes: type]
            std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(length)) + static_cast<int> (sizeof(type)), getStoredBytes(), bytesLength); // [length bytes: data]

            return buffer_ptr;
        }

        // returns message type 
        ResponseModel_t getMessageType() const {
            return messageType;
        }
    };
}

#endif