#include "stdafx.h"

#include "RawData.h"

RawData::RawData(void* data, ::std::uint64_t size)
	: _Buf(nullptr)
	, _Size(size)
{
	if (!data || (data && size == 0))
		throw ::std::logic_error("Ошибка при инициализации буфера");

	::std::memcpy(_Buf, data, size);

	assert(_Buf);
}

void* RawData::Data() const
{
	return _Buf;
}

::std::uint64_t RawData::Size() const
{
	return _Size;
}

RawData::~RawData()
{
	if (_Buf)
		delete _Buf;
}

void* RawData::GetData(::std::uint64_t& size)
{
	size = Size();
	return Data();
}