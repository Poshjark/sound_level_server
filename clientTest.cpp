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
#define MESSAGE_LENGTH 32

void print_char_arr(const char* data) {
	int i = 0;
	while ((int)data[i] >= 32 && (int)data[i] <= 126) {
		std::cout << data[i];
		i++;
	}
	std::cout << std::endl;
}


void send_message(std::string message) {
	boost::asio::io_service ios;

	boost::asio::ip::tcp::endpoint
		endpoint(boost::asio::ip::address::from_string(SERVER_IP_V4), SERVER_PORT);
	boost::asio::ip::tcp::socket socket(ios);
	socket.connect(endpoint);

	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(message), error);

	std::string d_buf;
	boost::asio::read_until(socket, boost::asio::dynamic_buffer(d_buf), '\n');

	if (error == boost::asio::error::eof)
	{
		std::cout << "Connection closed by server\n";
	}
	else if (error)
	{
		std::cout << "ERROR in connection" << std::endl;
		return;
	}

	std::cout << "Received: " << d_buf;

}

int main() {
	
	
	string msg = "Default message\n";
	
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
		send_message(msg);
		
		msg.clear();
	}
	
}

