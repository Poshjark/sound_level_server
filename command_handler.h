#pragma once
#include <string>
#include "volume_handler.h"
#include <iostream>


#define COMMAND_HANDLER_ERROR_CODE 1
#define COMMAND_HANDLER_OK_CODE 0
#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3


class CommandHandler {
public:
	class CommandHandlerResult {
	private:
		std::string result_message;
		bool is_ok;
	public:
		CommandHandlerResult(std::string message, bool _is_ok);
		operator bool() const { return is_ok; };
		const std::string& what();
	};
	static CommandHandlerResult execute_command(char arr[], VolumeHandler*, size_t message_length);
	
};