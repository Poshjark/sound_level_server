#include "Includes/Utils/Usings.h"
#include "Includes/Utils/Event.h"

#include <algorithm>

Event::Event()
	: m_handle(::CreateEvent(NULL, FALSE, FALSE, NULL))
{

}

int Event::Wait(int32_t timeout)
{
	return ::WaitForSingleObject(m_handle, timeout);
}

void Event::Activate()
{
	::SetEvent(m_handle);
}

HANDLE Event::GetHandle() const
{
	return m_handle;
}

Event::~Event()
{
	::CloseHandle(m_handle);
}

int Event::WaitMultiple(::std::vector<Event*> events, int timeout)
{
	::std::vector<HANDLE> handles;
	handles.reserve(events.size());
	::std::for_each(events.begin(), events.end(), [&handles](Event* event) {handles.emplace_back(event->GetHandle()); });
	return ::WaitForMultipleObjects(handles.size(), handles.data(), FALSE, timeout);
}
