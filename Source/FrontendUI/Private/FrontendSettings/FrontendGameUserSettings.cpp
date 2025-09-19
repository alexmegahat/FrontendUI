// Aleksei Miakishev All Rights Reserved


#include "FrontendSettings/FrontendGameUserSettings.h"

UFrontendGameUserSettings::UFrontendGameUserSettings()
	: OverallVolume(1.f),
	MusicVolume(1.f),
	SoundFXVolume(1.f),
	bAllowBackgroundAudio(false)
{
	
}

UFrontendGameUserSettings* UFrontendGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UFrontendGameUserSettings>(GEngine->GetGameUserSettings());
	}

	return nullptr;
}

void UFrontendGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	//The actual logic for controlling the volume goes here.
	//.....
	
	ApplyNonResolutionSettings();
	SaveSettings();
}

void UFrontendGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume = InVolume;

	//The actual logic for controlling the volume goes here.
	//.....
	
	ApplyNonResolutionSettings();
	SaveSettings();
}

void UFrontendGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;

	//The actual logic for controlling the volume goes here.
	//.....
	
	ApplyNonResolutionSettings();
	SaveSettings();
}

void UFrontendGameUserSettings::SetAllowBackgroundAudio(bool bIsAllowed)
{
	bAllowBackgroundAudio = bIsAllowed;

	//The actual logic for controlling the audio goes here.
	//.....
	
	ApplyNonResolutionSettings();
	SaveSettings();
}
