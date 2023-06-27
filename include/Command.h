#pragma once
#include <cstdint>
#include <string>

class Command
{
public:

	using data_t = int32_t;

	enum class Code : uint8_t
	{
		VolumeStepUp	=	0x00,
		VolumeStepDown	=	0x01,
		VolumeSetMute	=	0x02,
		VolumeSetLevel	=	0x03, 
		VolumeGetLevel	=	0x04,
		MediaPlayPause	=	0x05,
		MediaNext		=	0x06,
		MediaPrev		=	0x07,
		Unknown			=	0xAA
	};

	static Code CodeFromByte(const uint8_t& code)
	{
		if (code > static_cast<uint8_t>(Code::Unknown))
			return Code::Unknown;

		return static_cast<Command::Code>(code);
	}

	Command(const uint8_t& code)
		: Command(CodeFromByte(code))
	{

	}

	Command(const uint8_t& code, const void* data)
		: Command(CodeFromByte(code), data)
	{

	}

	Command(const Code& code) 
		: m_code(code)
		, m_data(0)
	{

	}

	Command(const Code& code, uint64_t data)
		: m_code(code)
		, m_data(0)
	{

	}


	Command(const Code& code, const void* data)
		: Command(code, *(data_t*)data)
	{

	}

	void SetData(data_t data)
	{
		m_data = data;
	}

	const Code& GetCode() const noexcept
	{
		return m_code;
	}

	const data_t GetData() const noexcept
	{
		return m_data;
	}

	bool RequiresData() const noexcept
	{
		return m_code == Code::VolumeSetLevel;
	}
private:
	Code		m_code;
	data_t	m_data;
};