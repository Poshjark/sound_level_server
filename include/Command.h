#pragma once

#include <cstdint>
#include <string>
#include <array>
#include <unordered_map>

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

	using serialized_t = std::array<uint8_t, sizeof(Code) + sizeof(data_t)>;

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


	serialized_t Serialize() const
	{
		serialized_t result;

		result[0] = static_cast<uint8_t>(m_code);

		*((uint8_t*)result.data() + sizeof(m_code)) = m_data;

		return result;
	}

private:
	Code		m_code;
	data_t	m_data;
};


const std::unordered_map<uint8_t, std::string> commandDescriptions
{
	{(uint8_t)Command::Code::VolumeStepUp		, "Volume step up"},
	{(uint8_t)Command::Code::VolumeStepDown		, "Volume step down"},
	{(uint8_t)Command::Code::VolumeSetMute		, "Switch volume mute state(if muted - turns mute off and vice versa"},
	{(uint8_t)Command::Code::VolumeSetLevel		, "Volume set level. Requires additional 4-byte unsigned integer of volume level 0 to 100 (percents)"},
	{(uint8_t)Command::Code::VolumeGetLevel		, "Volume get level - returns current volume level"},
	{(uint8_t)Command::Code::MediaPlayPause		, "Switch media play state. Play/pause"},
	{(uint8_t)Command::Code::MediaNext			, "Play next media if it's availiable"},
	{(uint8_t)Command::Code::MediaPrev			, "Play previous media if it's availiable"}

};
