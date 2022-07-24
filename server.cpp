#include "server.h"


typedef boost::shared_ptr<con_handler> pointer;



con_handler::con_handler(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler_input) :
    sock(io_service), volume_handler(volume_handler_input),
    command_handler(CommandHandler(volume_handler_input)) {};


pointer con_handler::create(boost::asio::any_io_executor& io_service, VolumeHandler* volume_handler)
    {
        return pointer(new con_handler(io_service,volume_handler));
    }

tcp::socket& con_handler::socket()
{
    return sock;
}

void con_handler::start()
{
    sock.async_read_some(
        boost::asio::buffer(receive_buffer, MESSAGE_LENGTH),
        boost::bind(&con_handler::handle_read,
            shared_from_this(),
            err,
            MESSAGE_LENGTH));


}


void con_handler::handle_write(const boost::system::error_code& err)
{
    if (!err) {
    }
    else {
        std::cerr << "error: " << err.message() << std::endl;
        sock.close();
    }
}

void con_handler::handle_read(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        auto result = command_handler.execute_command(receive_buffer, bytes_transferred);
        response = result.what() + '\n';
        sock.async_write_some(
            boost::asio::buffer(response),
            boost::bind(&con_handler::handle_write,
                shared_from_this(),
                err));

        if (!result) {
            sock.close();
            std::cerr << "Error occured " << result.what() << std::endl;
        }


    }
    else {
        std::cerr << "Error occured " << err.message() << std::endl;
        sock.close();
    }

}


void Server::start_accept()
{   // getting executor(ioservice) to bind async operations to
    auto executor = acceptor_.get_executor();
    con_handler::pointer connection = con_handler::create(executor, volume_handler);

    // asynchronous accept operation and wait for a new connection.
    acceptor_.async_accept(connection->socket(),
        boost::bind(&Server::handle_accept, this, connection,
            boost::asio::placeholders::error));
}

Server::Server(boost::asio::io_service& io_service, std::string ip_v4) :
    acceptor_(io_service,
        tcp::endpoint(boost::asio::ip::address::from_string(ip_v4), SERVER_PORT)) {
    this->volume_handler = new VolumeHandler();
    start_accept();
}



void Server::handle_accept(con_handler::pointer connection, const boost::system::error_code& err)
{
    if (!err) {
        connection->start();
    }
    start_accept();
}

Server::~Server() {
    delete volume_handler;
}


