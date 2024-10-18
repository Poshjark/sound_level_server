#pragma once

#include <queue>

#include "boost/thread.hpp"

#include "Includes/Server/IRawDataProcessor.h"
#include "Includes/Utils/Event.h"
#include "Includes/Utils/Usings.h"

class IncomingDataQueue
{
public:
	IncomingDataQueue(IRawDataProcessor& consumer);

	void Start();

	void Stop();
private:
	void _ThreadRoutine();

	void _OnStop();

	void _ProcessQueue();

	void _ProcessData(RawData& data);

	Event m_IncomingEvent;
	Event m_StopEvent;

	Utils::mutex_t m_QueueCs;
	::boost::thread m_procThread;
	::std::queue< RawData > m_OnProcessQueue;
	::std::queue< RawData > m_IncomingQueue;

	IRawDataProcessor& m_RawDataProcessor;
};
