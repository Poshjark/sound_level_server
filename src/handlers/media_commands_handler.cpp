#include "media_commands_handler.h"
#include <Windows.h>

namespace mcommands_handler{
void emulate_key(BYTE button_name_byte, BYTE button_code){
	keybd_event(button_name_byte, button_code, KEYEVENTF_EXTENDEDKEY | 0, 0);
	keybd_event(button_name_byte, button_code, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}
}


void MediaCommandsHandler::play_pause_media() {
	mcommands_handler::emulate_key(VK_MEDIA_PLAY_PAUSE, 0xB3);
}


void MediaCommandsHandler::next_media() {
	mcommands_handler::emulate_key(VK_MEDIA_NEXT_TRACK, 0xB0);
}

void MediaCommandsHandler::prev_media() {
	mcommands_handler::emulate_key(VK_MEDIA_PREV_TRACK, 0xB1);
}
