// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FrontendDebugHelper.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions;
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions); //filling in the array

	//sort the array from highest to lowest available screen resolutions
	AvailableResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)->bool { return A.SizeSquared() < B.SizeSquared(); });
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		//Debug::Print(TEXT("Available resolution: ") + Resolution.ToString()); //output format: Available resolution: X=2560 Y=1440
		AddDynamicOption(ResToValueString(Resolution), ResToDisplayText(Resolution));
	}

	//Use maximum allowed resolution as default value
	MaximumAllowedResolution = ResToValueString(AvailableResolutions.Last());
	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	//mainly for in-editor when using weird resolution
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResToDisplayText(UFrontendGameUserSettings::Get()->GetScreenResolution());
	}

	if (DataDynamicGetter)
	{
		//Debug::Print(TEXT("Resolution value from dynamic getter: ") + DataDynamicGetter->GetValueAsString()); //output format:Resolution value from dynamic getter: (X=3440,Y=1440)
		//DataDynamicGetter->GetValueAsString()
	}
}

FString UListDataObject_StringResolution::ResToValueString(const FIntPoint& InResolution) const
{
	//Resolution value from dynamic getter: (X=3440,Y=1440)
	return FString::Printf(TEXT("(X=%i,Y=%i)"), InResolution.X, InResolution.Y);
}

FText UListDataObject_StringResolution::ResToDisplayText(const FIntPoint& InResolution) const
{
	const FString DisplayString = FString::Printf(TEXT("%i x %i"), InResolution.X, InResolution.Y);
	return FText::FromString(DisplayString);
}
