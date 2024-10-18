#pragma once

#include <cstdint>
#include <vector>

#include "boost/noncopyable.hpp"
#include "Windows.h"


struct Event : boost::noncopyable
{
	Event();

	int Wait(int32_t timeout);

	void Activate();

	HANDLE GetHandle() const;

	~Event();

	static int WaitMultiple(::std::vector<Event*> events, int timeout);

private:

	HANDLE m_handle;
};