#include <algorithm>
#include <boost/asio.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "./cmake-build-debug/player.pb.h"

using boost::asio::ip::tcp;


// NOLINTNEXTLINE
int main(int argc, char *argv[]) {


    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8000));
    std::cout << "Listening at " << acceptor.local_endpoint() << std::endl;

    while (true) {
        tcp::socket s = acceptor.accept();

        std::thread([s = std::move(s)]() mutable {
            std::cout << "Connected client: " << s.remote_endpoint() << " --> " << s.local_endpoint() << std::endl;
//            boost::asio::streambuf buf;
//            boost::asio::read_until( s, buf, '\0' );
//            std::string data = boost::asio::buffer_cast<const char*>(buf.data());
//            std::cout << data << "124";
            tcp::iostream client(std::move(s));
            std::string mes;
            client << "Hello from C++ server!\n";
            while (client) {
                std::string buffer;
                PlayerAction tmp_;
                if (client >> buffer){
                    std::cout << buffer << '\n';
                }

                tmp_.ParseFromString(buffer);
                std::string answer_from_server;
                std::cout << tmp_.key_pressed() << '\n';
                tmp_.SerializeToString(&answer_from_server);
                client << answer_from_server;
            }
//                client >> buffer;

            std::cout << "Disconnected" << std::endl;
        }).detach();
    }
}