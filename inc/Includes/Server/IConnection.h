#pragma once
#include <cstdint>

struct IConnection
{
	virtual void Write(void* data, ::std::size_t size) = 0;
};