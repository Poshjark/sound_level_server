#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "volume_handler.h"
#include "command_handler.h"


#define SERVER_PORT 3000
#define MESSAGE_MAX_LENGTH 1024
#define MESSAGE_LENGTH 6u
#define COMMAND_LENGTH 3

using namespace boost::asio;
using ip::tcp;

class con_handler : public boost::enable_shared_from_this<con_handler>
{
private:
    boost::system::error_code err = boost::system::error_code();;
    tcp::socket sock;
    std::string response;
    enum { max_length = MESSAGE_MAX_LENGTH }; // Bad practice on defining consts!!
    uint8_t receive_buffer[max_length];
    VolumeHandler* volume_handler;
    CommandHandler command_handler;
public:
    typedef boost::shared_ptr<con_handler> pointer;
    con_handler(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler_input);

    static pointer create(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler);
    //socket creation
    tcp::socket& socket();

    void start();
private:
    void handle_read(const boost::system::error_code& err, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& err);
};


class Server
{
private:
    tcp::acceptor acceptor_;
    VolumeHandler* volume_handler = nullptr;
    void start_accept();
public:
    Server(boost::asio::io_service& io_service, std::string ip_v4); // TODO: Singleton?
    ~Server();
private:
    void handle_accept(con_handler::pointer connection, const boost::system::error_code& err);

};
