// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Widget_ActivatableBase.h"
#include "Widget_OptionsScreen.generated.h"

class UOptionsDataRegistry;
class UFrontend_TabListWidgetBase;
class UFrontendCommonListView;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class FRONTENDUI_API UWidget_OptionsScreen : public UWidget_ActivatableBase
{
	GENERATED_BODY()
protected:
	//~Begin UUserWidget Interface//
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Interface//

	//~Begin UCommonActivatableWidget Interface//
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~End UCommonActivatableWidget Interface//
	
private:
	//Handle the creation of data in the options screen. Direct access to this variable is forbidden.
	//Because of the nature of Common UI, when our options screen remains inactive,
	//this owning data registry can be deleted. ->
	//So we should never interact with this variable directly inside our options screen.
	//We should only use function GetOrCreateOwningDataRegistry.
	UPROPERTY(Transient)
	UOptionsDataRegistry* CreatedOwningDataRegistry;
	UOptionsDataRegistry* GetOrCreateOwningDataRegistry();
	
	void OnResetBoundActionTriggered();
	void OnBackBoundActionTriggered();

	UFUNCTION()
	void OnOptionsTabSelected(FName TabID);

	void OnListViewItemHovered(UObject* InHoveredItem, bool bWasHovered);
	void OnListViewItemSelected(UObject* InSelectedItem);

	//****** Bound Widgets *******//
	UPROPERTY(meta = (BindWidget))
	UFrontend_TabListWidgetBase* TabListWidget_OptionsTab;

	UPROPERTY(meta = (BindWidget))
	UFrontendCommonListView* CommonListView_OptionsList;
	//****** Bound Widgets *******//
	
	UPROPERTY(EditDefaultsOnly, Category = "Frontend Options Screen", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle ResetAction;

	FUIActionBindingHandle ResetActionHandle;
};
