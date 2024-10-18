#pragma once

#include "Includes/Server/IRawDataProcessor.h"

class RawDataProcessor final : public IRawDataProcessor
{
public:
	RawDataProcessor() = default;
private:
	virtual void Process(IRawData* data) override final
	{

	}
};