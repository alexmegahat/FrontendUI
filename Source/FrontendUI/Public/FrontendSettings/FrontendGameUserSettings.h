// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "FrontendGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
public:

	UFrontendGameUserSettings(); //default var values are in this constructor
	
	static UFrontendGameUserSettings* Get();

	//******* Gameplay Collection Tab *******//
	//Game Difficulty
	UFUNCTION()
	FString GetCurrentGameDifficulty() const {return CurrentGameDifficulty;}

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewGameDifficulty) {CurrentGameDifficulty = InNewGameDifficulty; }

	//Language
	UFUNCTION()
	FString GetCurrentLanguage() const;

	UFUNCTION()
	void SetCurrentLanguage(const FString& InNewLanguage);
	
	//....
	//******* Gameplay Collection Tab *******//

	//******* Audio Collection Tab *******//
	UFUNCTION()
	float GetOverallVolume() const {return OverallVolume;}

	UFUNCTION()
	void SetOverallVolume(float InVolume);

	UFUNCTION()
	float GetMusicVolume() const {return MusicVolume;}

	UFUNCTION()
	void SetMusicVolume(float InVolume);

	UFUNCTION()
	float GetSoundFXVolume() const {return SoundFXVolume;}

	UFUNCTION()
	void SetSoundFXVolume(float InVolume);

	UFUNCTION()
	bool GetAllowBackgroundAudio() const {return bAllowBackgroundAudio;}

	UFUNCTION()
	void SetAllowBackgroundAudio(bool bIsAllowed);

	UFUNCTION()
	bool GetUseHDRAudio() const {return bUseHDRAudio;}

	UFUNCTION()
	void SetUseHDRAudio(bool bIsAllowed);
	//******* Audio Collection Tab *******//

	//******* Video Collection Tab *******//
	UFUNCTION()
	float GetCurrentDisplayGamma() const;

	UFUNCTION()
	void SetCurrentDisplayGamma(float InNewGamma);
	//******* Video Collection Tab *******//

private:
	//default var values are in constructor
	
	//******* Gameplay Collection Tab *******//
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
	//******* Gameplay Collection Tab *******//
	
	//******* Audio Collection Tab *******//
	UPROPERTY(Config)
	float OverallVolume; //default value = 1

	UPROPERTY(Config)
	float MusicVolume; //default value = 1

	UPROPERTY(Config)
	float SoundFXVolume; //default value = 1

	UPROPERTY(Config)
	bool bAllowBackgroundAudio; //default false

	UPROPERTY(Config)
	bool bUseHDRAudio; //default false
	//******* Audio Collection Tab *******//
};
