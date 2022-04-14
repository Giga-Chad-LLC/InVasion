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
        NetworkPacket(std::unique_ptr<char[]> &&bytes_, uint32_t bytesLength_);
        NetworkPacket();

    public:
        // returns pointer to the beginning of the bytes
        char *getStoredBytes() const;
        // returns a `unique_ptr<char[]>` to the `bytes` array
        std::unique_ptr<char[]> getPureBytes();
        // returns bytes data length
        uint32_t bytesSize() const;
    };

    class NetworkPacketRequest : public NetworkPacket {
        RequestModel_t messageType;

    public:
        NetworkPacketRequest(std::unique_ptr<char[]> &&bytes_ptr, RequestModel_t messageType_, uint32_t bytesLength_);
        NetworkPacketRequest();
        // returns message type 
        RequestModel_t getMessageType() const;
        // returns message type enum object by its `uint32_t` representation
        static RequestModel_t getMessageTypeById(uint32_t type);
        // performs the serialization for Network Packet request-protocol: [4 bytes: length][4 bytes: type][`length` bytes: bytes] 
        std::shared_ptr<char[]> serializeToByteArray();
    };

    class NetworkPacketResponse : public NetworkPacket {
        ResponseModel_t messageType;

    public:
        NetworkPacketResponse(std::unique_ptr<char[]> &&bytes_ptr, ResponseModel_t messageType_, uint32_t bytesLength_);
        NetworkPacketResponse();
        NetworkPacketResponse(const NetworkPacketResponse &other);
        NetworkPacketResponse(NetworkPacketResponse &&other);
        NetworkPacketResponse &operator=(const NetworkPacketResponse &other);
        NetworkPacketResponse &operator=(NetworkPacketResponse &&other);

        // performs the serialization for Network Packet response-protocol: [4 bytes: bytes length][4 bytes: message_type][bytes] 
        std::shared_ptr<char[]> serializeToByteArray();
        // returns message type 
        ResponseModel_t getMessageType() const;
    };
}

#endif