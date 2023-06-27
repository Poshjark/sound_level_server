#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "volume_handler.h"
#include "command_handler.h"

#define SERVER_PORT 3000
#define MESSAGE_MAX_LENGTH 1024
#define MESSAGE_LENGTH 6u
#define COMMAND_LENGTH 3

using namespace boost::asio;
using ip::tcp;


class ConnectionHandler : public boost::enable_shared_from_this<ConnectionHandler>
{
    constexpr static uint32_t maxMessageLength = 1024u;
public:
    using Ptr = boost::shared_ptr<ConnectionHandler>;

    ConnectionHandler(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler);
    static Ptr Create(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler);

    tcp::socket& GetSocket();

    void Start();

private:
    

    void _HandleRead(const boost::system::error_code& err, size_t bytes_transferred);
    void _HandleWrite(const boost::system::error_code& err);

    boost::system::error_code err;
    tcp::socket m_socket;
    std::string response;

    char receive_buffer[maxMessageLength];

    CommandHandler::Ptr m_pCommandHandler;

};

