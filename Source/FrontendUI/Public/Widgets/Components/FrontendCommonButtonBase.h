// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "FrontendCommonButtonBase.generated.h"

class UCommonTextBlock;
class UCommonLazyImage;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UFrontendCommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	//Use this function to set CommonLazyImage_ButtonImage.
	//The reason for using FSlateBrush to set the Common Lazy Image and not a Texture2D is
	//because later when we retrieve the bound keys icon for Input Key Mapping (KeyRemap),
	//FSlateBrush is the type that we're going to get
	UFUNCTION(BlueprintCallable)
	void SetButtonImage(const FSlateBrush& InBrush);
	
private:
	//~Begin UUserWidget Interface//
	virtual void NativePreConstruct() override;
	//~End UUserWidget Interface//

	//~Begin UCommonButtonBase Interface//
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~End UCommonButtonBase Interface//
	
	//**** Bound Widgets ****//
	
	//BindWidgetOptional allows to make optional display text or image, because not every button in the UI needs to have either of these.

	//Optional text
	UPROPERTY(meta=(BindWidgetOptional))
	UCommonTextBlock* CommonTextBlock_ButtonText;

	//We need to use function SetButtonImage in bp to set the image.
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	
	//**** Bound Widgets ****//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	bool bUseUpperCaseForButtonText = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Frontend Button", meta = (AllowPrivateAccess = "true"))
	FText ButtonDescriptionText;
};
