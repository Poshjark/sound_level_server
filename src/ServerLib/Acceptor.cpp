#include "Acceptor.h"

Acceptor::Acceptor(const::std::string& ipv4, ::std::uint16_t port, boost::asio::io_context& ctx)
	: m_acceptor(ctx.get_executor(), boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ipv4), port))
{

}

void Acceptor::AsyncAccept(::boost::function<void(Connection::Ptr)>&& cb)
{
	m_acceptor.async_accept
	(
		[callback = ::std::move(cb)](const boost::system::error_code& err, ::boost::asio::ip::tcp::socket socket)
		{
			// TODO:: LOG ERRORS
			if (err)
				return;

			auto connection = Connection::Create(::std::move(socket));
			callback(connection);
		}
	);
}
