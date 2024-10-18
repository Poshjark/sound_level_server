#include "stdafx.h"

#include "IncomingDataQueue.h"

IncomingDataQueue::IncomingDataQueue(IRawDataProcessor& consumer)
	: m_RawDataProcessor(consumer)
{

}

void IncomingDataQueue::Start()
{
	m_procThread = ::boost::thread{ ::boost::bind(&IncomingDataQueue::_ThreadRoutine, this) };
}

void IncomingDataQueue::Stop()
{
	if (m_procThread.joinable())
	{
		m_StopEvent.Activate();
		m_procThread.interrupt();
		m_procThread.join();
	}
}

void IncomingDataQueue::_ThreadRoutine()
{
	try
	{
		while (true)
		{
			constexpr auto stopEventActivated = WAIT_OBJECT_0;
			constexpr auto incomingDataEventActivated = WAIT_OBJECT_0 + 1;
			const ::std::vector<Event*> events{ &m_StopEvent, &m_IncomingEvent };
			const auto ret = Event::WaitMultiple(events, INFINITE);
			if (ret == stopEventActivated)
			{
				_OnStop();
				return;
			}
			else if (ret == incomingDataEventActivated)
			{
				_ProcessQueue();
			}
		}
	}
	catch (::boost::thread_interrupted& e)
	{
		// Всё окай
	}
	catch (::std::exception)
	{
		// TODO:: Лог!
	}
}

void IncomingDataQueue::_OnStop()
{

}

void IncomingDataQueue::_ProcessQueue()
{
	Utils::Locker lock{ m_QueueCs };
	::std::swap(m_IncomingQueue, m_OnProcessQueue);
	while (!m_OnProcessQueue.empty())
	{
		_ProcessData(m_OnProcessQueue.front());
		m_OnProcessQueue.pop();
	}
}

void IncomingDataQueue::_ProcessData(RawData& data)
{
	m_RawDataProcessor.Process(&data);
}
