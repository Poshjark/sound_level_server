#include "command_handler.h"
#include "media_commands_handler.h"

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
    return boost::make_shared<CommandHandler>();
}

const std::string& CommandHandler::Result::What()
{
    return m_resultMessage;
}

CommandHandler::Result CommandHandler::ExecuteCommand(char raw_message[], size_t message_length) 
{
    typedef CommandHandler::Result ch_result_t;
    float m_volumeLevel = this->m_pVolumeHandler->update_volume_value();
    std::string command = "000";
    for (size_t i = 0; i < COMMAND_LENGTH; i++) {
        command[i] = raw_message[i];
    }
    
    std::cout << "Command received: " << command << std::endl;
    if (command == "vsu") {
        m_volumeLevel = this->m_pVolumeHandler->volume_step_up();
    }
    else if (command == "vsd") {
        m_volumeLevel = this->m_pVolumeHandler->volume_step_down();
    }
    else if (command == "vsm") {
        m_volumeLevel = this->m_pVolumeHandler->mute();
    }
    else if (command == "vsl") {
        try {
            std::string new_sound_level_str;
            for (size_t i = 3; i < COMMAND_LENGTH + SOUND_LEVEL_LENGTH  && std::isdigit(raw_message[i]); i++) {
                new_sound_level_str += raw_message[i];
            }
            int new_sound_level_int = std::stoi(new_sound_level_str);
            //std::clog << "New sound level to set: " << new_sound_level_int << std::endl;
            m_volumeLevel = this->m_pVolumeHandler->set_volume(new_sound_level_int);
        }
        catch (std::exception error) {
            return ch_result_t(error.what(),false);
        }

    }
    else if (command == "gvl") {
    }
    else if (command == "mpp") {
        MediaCommandsHandler::play_pause_media();
    }
    else if (command == "mnx"){
        MediaCommandsHandler::next_media();
    }
    else if (command == "mpr"){
        MediaCommandsHandler::prev_media(); 
    }
    else{
        
    }

    std::string m_muted = m_pVolumeHandler->m_muted ? "true" : "false";
    std::string m_resultMessage;
    m_resultMessage += "t";
    std::string volume_level_str = std::to_string((int)(m_volumeLevel * 100 + 0.5));
    if (volume_level_str.size() < 3) {
        volume_level_str = std::string(3 - volume_level_str.size(), '0') + volume_level_str;
    }
    m_resultMessage += volume_level_str;
    m_resultMessage += m_muted[0];
	return ch_result_t(m_resultMessage,true);
}

CommandHandler::Result::operator bool() const
{
    return is_ok; 
}