#pragma once

#include <cstdint>

class IRawData
{
public:
	virtual void* GetData(::std::uint64_t& size) = 0;
};