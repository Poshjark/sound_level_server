#include "stdafx.h"

#include "Connection.h"

Connection::Ptr Connection::Create(::boost::asio::ip::tcp::socket&& socket)
{
	return ::boost::make_shared<Connection>(::std::move(socket));
}

Connection::Connection(::boost::asio::ip::tcp::socket&& socket)
	: m_socket(::std::move(socket))
{

}

void Connection::Write(void* data, ::std::size_t size)
{
	// TODO:: выделять большие буферы на большие посылки
	assert(size <= maxMessageLength);
	::std::memcpy(outBuffer, data, size);

	// TODO:: ring buffer?
	m_socket.async_write_some(
		boost::asio::buffer(receive_buffer),
		boost::bind(&Connection::_HandleWrite,
			shared_from_this(),
			boost::asio::placeholders::error));

}

void Connection::WaitData(IConnectionEventsListener& listener)
{
	_Listener = &listener;
	m_socket.async_read_some
	(
		boost::asio::buffer(receive_buffer, maxMessageLength),
		boost::bind
		(
			&Connection::_HandleRead,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
}

Connection::~Connection()
{
	_CloseSocket();
}

void Connection::_CloseSocket()
{
	try
	{
		if (m_socket.is_open())
			m_socket.close();
	}
	catch (::std::exception& e)
	{
		// TODO:: Сделать нормальный лог(видимо, файловый, скорее всего текстовый)
		::std::cout << e.what() << ::std::endl;
	}
}

bool Connection::_CloseSocketOnError(const boost::system::error_code& errc)
{
	if (!errc)
		return false;

	_Listener->OnDisconnected();
	_CloseSocket();
	return true;
}

void Connection::_HandleWrite(const boost::system::error_code& errc)
{
	if (_CloseSocketOnError(errc))
		return;

	_Listener->OnWrite();
}

void Connection::_HandleRead(const boost::system::error_code& errc, std::size_t bytesTransferred)
{
	_Listener->OnRead(receive_buffer, bytesTransferred);

	m_socket.async_write_some(
		boost::asio::buffer(receive_buffer),
		boost::bind(&Connection::_HandleWrite,
			shared_from_this(),
			boost::asio::placeholders::error));
}
