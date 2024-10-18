#pragma once

#include <cstdint>
#include <cstddef>

struct IConnectionEventsListener
{
	virtual void OnRead(void* data, ::std::size_t bytesRead) = 0;
	virtual void OnWrite() = 0;
	virtual void OnDisconnected() = 0;
};