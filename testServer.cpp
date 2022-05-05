#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "volume_handler.h"
#include "command_handler.h"


#define SERVER_IP_V4 "127.0.0.1"
#define SERVER_PORT 88
#define MESSAGE_MAX_LENGTH 1024
#define MESSAGE_LENGTH 6u
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
    const char* response;
    enum { max_length = MESSAGE_MAX_LENGTH };
    char receive_buffer[max_length];
    VolumeHandler* volume_handler;
    CommandHandler command_handler;
public:
    typedef boost::shared_ptr<con_handler> pointer;
    con_handler(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler_input) 
        : sock(io_service), volume_handler(volume_handler_input),
        command_handler(CommandHandler(volume_handler_input)) {}
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
            boost::asio::buffer(receive_buffer, MESSAGE_LENGTH),
            boost::bind(&con_handler::handle_read,
                shared_from_this(),
                boost::system::error_code(),
                MESSAGE_LENGTH));

        
    }

    void handle_read(const boost::system::error_code& err, size_t bytes_transferred){
        if (!err) {
            auto result = command_handler.execute_command(receive_buffer, bytes_transferred);
            response = result.what().c_str();
            std::cout << "Response for client: " << response << std::endl;
            sock.async_write_some(
                boost::asio::buffer(response,6),
                boost::bind(&con_handler::handle_write,
                    shared_from_this(),
                    boost::system::error_code(),
                    MESSAGE_LENGTH));

            if (!result) {
                sock.close();
                std::cerr << "Error occured " << result.what() << std::endl;
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
        acceptor_(io_service,
            tcp::endpoint(boost::asio::ip::address::from_string(SERVER_IP_V4), SERVER_PORT)){
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