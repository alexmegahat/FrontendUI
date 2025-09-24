// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/Widget_ListEntry_Base.h"
#include "Widget_ListEntry_KeyRemap.generated.h"

class UFrontendCommonButtonBase;
class UListDataObject_KeyRemap;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_ListEntry_KeyRemap : public UWidget_ListEntry_Base
{
	GENERATED_BODY()
protected:
	//~Begin UUserWidget Interface
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Interface
	
	//~Begin UWidget_ListEntry_Base Interface
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject) override;
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason) override;
	//~End UWidget_ListEntry_Base Interface
	
private:
	//list entry buttons
	void OnRemapKeyButtonClicked();
	void OnResetKeyBindingButtonClicked();

	//callbacks bound to the delegates inside CreatedKeyRemapScreen
	void OnKeyToRemapPressed(const FKey& PressedKey);
	void OnKeyRemapCanceled(const FString& CancelReason);
	
	//****** Bound Widgets *******//
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonButtonBase* CommonButton_RemapKey;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UFrontendCommonButtonBase* CommonButton_ResetKeyBinding;

	//****** Bound Widgets *******//
	
	UPROPERTY(Transient)
	UListDataObject_KeyRemap* CachedOwningKeyRemapDataObject;
};
