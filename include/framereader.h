#include "abstractframe.h"

class IncomingFrame : public IFrame
{
public:
	virtual void foo() override;
	virtual std::vector<variant>  GetData() const override;
	virtual uint8_t GetFunctionCode() override;
	virtual FrameType GetType() override;
	virtual void* GetRawData() const;

	uint32_t Feed(const void* data);


private:
	void _ParseData(const void* rawData);


	std::vector<variant> _data;

};