#include <iostream>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

#define SERVER_IP_V4 "127.0.0.1"
#define SERVER_PORT 88
#define MESSAGE_MAX_LENGTH 1024

void print_char_arr(const char* data) {
	int i = 0;
	while ((int)data[i] >= 32 && (int)data[i] <= 126) {
		std::cout << data[i];
		i++;
	}
	std::cout << std::endl;
}


int main() {
	boost::asio::io_service io_service;
	tcp::socket socket(io_service);
	
	string msg = "Default message\n";
	boost::system::error_code error;
	boost::asio::streambuf receive_buffer;
	std::cout << std::boolalpha;
	while (true) {
		std::cout << "Enter the message to send(exit to close client):\n" << std::endl;
		std::cin >> msg;
		if (msg == "exit") {
			break;
		}
		if (msg.size() >= MESSAGE_MAX_LENGTH) {
			std::cout << "Message is too large, it will be trimmed to max message size " << MESSAGE_MAX_LENGTH << " characters\n";
			msg = msg.substr(0, MESSAGE_MAX_LENGTH);
		}
		while (msg.size() < 6) {
			msg += ' ';
		}
		socket.connect(tcp::endpoint(boost::asio::ip::address::from_string(SERVER_IP_V4), SERVER_PORT));
		boost::asio::write(socket, boost::asio::buffer(msg), error);
		if (!error) {
			cout << "Message sent"  << endl;
			std::cout << "Was socket opened while sending? - " << socket.is_open() << std::endl;
		}
		else {
			cout << "Error while sending: " << error.message() << endl;
			std::cout << "Was socket opened while sending? - " << socket.is_open() << std::endl;
		}


		boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
		if (error && error != boost::asio::error::eof) {
			cout << "receive failed " << error.message() << endl;
			std::cout << "Is socket opened while failing receiving? - " <<  socket.is_open() << std::endl;
		}
		else {
			const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
			print_char_arr(data);
			std::cout << "Is socket opened after receiving? - " << socket.is_open() << std::endl;
		}
		msg.clear();
		socket.close();
	}
	
}

