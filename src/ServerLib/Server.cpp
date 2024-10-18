#include "Server.h"

namespace Constants
{
	static constexpr auto AllInterfacesIP = "0.0.0.0";
}

Server::Server(::std::uint16_t port)
	: _RawDataConsumer(::boost::make_shared<RawDataProcessor>())
	, _IncomingDataProcessor(*_RawDataConsumer)
	, _Port(port)
{
	_CreateAcceptor();

	BOOST_ASSERT(_Acceptor);
}

Server::~Server()
{
	Stop();
}

void Server::Start()
{
	Locker guard{ _Mtx };
	_FictiveWork.reset(new work_t(_Ctx));
	_IncomingDataProcessor.Start();
	_StartAccept();
	_ProcThread = ::boost::thread(::boost::bind(&Server::_ProcRoutine, this));
}

bool Server::OnWork() const
{
	Locker guard{ _Mtx };
	return _FictiveWork.get();
}

void Server::Stop()
{
	Locker guard{ _Mtx };
	_FictiveWork.reset();
	_Ctx.stop();
	_DropAllTasks();
	_StopEvent.Activate();
	_IncomingDataProcessor.Stop();
}

void Server::WaitForStopped()
{
	if (_ProcThread.joinable())
	{
		_ProcThread.interrupt();
		_ProcThread.join();
	}
}

Event& Server::StopEvent()
{
	return _StopEvent;
}

void Server::_ProcRoutine()
{
	_Ctx.run();
}

void Server::_DropAllTasks()
{

}

void Server::_HandleAccept(Connection::Ptr connection)
{
	_StartAccept();

	_Clients.emplace_back(::std::move(connection), _IncomingDataProcessor);

}

void Server::_StartAccept()
{
	namespace bph = ::boost::placeholders;
	_Acceptor->AsyncAccept(boost::bind(&Server::_HandleAccept, this, bph::_1));
}

void Server::_CreateAcceptor()
{
	_Acceptor = ::boost::make_shared<Acceptor>(Constants::AllInterfacesIP, _Port, _Ctx);
}
