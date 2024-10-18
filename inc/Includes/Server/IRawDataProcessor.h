#pragma once

#include "IRawData.h"
#include "RawData.h"

class IRawDataProcessor
{
public:
	using Ptr = ::boost::shared_ptr<IRawDataProcessor>;
	virtual void Process(IRawData* data) = 0;

	virtual ~IRawDataProcessor() {};

};