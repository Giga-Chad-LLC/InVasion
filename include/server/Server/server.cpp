#include <iostream>
#include <memory>
#include <string>
#include "server.h"



namespace invasion::server {
using boost::asio::ip::tcp;

Server::Server(std::string host, short port):
    m_acceptor(m_ios, tcp::endpoint(boost::asio::ip::address::from_string(host), port)) {
    std::cout << "Listening at " << m_acceptor.local_endpoint() << std::endl;
}

Server::~Server() {
    stop();
}

void Server::start() {
    std::cout << "Starting server..." << std::endl;
    acceptNewClient();
}

void Server::acceptNewClient() {
    std::cout << "Accept new connection" << std::endl;

    // create execution thread for the newly connected client
    std::shared_ptr<boost::asio::io_service> ioService(new boost::asio::io_service);
    std::shared_ptr<boost::asio::io_service::work> ioWork(new boost::asio:: io_service::work(*ioService));
    std::shared_ptr<tcp::socket> socket(new tcp::socket(*ioService));

    Connection connection(ioService, ioWork, socket);

    m_acceptor.async_accept(*connection.socket, [this, connection](const boost::system::error_code& errorCode) {
        onAccept(errorCode, connection);
    });
}

void Server::onAccept(const boost::system::error_code& errorCode, Connection connection) {
    if (errorCode.value() == 0) {
        // add client to session
        std::cout << "Connected client: " << connection.socket->remote_endpoint() << " --> " << connection.socket->local_endpoint() << std::endl;

    }
    else {
        std::cout << "Server Error: Error code = " << errorCode.value() << ", Message: " << errorCode.message() << std::endl;
    }

    // Init next async accept operation if server has not been stopped yet.
    if (m_isActive.load()) {
        acceptNewClient();
    }
    else {
        // Stop accepting incoming connections and free allocated resources.
        m_acceptor.close();
    }
}

void Server::stop() {
    std::cout << "Stopping server..." << std::endl;
    m_isActive.store(false);
    m_ios.stop();
}


std::shared_ptr<Session> Server::getAvailableSession() {
    return nullptr;
}

std::shared_ptr<Session> Server::addSession() {
    return nullptr;
}

void Server::removeSession(uint32_t sessionId) {}
}