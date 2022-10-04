#include "abstractframe.h"
#include "framereader.h"

#define STUB	return nullptr

IFrame::Ptr CreateFrame(const void* data)
{
	if (!data)
		return nullptr;
	IFrame::Ptr result;
	uint8_t type_code = *(uint8_t*)data;
	switch (type_code)
	{
		case	FrameType::ValuesRequest:		result.reset(new IncomingFrame()); break;
		case	FrameType::ValuesRespone:		STUB;
		case	FrameType::CommonPollRequest:	STUB;
		case	FrameType::CommonPollRespone:	STUB;
		case	FrameType::Notification:		STUB;

	default:
		STUB;
	}
}