// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "FrontendTypes/FrontendEnumTypes.h"
#include "Widget_ListEntry_Base.generated.h"

class UCommonTextBlock;
class UListDataObject_Base;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_ListEntry_Base : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName = "On List Entry Widget Hovered"))
	void BP_OnListEntryWidgetHovered(bool bWasHovered, bool bIsEntryWidgetStillSelected);
	void NativeOnListEntryWidgetHovered(bool bWasHovered);
	
protected:
	//The child widget bp should override this for gamepad interaction to function properly.
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Get Widget to focus for Gamepad"))
	UWidget* BP_GetWidgetToFocusForGamepad() const;
	
	
	//~Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	//gets called when this entry widget is released, and when we switch to another Tab (then all the entry widgets will be released)
	virtual void NativeOnEntryReleased() override;
	//~End IUserObjectListEntry Interface

	//~Begin UUserWidget Interface
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	//~End UUserWidget Interface
	
	//The child class should override this function to handle the initialization needed. Super call is expected in the child class.
	virtual void OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject);

	//The child class should override this function to update its UI values after the data object has been modified. Super call is not needed. 
	virtual void OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData, EOptionsListDataModifyReason ModifyReason);

	//more like force select
	void SelectThisEntryWidget();
private:
	//****** Bound Widgets *******//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional, AllowPrivateAccess = "true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//****** Bound Widgets *******//
};
