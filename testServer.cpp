#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "volume_handler.h"

#define SERVER_IP_V4 "127.0.0.1"
#define SERVER_PORT 88
#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;



void print_char_arr(char data[]) {
    int i = 0;
    while ((int)data[i] >= 32 && (int)data[i] <= 126) {
        std::cout << data[i];
        i++;
    }
    std::cout << std::endl;
}


class con_handler : public boost::enable_shared_from_this<con_handler>
{
private:
    tcp::socket sock;
    std::string message = "";
    enum { max_length = MESSAGE_MAX_LENGTH };
    char my_data_message[max_length];
    VolumeHandler* volume_handler;
public:
    typedef boost::shared_ptr<con_handler> pointer;
    con_handler(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler_input) 
        : sock(io_service), volume_handler(volume_handler_input)  {}
    // creating the pointer
    static pointer create(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler)
    {
        return pointer(new con_handler(io_service,volume_handler));
    }
    //socket creation
    tcp::socket& socket()
    {
        return sock;
    }

    void start()
    {
        sock.async_read_some(
            boost::asio::buffer(my_data_message, max_length),
            boost::bind(&con_handler::handle_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));

        sock.async_write_some(
            boost::asio::buffer(message, max_length),
            boost::bind(&con_handler::handle_write,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err) {

            //print_char_arr(my_data_message);
            std::string command="000";
            for (size_t i = 0; i < COMMAND_LENGTH; i++) {
                command[i] = my_data_message[i];
            }
            std::cout << "Command received: " << command << std::endl;
            if (command == "vsu") {
                volume_handler->volume_step_up();
            }
            else if (command == "vsd") {
                volume_handler->volume_step_down();
            }
            else if (command == "vsm") {
                volume_handler->mute();
            }
            else if (command == "vsl") {
                try{
                    std::string new_sound_level_str("   ");
                    for (size_t i = 3, j = 0; i < 7 && std::isdigit(my_data_message[i]); i++, j++) {
                        new_sound_level_str[j] = my_data_message[i];
                    }
                    int new_sound_level_int = std::stoi(new_sound_level_str);
                    std::clog << "New sound level to set: " << new_sound_level_int << std::endl;
                    volume_handler->set_volume(new_sound_level_int);
                }
                catch (std::exception error) {
                    std::cerr << "error: " << error.what() << std::endl;
                    sock.close();
                }
                    
            }
        }
        else {
            std::cerr << "error: " << err.message() << std::endl;
            sock.close();
        }
    }
    void handle_write(const boost::system::error_code& err, size_t bytes_transferred)
    {
        if (!err) {
            //cout << "Server sent Hello message!" << endl;
        }
        else {
            std::cerr << "error: " << err.message() << endl;
            sock.close();
        }
    }
};


class Server
{
private:
    tcp::acceptor acceptor_;
    VolumeHandler* volume_handler = nullptr;
    void start_accept()
    {
        // socket
        auto executor = acceptor_.get_executor();
        con_handler::pointer connection = con_handler::create(executor,volume_handler);

        // asynchronous accept operation and wait for a new connection.
        acceptor_.async_accept(connection->socket(),
            boost::bind(&Server::handle_accept, this, connection,
                boost::asio::placeholders::error));
    }
public:
    //constructor for accepting connection from client
    Server(boost::asio::io_service& io_service) :
        acceptor_(io_service, tcp::endpoint(boost::asio::ip::address::from_string(SERVER_IP_V4), SERVER_PORT)){
        this->volume_handler = new VolumeHandler();
        start_accept();
    }
    void handle_accept(con_handler::pointer connection, const boost::system::error_code& err)
    {
        if (!err) {
            connection->start();
        }
        start_accept();
    }

};

int main(int argc, char* argv[])
{
    try
    {
        boost::asio::io_service io_service;
        Server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << endl;
    }
    return 0;
}