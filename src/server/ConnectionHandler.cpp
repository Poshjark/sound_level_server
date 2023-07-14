#include "ConnectionHandler.h"
#include <boost/make_shared.hpp>


ConnectionHandler::ConnectionHandler(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler)
    : m_socket(io_service)
    , m_pCommandHandler(commandHandler)
{

}


ConnectionHandler::Ptr ConnectionHandler::Create(boost::asio::any_io_executor& io_service, CommandHandler::Ptr commandHandler)
{
    return boost::make_shared<ConnectionHandler>(io_service, commandHandler);
}

tcp::socket& ConnectionHandler::GetSocket()
{
    return m_socket;
}

void ConnectionHandler::Start()
{
    m_socket.async_read_some
    (
        boost::asio::buffer(receive_buffer, maxMessageLength),
        boost::bind
            (
                &ConnectionHandler::_HandleRead,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
    );
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
        auto result = m_pCommandHandler->ExecuteRawCommand(receive_buffer, bytes_transferred);
        response = result.What();
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
