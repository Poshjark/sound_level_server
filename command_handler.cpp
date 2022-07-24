#include "command_handler.h"
#include "media_commands_handler.h"

#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3
#define SOUND_LEVEL_LENGTH 3

CommandHandler::CommandHandlerResult::CommandHandlerResult(std::string message, bool _is_ok)
    :
    result_message(message), is_ok(_is_ok)
{
}

CommandHandler::CommandHandler(VolumeHandler* _volume_handler) : volume_handler(_volume_handler) {

}

const std::string& CommandHandler::CommandHandlerResult::what()
{
    return this->result_message;
}

CommandHandler::CommandHandlerResult CommandHandler::execute_command(char raw_message[], size_t message_length) {
    typedef CommandHandler::CommandHandlerResult ch_result_t;
    float volume_level = this->volume_handler->update_volume_value();
    std::string command = "000";
    for (size_t i = 0; i < COMMAND_LENGTH; i++) {
        command[i] = raw_message[i];
    }
    
    std::cout << "Command received: " << command << std::endl;
    if (command == "vsu") {
        volume_level = this->volume_handler->volume_step_up();
    }
    else if (command == "vsd") {
        volume_level = this->volume_handler->volume_step_down();
    }
    else if (command == "vsm") {
        volume_level = this->volume_handler->mute();
    }
    else if (command == "vsl") {
        try {
            std::string new_sound_level_str;
            for (size_t i = 3; i < COMMAND_LENGTH + SOUND_LEVEL_LENGTH  && std::isdigit(raw_message[i]); i++) {
                new_sound_level_str += raw_message[i];
            }
            int new_sound_level_int = std::stoi(new_sound_level_str);
            //std::clog << "New sound level to set: " << new_sound_level_int << std::endl;
            volume_level = this->volume_handler->set_volume(new_sound_level_int);
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

    std::string muted = this->volume_handler->muted ? "true" : "false";
    std::string result_message;
    result_message += "t";
    std::string volume_level_str = std::to_string((int)(volume_level * 100 + 0.5));
    if (volume_level_str.size() < 3) {
        volume_level_str = std::string(3 - volume_level_str.size(), '0') + volume_level_str;
    }
    result_message += volume_level_str;
    result_message += muted[0];
	return ch_result_t(result_message,true);
}