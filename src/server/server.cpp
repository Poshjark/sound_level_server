#include "Server.h"
#include <boost/make_shared.hpp>

void Server::_StartAccept()
{   
    // getting executor(ioservice) to bind async operations to
    auto executor = m_acceptor.get_executor();
    ConnectionHandler::Ptr connection = ConnectionHandler::Create(executor, m_pCommandHandler);

    // asynchronous accept operation and wait for a new connection.
    m_acceptor.async_accept(connection->GetSocket(),
        boost::bind(&Server::_HandleAccept, this, connection,
            boost::asio::placeholders::error));
}

Server::Server(boost::asio::io_service& io_service, const std::string& ip_v4) 
    : m_acceptor(io_service, tcp::endpoint(boost::asio::ip::address::from_string(ip_v4), SERVER_PORT)) 
    , m_pCommandHandler(CommandHandler::Create())
{
    _StartAccept();
}



void Server::_HandleAccept(ConnectionHandler::Ptr connection, const boost::system::error_code& err)
{
    if (!err) 
    {
        connection->Start();
    }
    _StartAccept();
}

Server::~Server() 
{

}


