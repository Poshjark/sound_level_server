#include "Client.h"

Client::Client(Connection::Ptr&& connection, IncomingDataQueue& dataProcessor)
	: m_connection(::std::move(connection))
	, m_DataProcessor(dataProcessor)
{
	assert(m_connection);
	m_connection->WaitData(*this);
}

void Client::OnRead(void* data, ::std::size_t bytesRead)
{
	_DoSomething(data, bytesRead);
}

void Client::OnWrite()
{

}

void Client::OnDisconnected()
{

}

void Client::_DoSomething(void* data, ::std::size_t bytesRead)
{

}
