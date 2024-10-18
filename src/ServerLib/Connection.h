#pragma once

#include "boost/enable_shared_from_this.hpp"
#include "boost/make_shared.hpp"
#include "boost/asio/ip/tcp.hpp"

#include "Includes/Server/IConnection.h"	
#include "Includes/Server/IConnectionEventsListener.h"	

constexpr static uint32_t maxMessageLength = 1024u;

struct Connection : public ::boost::enable_shared_from_this<Connection>, public IConnection
{
	using Ptr = boost::shared_ptr<Connection>;

	static Connection::Ptr Create(::boost::asio::ip::tcp::socket&& socket);

	explicit Connection(::boost::asio::ip::tcp::socket&& socket);

	virtual void Write(void* data, ::std::size_t size) override final;

	void WaitData(IConnectionEventsListener& listener);

	~Connection();

private:

	void _CloseSocket();

	bool _CloseSocketOnError(const boost::system::error_code& errc);

	void _HandleWrite(const boost::system::error_code& errc);

	void _HandleRead(const boost::system::error_code& errc, std::size_t bytesTransferred);

private:
	IConnectionEventsListener* _Listener;
	char receive_buffer[maxMessageLength];
	char outBuffer[maxMessageLength];
	boost::asio::ip::tcp::socket m_socket;
};
