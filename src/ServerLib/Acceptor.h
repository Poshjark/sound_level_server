#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/function.hpp"
#include "boost/asio/io_context.hpp"

#include "Connection.h"

class Acceptor
{
public:

	using Ptr = ::boost::shared_ptr<Acceptor>;

	explicit Acceptor(const ::std::string& ipv4, ::std::uint16_t port, boost::asio::io_context& ctx);

	void AsyncAccept(::boost::function<void(Connection::Ptr)>&& cb);

private:
	boost::asio::ip::tcp::acceptor m_acceptor;
};

