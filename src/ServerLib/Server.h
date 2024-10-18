#pragma once

#include "boost/thread.hpp"
#include "boost/asio.hpp"

#include "Includes/Utils/Event.h"
#include "Includes/Utils/Usings.h"
#include "Connection.h"
#include "Acceptor.h"
#include "Client.h"
#include "RawDataProcessor.h"

using work_t = ::boost::asio::io_service::work;

/**
 * @brief ������� �������
*/
class Server : public ::boost::enable_shared_from_this<Server>
{
	using Locker = Utils::Locker;
public:
	/**
	 * @brief ����������� �������, ��������������� ��������� ����
	*/
	explicit Server(::std::uint16_t port);

	/*
	* ����������
	*/
	~Server();

	/**
	 * @brief ������ ��������� ����������� � ������������ �������
	*/
	void Start();

	/**
	 * @brief  ��������� �� ������ � ������
	*/
	bool OnWork() const;

	/**
	 * @brief ���������� ������
	*/
	void Stop();

	/**
	 * @brief ��������� ��������� ��������� ������� (TODO:: ����� �����-�� - �� ���� ������������ � ���, ���� ����� ���, ��?)
	*/
	void WaitForStopped();

	/**
	 * @brief �������� ����-�����(TODO:: � �� �� �����, ��� � ���������� ��������� �� WaitForStopped)
	*/
	Event& StopEvent();
private:

	/**
	 * @brief ������ ������ ���������� ������������ ���������
	*/
	void _ProcRoutine();

	/**
	 * @brief TODO
	*/
	void _DropAllTasks();

	/**
	 * @brief ���������� ������������ �����������
	*/
	void _HandleAccept(Connection::Ptr connection);

	/**
	 * @brief ���������������� ������ �� �������� �����������
	*/
	void _StartAccept();

	/**
	 * @brief ���������������� ���������
	*/
	void _CreateAcceptor();

	// ����
	const ::std::uint16_t _Port;

	// ����������� �������� ���������� ������� ��������
	boost::asio::io_context _Ctx;

	// �������� �������� �����������
	Acceptor::Ptr _Acceptor;

	// ������ ��������
	::std::vector<Client> _Clients;

	// ��������� ������ ��� ��������� ��������� �� �����
	::boost::shared_ptr<work_t> _FictiveWork;

	// ����� ���������� ������������ ���������
	::boost::thread _ProcThread;

	// ����������� ������(TODO:: ����?)
	mutable Utils::mutex_t _Mtx;

	// ������� ��������
	Event					_StopEvent;

	// ����������� �������� ����� ������
	IRawDataProcessor::Ptr		_RawDataConsumer;

	// �������� ������� ����� ������
	IncomingDataQueue		_IncomingDataProcessor;
};
