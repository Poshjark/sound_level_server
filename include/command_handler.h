#pragma once
#include <string>
#include "volume_handler.h"

#include <iostream>


#define COMMAND_HANDLER_ERROR_CODE 1
#define COMMAND_HANDLER_OK_CODE 0
#define MESSAGE_MAX_LENGTH 1024
#define COMMAND_LENGTH 3


class CommandHandler 
{
public:
	
	using Ptr = boost::shared_ptr<CommandHandler>;

	CommandHandler();
	static CommandHandler::Ptr Create();

	class Result 
	{

	public:

		Result(std::string message, bool _is_ok);
		operator bool() const;
		const std::string& What();
	private:
		std::string m_resultMessage;
		std::string response;
		bool is_ok;
	};
	Result ExecuteCommand(char arr[], size_t message_length);
private:

	VolumeHandler::Ptr m_pVolumeHandler;
};