// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "AsyncActionPushConfirmScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfirmScreenButtonClickedDelegate, EConfirmScreenButtonType, ClickedButtonType);

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UAsyncActionPushConfirmScreen : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", HidePin = "WorldContextObject", BlueprintInternalUseOnly = "true", DisplayName = "Show Confirmation Screen"))
	static UAsyncActionPushConfirmScreen* PushConfirmScreen(
		const UObject* WorldContextObject,
		EConfirmScreenType ScreenType,
		FText InScreenTitle,
		FText InScreenMessage);

	//~Begin UBlueprintAsyncActionBase Interface
	virtual void Activate() override;
	//~End UBlueprintAsyncActionBase Interface

	UPROPERTY(BlueprintAssignable)
	FOnConfirmScreenButtonClickedDelegate OnButtonClicked;

private:
	TWeakObjectPtr<UWorld> CachedOwningWorld;
	EConfirmScreenType CachedScreenType;
	FText CachedScreenTitle;
	FText CachedScreenMessage;
};
