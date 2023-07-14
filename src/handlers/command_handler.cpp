#include "command_handler.h"
#include "media_commands_handler.h"
#include "Command.h"

#include <boost/make_shared.hpp>



static const std::string& GetHelpMessage()
{
    static const std::string helpMessage = 
        "Command must contain at least 1 byte - command code\n"\
        "0x00 - Volume step up\n"
        "0x01 - Volume step down\n"
        "0x02 - Switch volume mute state(if muted - turns mute off and vice versa\n"
        "0x03 - Volume set level. Requires additional 4-byte unsigned integer of volume level 0 to 100 (percents)\n"
        "0x04 - Volume get level - returns current volume level\n"
        "0x05 - Switch media play state. Play/pause\n"
        "0x06 - Play next media if it's availiable\n"
        "0x07 - Play previous media if it's availiable\n";

    return helpMessage;
}


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
    // mimimum message length is length of command code - one byte
    constexpr static uint32_t minimumMessageLength = 1u;

    // for messages with data min length advances by data length (4 bytes) and equals 1 + 4 = 5 bytes
    constexpr static uint32_t minimumMessageLengthWithData = minimumMessageLength + sizeof(Command::data_t);

    using ch_result_t = CommandHandler::Result ;

    if (message_length < 1)
        return ch_result_t{ "bad command", false };
    
    std::cout << "Command received:\t";

    if (message_length >= 4 && std::string(raw_message, 4) == "help")
    {
        std::cout << "Help\n";
        return ch_result_t{ GetHelpMessage(), true };
    }
        
  


    auto command = Command{ *(uint8_t*)raw_message };

    // Какой-то костыль
    std::cout << commandDescriptions.at((uint8_t)command.GetCode()) << std::endl;

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