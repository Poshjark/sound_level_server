#pragma once
#include <mmdeviceapi.h>
#include <Functiondiscoverykeys_devpkey.h>
#include <string>
#include <stdlib.h>
#include <endpointvolume.h>
#include <Windows.h>
#include <stdio.h>
#include <boost/shared_ptr.hpp>


#define DEBUG


void exit_on_error(HRESULT);




class VolumeHandler 
{
public:
	using Ptr = boost::shared_ptr<VolumeHandler>;

public:

	bool IsMuted();

	static VolumeHandler::Ptr Create();



	const float& volume_step_up();
	const float& volume_step_down();
	const float& set_volume(int);
	const float& mute();

	~VolumeHandler();
private:
#ifdef DEBUG
public:
#endif
	void get_name();
	const float& update_volume_value();

	VolumeHandler();

	std::string name;
	HRESULT hr = S_OK;
	IMMDevice* audio_device;
	IAudioEndpointVolume* endpoint_volume_interface;
	GUID g_guidMyContext;
	float m_volumeLevel;

	// Winapi хочет именно BOOL
	BOOL m_muted;

};