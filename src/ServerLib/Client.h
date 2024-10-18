#pragma once

#include "Includes/Server/IConnectionEventsListener.h"
#include "Connection.h"
#include "IncomingDataQueue.h"

class Client : public IConnectionEventsListener
{
public:
	Client(Connection::Ptr&& connection, IncomingDataQueue& dataProcessor);
private:
	virtual void OnRead(void* data, ::std::size_t bytesRead) override final;

	virtual void OnWrite() override final;
	virtual void OnDisconnected() override final;

	void _DoSomething(void* data, ::std::size_t bytesRead);

private:
	Connection::Ptr m_connection;
	IncomingDataQueue& m_DataProcessor;
};
