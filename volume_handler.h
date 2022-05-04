#pragma once
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <string>
#include <stdlib.h>
#include <endpointvolume.h>
#include <Windows.h>
#include <stdio.h>

#define DEBUG


void exit_on_error(HRESULT);


#define SAFE_RELEASE(pp)  \
              if ((pp) != NULL)  \
                { (pp)->Release(); (pp) = NULL; }

class VolumeHandler {


#ifdef DEBUG
public:
#endif

	std::string name;
	HRESULT hr = S_OK;
	IMMDevice* audio_device;
	IAudioEndpointVolume* endpoint_volume_interface;
	GUID g_guidMyContext;
	float volume_level;
	const float& update_volume_value();
	void get_name();
public:
	BOOL muted;
	VolumeHandler();
	float volume_step_up();
	float volume_step_down();
	float set_volume(int);
	float mute();

	~VolumeHandler();


};