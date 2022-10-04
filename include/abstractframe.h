#include <cstdint>
#include <memory>
#include <boost/variant.hpp>
#include <vector>


enum FrameType : uint8_t
{
	ValuesRequest		= 0x01,
	ValuesRespone		= 0x02,
	CommonPollRequest	= 0x03,
	CommonPollRespone	= 0x04,
	Notification		= 0x05
};

enum class ValueType : uint8_t
{
	UINT64_T	=	0x01,
	INT64_T		=	0x02,
	UINT32_T	=	0x03,
	INT32_T		=	0x04,
	UINT16_T	=	0x05,
	INT16_T		=	0x06,
	UINT8_T		=	0x07,
	INT8_T		=	0x08,
	BOOL		=	0x08,
	STRING		=	0x0A,
};

class IFrame
{
public:
	typedef std::shared_ptr<IFrame> Ptr;
	typedef boost::variant<uint64_t, int64_t, uint32_t, int32_t, uint16_t, int16_t, uint8_t, int8_t, bool, std::string> variant;
	virtual void foo() = 0;
	virtual std::vector<variant>  GetData() const = 0;
	virtual void* GetRawData() const = 0;
	virtual uint8_t GetFunctionCode() = 0;
	virtual FrameType GetType() = 0;
	virtual ~IFrame() = 0;

};


IFrame::Ptr CreateFrame(const void* data);



