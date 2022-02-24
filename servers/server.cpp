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
			tcp::iostream client(std::move(s));

			client << "Hello from C++ server!\n";

			while (client) {
				std::string msg;

				if (!(client >> msg)) {
					break;
				}

				client << "take it back: " << msg << std::endl;
				std::cout << "Received: " << msg << std::endl;
			}

			std::cout << "Disconnected" << std::endl;
		}).detach();
	}
}