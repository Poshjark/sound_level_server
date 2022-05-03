#include "command_handler.h"


int CommandHandler::execute_command(std::string command) {
    if (command == "vsu") {
        volume_handler->volume_step_up();
    }
    else if (command == "vsd") {
        volume_handler->volume_step_down();
    }
    else if (command == "vsm") {
        volume_handler->mute();
    }
    else if (command == "vsl") {
        try {
            std::string new_sound_level_str("   ");
            for (size_t i = 3, j = 0; i < 7 && std::isdigit(my_data_message[i]); i++, j++) {
                new_sound_level_str[j] = my_data_message[i];
            }
            int new_sound_level_int = std::stoi(new_sound_level_str);
            std::clog << "New sound level to set: " << new_sound_level_int << std::endl;
            volume_handler->set_volume(new_sound_level_int);
        }
        catch (std::exception error) {
            std::cerr << "error: " << error.what() << std::endl;
            sock.close();
        }

    }
	return 0;
}