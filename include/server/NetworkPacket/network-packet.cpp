#include <cstring>
#include "network-packet.h"

namespace invasion::server {
// NetworkPacket
NetworkPacket::NetworkPacket(std::unique_ptr<char[]> &&bytes_, uint32_t bytesLength_) : bytes(std::move(bytes_)), bytesLength(bytesLength_) {}


NetworkPacket::NetworkPacket(): bytes(nullptr), bytesLength(0U) {}


char* NetworkPacket::getStoredBytes() const {
    return bytes.get();
}

std::unique_ptr<char[]> NetworkPacket::getPureBytes() {
    std::unique_ptr<char[]> buffer_ptr(new char[bytesLength]);
    std::memcpy(buffer_ptr.get(), getStoredBytes(), bytesLength);
    
    return std::move(buffer_ptr);
}

uint32_t NetworkPacket::bytesSize() const noexcept {
    return bytesLength;
}

uint32_t NetworkPacket::totalSize() const noexcept {
    return bytesLength + 8U;
}



// NetworkPacketRequest
NetworkPacketRequest::NetworkPacketRequest(std::unique_ptr<char[]> &&bytes_ptr, RequestModel_t messageType_, uint32_t bytesLength_) : NetworkPacket(std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}

NetworkPacketRequest::NetworkPacketRequest() : NetworkPacket(), messageType(RequestModel_t::UnknownRequestModel) {}

RequestModel_t NetworkPacketRequest::getMessageType() const {
    return messageType;
}

// returns message type enum object by its `uint32_t` representation
RequestModel_t NetworkPacketRequest::getMessageTypeById(uint32_t type) {
    if (type == static_cast<uint32_t> (RequestModel_t::MoveRequestModel)) {
        return RequestModel_t::MoveRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::ShootRequestModel)) {
        return RequestModel_t::ShootRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::UpdateGameStateRequestModel)) {
        return RequestModel_t::UpdateGameStateRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::RespawnPlayerRequestModel)) {
        return RequestModel_t::RespawnPlayerRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::SelectPlayerSpecializationRequestModel)) {
        return RequestModel_t::SelectPlayerSpecializationRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::ChangePlayerSpecializationRequestModel)) {
        return RequestModel_t::ChangePlayerSpecializationRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::ApplyAbilityRequestModel)) {
        return RequestModel_t::ApplyAbilityRequestModel;
    }
    else if (type == static_cast<uint32_t> (RequestModel_t::UseSupplyRequestModel)) {
        return RequestModel_t::UseSupplyRequestModel;
    }

    return RequestModel_t::UnknownRequestModel;
}


std::shared_ptr<char[]> NetworkPacketRequest::serializeToByteArray() {
    uint32_t type = static_cast<uint32_t> (messageType);

    char *buffer = new char[bytesLength + sizeof(type) + sizeof(bytesLength)];
    std::shared_ptr<char[]> buffer_ptr(buffer);

    std::memcpy(buffer_ptr.get(), reinterpret_cast<char*> (&bytesLength), sizeof(bytesLength));
    std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(bytesLength)), reinterpret_cast<char *> (&type), sizeof(type));
    std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(bytesLength) + sizeof(type)), getStoredBytes(), bytesLength);
    return buffer_ptr;
}



// NetworkPacketResponse
NetworkPacketResponse::NetworkPacketResponse(std::unique_ptr<char[]> &&bytes_ptr, ResponseModel_t messageType_, uint32_t bytesLength_) : NetworkPacket(std::move(bytes_ptr), bytesLength_), messageType(messageType_) {}

NetworkPacketResponse::NetworkPacketResponse(): NetworkPacket(nullptr, 0U), messageType(ResponseModel_t::UnknownResponseModel) {}


NetworkPacketResponse::NetworkPacketResponse(const NetworkPacketResponse &other) {
    *this = other;
}

NetworkPacketResponse::NetworkPacketResponse(NetworkPacketResponse &&other) {
    *this = std::move(other);
}

NetworkPacketResponse& NetworkPacketResponse::operator=(const NetworkPacketResponse &other) {
    bytesLength = other.bytesLength;
    messageType = other.messageType;
    char *newBytes = new char[other.bytesLength];

    std::memcpy(newBytes, other.getStoredBytes(), other.bytesLength);
    bytes.reset(newBytes);

    return *this;
}


NetworkPacketResponse& NetworkPacketResponse::operator=(NetworkPacketResponse &&other) {
    bytes.swap(other.bytes);
    bytesLength = std::move(other.bytesLength);
    messageType = std::move(other.messageType);

    return *this;
}

std::shared_ptr<char[]> NetworkPacketResponse::serializeToByteArray() {
    uint32_t type = static_cast<uint32_t> (messageType);
    uint32_t length = bytesSize();

    char *buffer = new char[length + sizeof(type) + sizeof(length)];
    std::shared_ptr<char[]> buffer_ptr(buffer);
    
    std::memcpy(buffer_ptr.get(), reinterpret_cast<char *> (&length), sizeof(length)); // [4 bytes: length]
    std::memcpy(buffer_ptr.get() + sizeof(length), reinterpret_cast<char *> (&type), sizeof(type)); // [4 bytes: type]
    std::memcpy(buffer_ptr.get() + static_cast<int> (sizeof(length)) + static_cast<int> (sizeof(type)), getStoredBytes(), bytesLength); // [length bytes: data]

    return buffer_ptr;
}

ResponseModel_t NetworkPacketResponse::getMessageType() const {
    return messageType;
}
}