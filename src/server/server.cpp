#include "server.h"
#include <boost/make_shared.hpp>

typedef boost::shared_ptr<ConnectionHandler> Ptr;



ConnectionHandler::ConnectionHandler(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler)
    : m_socket(io_service)
    , m_pCommandHandler()
{

}


Ptr ConnectionHandler::Create(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler)
{
    return boost::make_shared<ConnectionHandler>(io_service, commandHandler);
}

tcp::socket& ConnectionHandler::GetSocket()
{
    return m_socket;
}

void ConnectionHandler::Start()
{
    m_socket.async_read_some(
        boost::asio::buffer(receive_buffer, maxMessageLength),
        boost::bind(&ConnectionHandler::_HandleRead,
            shared_from_this(),
            err,
            boost::placeholders::_1));


}


void ConnectionHandler::_HandleWrite(const boost::system::error_code& err)
{
    if (!err) 
    {

    }
    else 
    {
        std::cerr << "error: " << err.message() << std::endl;
        m_socket.close();
    }
}

void ConnectionHandler::_HandleRead(const boost::system::error_code& err, size_t bytes_transferred) 
{
    if (!err) 
    {
        auto result = m_pCommandHandler->ExecuteCommand(receive_buffer, bytes_transferred);
        response = result.What() + '\n';
        m_socket.async_write_some(
            boost::asio::buffer(response),
            boost::bind(&ConnectionHandler::_HandleWrite,
                shared_from_this(),
                err));

        if (!result) {
            m_socket.close();
            std::cerr << "Error occured " << result.What() << std::endl;
        }


    }
    else 
    {
        std::cerr << "Error occured " << err.message() << std::endl;
        m_socket.close();
    }

}


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


