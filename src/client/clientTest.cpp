#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <thread>

#include "Command.h"
#include "clientTest.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

void print_char_arr(const char* data) 
{
	int i = 0;
	while ((int)data[i] >= 32 && (int)data[i] <= 126) 
	{
		std::cout << data[i];
		i++;
	}
}
#define SERVER_PORT 3000

namespace
{
	constexpr static const char* defaultServerIP = { "127.0.0.1" };
	static std::string serverIP = defaultServerIP;
}


#define MESSAGE_MAX_LENGTH 1024
#define MESSAGE_LENGTH 32

bool readHandled = false;

uint8_t m_buffer[1024];

void SendData(const boost::asio::const_buffer& buff);

static void HandleRead(const boost::system::error_code& error, std::size_t bytes_transferred)
{
	print_char_arr((char*)*m_buffer);
	readHandled = true;
}

void SendCommand(const Command& command)
{
	SendData(boost::asio::buffer(command.Serialize()));
}

void SendStrMessage(const std::string& message)
{
	SendData(boost::asio::buffer(message));
}

void SendData(const boost::asio::const_buffer& buff)
{
	boost::asio::io_service ios;

	boost::asio::ip::tcp::endpoint
		endpoint(boost::asio::ip::address::from_string(::serverIP), SERVER_PORT);
	boost::asio::ip::tcp::socket socket(ios);
	socket.connect(endpoint);

	boost::system::error_code error;
	socket.write_some(buff, error);

	socket.async_read_some(boost::asio::buffer(m_buffer, 1024), HandleRead);

	while (!readHandled)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds{ 50u });
	}
	readHandled = false;

	if (error == boost::asio::error::eof)
	{
		std::cout << "Connection closed by server\n";
	}
	else if (error)
	{
		std::cout << "ERROR in connection" << std::endl;
		return;
	}
}

static void ReadServerIP(char* argc[])
{

	for (int i = 0; i < 16; ++i)
	{
		if (!argc[i])
			break;

		try
		{
			boost::asio::ip::address::from_string(argc[i]);
			::serverIP = argc[i];
			return;
		}
		catch (...)
		{
		}
	}

	std::cout << "Enter server ip:\t";

	while (true)
	{
		std::cin >> ::serverIP;
		try
		{
			boost::asio::ip::address::from_string(::serverIP);
			break;
		}
		catch(...)
		{
		}
		std::cout << "Wrong address, please, specify valid ip address(eg 127.0.0.1):\t";
		
	}


}


int main(int argv, char* argc[]) 
{

	ReadServerIP(argc);
	string msg = "Default message\n";
	
	while (true) {
		std::cout << "Enter command code to send(exit to close client, help to print availiavle codes):\n" << std::endl;
		std::cin >> msg;
		if (msg == "exit") {
			break;
		}

		if (msg == "help")
		{
			SendStrMessage(msg);
			msg.clear();
			continue;
		}

		uint8_t code = (uint8_t)Command::Code::Unknown;

		code = std::stoi(msg);

		try
		{
			Command c{ code };
			//if (c.RequiresData())
			//{
			//	Command::data_t data = 0;
			//	std::cout << "This command requires data: ";
			//	std::cin >> data;
			//	c.SetData(data);
			//}
			SendCommand(c);
		}
		catch(...)
		{
		}
		
		msg.clear();
	}
	
}

