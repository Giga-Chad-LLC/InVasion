#include <algorithm>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <utility>
#include "player.pb.h"

using boost::asio::ip::tcp;


std::string make_string(boost::asio::streambuf& streambuf) {
	return {
		boost::asio::buffers_begin(streambuf.data()), 
		boost::asio::buffers_end(streambuf.data())
	};
}

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

			// PlayerAction action;
			// action.set_key_pressed(PlayerAction::Action::PlayerAction_Action_StartMovingLeft);

			while (client) {
				// std::string msg;
				// if (!(client >> msg)) {
				// 	break;
				// }
				// // client << "take it back: " << msg << std::endl;
				// std::cout << "Received: " << msg << std::endl;
				// std::size_t n = 4;
				// boost::asio::streambuf streambuf(n);
				// boost::asio::read(socket, streambuf, boost::asio::transfer_exactly(n));
				// std::cout << make_string(streambuf) << std::endl;
				std::uint32_t u32;
				client >> u32;
				std::cout << "Data: " << u32 << std::endl;
			}

			std::cout << "Disconnected" << std::endl;
		}).detach();
	}
}