#ifndef NETWORK_PACKET_H_
#define NETWORK_PACKET_H_

#include <memory>

namespace invasion::server {
    // When adding new message type, update `NetworkPacketRequest::getMessageTypeById` method
    enum class RequestModel_t : uint32_t {
        MoveRequestModel = 0,
        UpdateGameStateRequestModel = 1,
        ShootRequestModel = 2,
        RespawnPlayerRequestModel = 3,
        SelectPlayerSpecializationRequestModel = 4,
        ChangePlayerSpecializationRequestModel = 5,
        ApplyAbilityRequestModel = 6,
        UseSupplyRequestModel = 7,
        WeaponDirectionRequestModel = 8,
        ReloadWeaponRequestModel = 9,
        ClientCredentialsRequestModel = 10,
        UnknownRequestModel
    };
    enum class ResponseModel_t : uint32_t {
        PlayerPositionResponseModel = 1001,
        WeaponStateResponseModel = 1002, 
        GameStateResponseModel = 1003,
        RespawnPlayerResponseModel = 1004,
        GameOverResponseModel = 1005,
        PlayerSpecializationResponseModel = 1006,
        HandshakeResponseModel = 1007,
        SupplyResponseModel = 1008,
        UseSupplyResponseModel = 1009,
        UpdatePlayerAmmoResponseModel = 1010,
        UpdatePlayerHitpointsResponseModel = 1011,
        WeaponDirectionResponseModel = 1012,
		UsernameResponseModel = 1013,
        ClientConnectedResponseModel = 1014,
        ClientDisconnectedResponseModel = 1015,
        UnknownResponseModel
    };

    class NetworkPacketRequest;
    class NetworkPacketResponse;

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
        uint32_t bytesSize() const noexcept;
        // returns total size of all bytes (including 4 bytes for length and 4 bytes for type)
        uint32_t totalSize() const noexcept;

        // serialize
        template <class Model>
        inline static std::unique_ptr<char[]> serialize(const Model& model) {
            std::unique_ptr <char[]> buffer_ptr(new char[model.ByteSizeLong()]);
            model.SerializeToArray(buffer_ptr.get(), model.ByteSizeLong());
            return std::move(buffer_ptr);
        }
        // deserialize
        template <class Model>
        inline static void deserialize(
            Model& model,
            std::shared_ptr<NetworkPacketRequest> packet
        ) {
            model.ParseFromArray(packet->getStoredBytes(), packet->bytesSize());
        }
        // deserialize
        template <class Model>
        inline static void deserialize(
            Model& model,
            std::shared_ptr<NetworkPacketResponse> packet
        ) {
            model.ParseFromArray(packet->getStoredBytes(), packet->bytesSize());
        }
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