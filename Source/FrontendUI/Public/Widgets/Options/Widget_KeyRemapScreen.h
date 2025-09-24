// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "CommonInputTypeEnum.h"
#include "Widget_KeyRemapScreen.generated.h"

class UCommonRichTextBlock;
class FKeyRemapScreenInputPreprocessor;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_KeyRemapScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
	
public:
	void SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType);

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnKeyRemapScreenKeyPressedDelegate OnKeyRemapScreenKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnKeyRemapScreenKeySelectCancelDelegate, const FString& /*CanceledReason*/)
	FOnKeyRemapScreenKeySelectCancelDelegate OnKeyRemapScreenKeySelectCancel;

protected:
	//~Begin UCommonActivatableWidget Interface//
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~End UCommonActivatableWidget Interface//
	
private:
	void OnValidKeyPressedDetected(const FKey& PressedKey);
	void OnKeySelectCancelled(const FString& CanceledReason);

	//Delay a tick to make sure the input key is captured properly before calling the PreDeactivateCallback and deactivating the widget.
	void RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback);
	
	//****** Bound Widgets *******//
	UPROPERTY(meta=(BindWidget))
	UCommonRichTextBlock* CommonRichText_RemapMessage;
	//****** Bound Widgets *******//

	TSharedPtr<FKeyRemapScreenInputPreprocessor> CachedInputPreprocessor;

	ECommonInputType CachedDesiredInputType;
};
