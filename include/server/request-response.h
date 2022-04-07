//
// Created by nikolai on 05.03.2022.
//

#ifndef INVASIONSERVERS_REQUENT_AND_RESPONSE_MODELS_H
#define INVASIONSERVERS_REQUENT_AND_RESPONSE_MODELS_H

#include "player-position-response-model.pb.h"
#include "move-request-model.pb.h"
#include <memory>

struct RequestObject {
    uint32_t sizeMessage;
    uint32_t typeRequest;
    char arrBytes;
};

struct ResponseObject {
    uint32_t sizeMessage;
    uint32_t typeRequest;
    char arrBytes;

    ResponseObject() = default;

    ResponseObject(const ResponseObject &other) noexcept: sizeMessage(other.sizeMessage),
                                                          typeRequest(other.typeRequest),
                                                          arrBytes(other.arrBytes) {}

    ResponseObject(ResponseObject &&other) noexcept: sizeMessage(other.sizeMessage), typeRequest(other.typeRequest),
                                                     arrBytes(other.arrBytes) {}

    ResponseObject &operator=(const ResponseObject &other) noexcept {
        sizeMessage = other.sizeMessage;
        typeRequest = other.typeRequest;
        arrBytes = other.arrBytes;
        return *this;
    }

    ResponseObject &operator=(const ResponseObject &&other) noexcept {
        sizeMessage = other.sizeMessage;
        typeRequest = other.typeRequest;
        arrBytes = other.arrBytes;
        return *this;
    }
};


#endif //INVASIONSERVERS_REQUENT_AND_RESPONSE_MODELS_H
