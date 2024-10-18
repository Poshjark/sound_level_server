#pragma once

#include <cstdint>
#include "boost/noncopyable.hpp"

#include "Includes/Server/IRawData.h"

class RawData final : boost::noncopyable, public IRawData
{
public:
	RawData(void* data, ::std::uint64_t size);

	void* Data() const;

	::std::uint64_t Size() const;

	virtual void* GetData(::std::uint64_t& size) override final;

	~RawData();

private:
	void* _Buf;
	::std::uint64_t _Size;
};
