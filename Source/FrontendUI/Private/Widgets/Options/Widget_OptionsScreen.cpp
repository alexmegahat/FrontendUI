// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/Widget_OptionsScreen.h"
#include "Widgets/Options/Widget_OptionsScreen.h"
#include "Widgets/Options/OptionsDataRegistry.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "FrontendDebugHelper.h"
#include "Widgets/Components/Frontend_TabListWidgetBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"

void UWidget_OptionsScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
			ResetAction,
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}
	
	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
	
	TabListWidget_OptionsTab->OnTabSelected.AddUniqueDynamic(this, &ThisClass::OnOptionsTabSelected);
}

void UWidget_OptionsScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
	for (UListDataObject_Collection* TabCollection : GetOrCreateOwningDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection) continue;

		const FName TabID = TabCollection->GetDataID();
		// if this button was created before do not attempt to create again
		if (TabListWidget_OptionsTab->GetTabButtonBaseByID(TabID) != nullptr) continue;

		TabListWidget_OptionsTab->RequestRegisterTab(TabID, TabCollection->GetDataDisplayName());
	}
	
}

UOptionsDataRegistry* UWidget_OptionsScreen::GetOrCreateOwningDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegistry(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("Data registry for options screen is not valid."));
	
	return CreatedOwningDataRegistry;
}

void UWidget_OptionsScreen::OnResetBoundActionTriggered()
{
	Debug::Print("Reset Bound Action Triggered");
}

void UWidget_OptionsScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}

void UWidget_OptionsScreen::OnOptionsTabSelected(FName TabID)
{
	Debug::Print("New tab selected. Tab ID: " + TabID.ToString());
}
