// Aleksei Miakishev All Rights Reserved


#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"

TSubclassOf<UUserWidget> UFrontendLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("Forgot to assign valid widget bp in the project settings as loading screen."));

	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();

	return LoadedLoadingScreenWidget;
}
