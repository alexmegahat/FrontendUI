// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "FrontendLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, DefaultConfig)
class FRONTENDUI_API UFrontendLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	//use this function to load in the soft widget reference to display it in the viewport
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldLoadingScreenExtraSeconds = 3.f;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShowLoadingScreenInEditor = false;
};
