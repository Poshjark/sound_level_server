#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>

#include "volume_handler.h"
#include "command_handler.h"

#include "ConnectionHandler.h"



#define SERVER_PORT 3000
#define MESSAGE_MAX_LENGTH 1024
#define MESSAGE_LENGTH 6u
#define COMMAND_LENGTH 3

using namespace boost::asio;
using ip::tcp;


class Server : boost::asio::detail::noncopyable
{

public:
    Server(boost::asio::io_service& io_service, const std::string& ip_v4); // TODO: Singleton?
    ~Server();

private:
    void _StartAccept();
    void _HandleAccept(ConnectionHandler::Ptr connection, const boost::system::error_code& err);

    tcp::acceptor m_acceptor;
    CommandHandler::Ptr m_pCommandHandler;

};
