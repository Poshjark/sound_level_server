#include "volume_handler.h"
#include <iostream>
void exit_on_error(HRESULT hr) {
	if (hr != S_OK) {
		CoUninitialize();
		std::cerr << "Error in volumehandler " << hr << std::endl;
		exit(-1);
	}
}

VolumeHandler::VolumeHandler() : audio_device(nullptr), endpoint_volume_interface(nullptr), g_guidMyContext(GUID_NULL) {
#ifndef COM_ACTIVATED
	CoInitialize(NULL);
#define COM_ACTIVATED
#endif // !COM_ACTIVATED

	// Getting device enumerator to get AudioEndpoint
	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID*)&deviceEnumerator);
	exit_on_error(hr);

	//Getting default audio endpoint from enumerator
	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audio_device);
	exit_on_error(hr);

	// Releasing(deallocating) device enumerator. It's no more needed
	deviceEnumerator->Release();
	deviceEnumerator = nullptr;

	// Activating interface of audio endpoint for interaction with sound levels
	hr = audio_device->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&endpoint_volume_interface);
	exit_on_error(hr);

	// Getting guid context to call in the future. It will be one of params.
	GUID g_guidMyContext = GUID_NULL;
	hr = CoCreateGuid(&g_guidMyContext);
	exit_on_error(hr);

	// Just initiliazing/updating volume  with real value
	hr = endpoint_volume_interface->GetMasterVolumeLevel(&volume_level);
	exit_on_error(hr);

	// Initiliazing mute state
	hr = endpoint_volume_interface->GetMute(&muted);
	exit_on_error(hr);
}

const float& VolumeHandler::update_volume_value()
{
	hr = endpoint_volume_interface->GetMasterVolumeLevelScalar(&volume_level);
	exit_on_error(hr);
	return volume_level;
}
const float& VolumeHandler::volume_step_up() {
	hr = endpoint_volume_interface->VolumeStepUp(&g_guidMyContext);
	exit_on_error(hr);
	return update_volume_value();
}

const float& VolumeHandler::volume_step_down() {
	hr = endpoint_volume_interface->VolumeStepDown(&g_guidMyContext);
	exit_on_error(hr);
	return update_volume_value();
}

const float& VolumeHandler::set_volume(int val) {
	if (val >= 0 && val <= 100) {
		float new_val = (float)val / 100.0f;
		hr = endpoint_volume_interface->SetMasterVolumeLevelScalar(new_val, &g_guidMyContext);
		exit_on_error(hr);
	}
	return update_volume_value();
}

const float& VolumeHandler::mute() {
	muted = !muted;
	hr = endpoint_volume_interface->SetMute(muted, &g_guidMyContext);
	exit_on_error(hr);
	return update_volume_value();
}




void VolumeHandler::get_name()
{
	IPropertyStore* pp = nullptr;
	hr = audio_device->OpenPropertyStore(STGM_READ, &pp);
	exit_on_error(hr);

	PROPVARIANT property;
	hr = pp->GetValue(PKEY_DeviceInterface_FriendlyName, &property);
	exit_on_error(hr);

	LPWSTR lpwstr = property.pwszVal;
	size_t buffer_size = wcslen(lpwstr);
	char buffer[256];
	this->name.clear();
	wcstombs_s(&buffer_size, buffer, lpwstr, buffer_size);
	for (auto c : buffer) {
		if (int(c) >= 65 && int(c) <= 126 || c == ' ') {
			name += c;
		}
		else {
			break;
		}

	}

	SAFE_RELEASE(pp);
}



VolumeHandler::~VolumeHandler()
{
	SAFE_RELEASE(audio_device);
	SAFE_RELEASE(endpoint_volume_interface);
}