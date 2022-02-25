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
		
		std::thread([socket = std::move(s)]() mutable {
			std::cout << "Connected client: " << socket.remote_endpoint() << " --> " << socket.local_endpoint() << std::endl;
			tcp::iostream client(std::move(socket));
			

			client << "Hello from C++ server!" << std::endl;			

			// boost::asio::streambuf write_buffer;
  		// std::ostream output(&write_buffer);

			// output << "Hello, it is buffered data!";
			// std::cout << "Send to client: " << make_string(write_buffer) << std::endl;
			// boost::asio::write(socket, write_buffer);


			while (client) {
				// get the message data length in bytes
				std::uint32_t size;
				client.read(reinterpret_cast<char*> (&size), sizeof(size));
				// std::cout << "Msg size: " << size << std::endl;

				char arr[size];
				client.read(reinterpret_cast<char*> (&arr), size);

				PlayerAction action;
				action.ParseFromArray(arr, size);
				// std::cout << "Action retrieve: " << action.key_pressed() << ", size in bytes: " << action.ByteSizeLong() << std::endl;
				char action_buffer[action.ByteSizeLong()];
				action.SerializeToArray(action_buffer, action.ByteSizeLong());

				// std::cout << "Data should be " << data_expected_size << " bytes in length" << std::endl;
				// void *buffer = malloc(size);
				// address_book.SerializeToArray(buffer, size);
				// char action_buffer[];
				// action.SerializeToArray

				client.write(action_buffer, action.ByteSizeLong());
			}

			std::cout << "Disconnected" << std::endl;
		}).detach();
	}
}