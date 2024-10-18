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
 * @brief Инстанс сервера
*/
class Server : public ::boost::enable_shared_from_this<Server>
{
	using Locker = Utils::Locker;
public:
	/**
	 * @brief Конструктор сервера, прослушивающего указанный порт
	*/
	explicit Server(::std::uint16_t port);

	/*
	* Деструктер
	*/
	~Server();

	/**
	 * @brief Начать принимать подключения и обрабатывать запросы
	*/
	void Start();

	/**
	 * @brief  Находится ли сервер в работе
	*/
	bool OnWork() const;

	/**
	 * @brief Остановить сервер
	*/
	void Stop();

	/**
	 * @brief Синхронно дождаться остановки сервера (TODO:: херня какая-то - ты либо останавливай и жди, либо нахер иди, не?)
	*/
	void WaitForStopped();

	/**
	 * @brief Получить стоп-эвент(TODO:: в ту же степь, что и предыдущее замечание по WaitForStopped)
	*/
	Event& StopEvent();
private:

	/**
	 * @brief Рутина потока исполнения асинхронного контекста
	*/
	void _ProcRoutine();

	/**
	 * @brief TODO
	*/
	void _DropAllTasks();

	/**
	 * @brief Обработать установление подключения
	*/
	void _HandleAccept(Connection::Ptr connection);

	/**
	 * @brief Зарегистрировать задачу на принятие подключения
	*/
	void _StartAccept();

	/**
	 * @brief Инициализировать акцептора
	*/
	void _CreateAcceptor();

	// Порт
	const ::std::uint16_t _Port;

	// Асинхронный контекст исполнения сетевых операций
	boost::asio::io_context _Ctx;

	// Акцептор входящих подключений
	Acceptor::Ptr _Acceptor;

	// Список клиентов
	::std::vector<Client> _Clients;

	// Фиктивная задача для удержания контекста на плаву
	::boost::shared_ptr<work_t> _FictiveWork;

	// Поток исполнения асинхронного контекста
	::boost::thread _ProcThread;

	// Критическая секция(TODO:: чего?)
	mutable Utils::mutex_t _Mtx;

	// Событие останова
	Event					_StopEvent;

	// Потребитель входящих сырых данных
	IRawDataProcessor::Ptr		_RawDataConsumer;

	// Входящая очередь сырых данных
	IncomingDataQueue		_IncomingDataProcessor;
};
