#include "command_handler.h"
#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3
#define SOUND_LEVEL_LENGTH 3

CommandHandler::CommandHandlerResult::CommandHandlerResult(std::string message, bool _is_ok)
    :
    result_message(message), is_ok(_is_ok)
{
}

const std::string& CommandHandler::CommandHandlerResult::what()
{
    return this->result_message;
}

CommandHandler::CommandHandlerResult CommandHandler::execute_command(char raw_message[],VolumeHandler* volume_handler, size_t message_length) {
    typedef CommandHandler::CommandHandlerResult ch_result_t;
    float volume_level = 0;
    std::string command = "000";
    for (size_t i = 0; i < COMMAND_LENGTH; i++) {
        command[i] = raw_message[i];
    }
    std::cout << "Command received: " << command << std::endl;
    if (command == "vsu") {
        volume_level = volume_handler->volume_step_up();
    }
    else if (command == "vsd") {
        volume_level = volume_handler->volume_step_down();
    }
    else if (command == "vsm") {
        volume_level = volume_handler->mute();
    }
    else if (command == "vsl") {
        try {
            std::string new_sound_level_str;
            for (size_t i = 3; i < COMMAND_LENGTH + SOUND_LEVEL_LENGTH  && std::isdigit(raw_message[i]); i++) {
                new_sound_level_str += raw_message[i];
            }
            int new_sound_level_int = std::stoi(new_sound_level_str);
            std::clog << "New sound level to set: " << new_sound_level_int << std::endl;
            volume_level = volume_handler->set_volume(new_sound_level_int);
        }
        catch (std::exception error) {
            return ch_result_t(error.what(),false);
        }

    }
    else if (command == "gvl") {
        volume_level = volume_handler->update_volume_value();
    }
    std::string muted = volume_handler->muted ? "true" : "false";
    std::string result_message = "Operation " 
        + command 
        +  " succeded. New volume level is " 
        + std::to_string((int)(volume_level*100)) + ". Channel is muted - " 
        + muted + "\n";
	return ch_result_t(result_message,true);
}


