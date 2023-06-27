#include "command_handler.h"
#include "media_commands_handler.h"
#include "Command.h"

#include <boost/make_shared.hpp>

#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3
#define SOUND_LEVEL_LENGTH 3

CommandHandler::Result::Result(std::string message, bool _is_ok)
    :
    m_resultMessage(message), is_ok(_is_ok)
{
}

CommandHandler::CommandHandler() 
    : m_pVolumeHandler(VolumeHandler::Create()) 
{

}

CommandHandler::Ptr CommandHandler::Create()
{
    return Ptr(new CommandHandler());
}

const std::string& CommandHandler::Result::What()
{
    return m_resultMessage;
}

CommandHandler::Result CommandHandler::ExecuteRawCommand(char raw_message[], size_t message_length) 
{
    constexpr static uint32_t minimumMessageLength = 3u;
    constexpr static uint32_t minimumMessageLengthWithData = minimumMessageLength + sizeof(Command::data_t);

    using ch_result_t = CommandHandler::Result ;

    if (message_length < 3)
    {
        return ch_result_t{ "bad command", false };
    }

    auto command = Command{ *(uint8_t*)raw_message };

    if (command.RequiresData())
    {
        if(message_length < minimumMessageLengthWithData)
            return ch_result_t{ "This command requires data to execute", false };

        command.SetData(*(Command::data_t*)(void*)(raw_message + minimumMessageLength));
    }

    return _ExecuteCommand(command);

}

CommandHandler::Result CommandHandler::_ExecuteCommand(const Command& command)
{

    // TODO::Invalid codes!
    using ch_result_t = CommandHandler::Result;

    std::string m_resultMessage;
    try
    {
        switch (command.GetCode())
        {
        case Command::Code::VolumeStepUp:
            m_pVolumeHandler->volume_step_up();
            break;
        case Command::Code::VolumeStepDown:
            m_pVolumeHandler->volume_step_down();
            break;
        case Command::Code::VolumeSetMute:
            m_pVolumeHandler->mute();
            break;
        case Command::Code::VolumeSetLevel:
            m_pVolumeHandler->set_volume(command.GetData());
            break;
        case Command::Code::MediaPlayPause:
            MediaCommandsHandler::play_pause_media();
            break;
        case Command::Code::MediaNext:
            MediaCommandsHandler::next_media();
            break;
        case Command::Code::MediaPrev:
            MediaCommandsHandler::prev_media();
            break;
            // We are executing this command by default
        case Command::Code::VolumeGetLevel: [[fallthrough]]
        case Command::Code::Unknown: [[fallthrough]]
        default:
            break;
        }
        const auto m_volumeLevel = m_pVolumeHandler->update_volume_value();
    
   
 
        m_resultMessage += "t";
        std::string volume_level_str = std::to_string((int)(m_volumeLevel * 100 + 0.5));

        if (volume_level_str.size() < 3) 
            volume_level_str = std::string(3 - volume_level_str.size(), '0') + volume_level_str;
    
        m_resultMessage += volume_level_str;
        m_resultMessage += m_pVolumeHandler->IsMuted() ? 't' : 'f';

    }
    catch (const std::exception& e)
    {
        return ch_result_t(e.what(), false);
    }
    return ch_result_t(m_resultMessage, true);
}

CommandHandler::Result::operator bool() const
{
    return is_ok; 
}