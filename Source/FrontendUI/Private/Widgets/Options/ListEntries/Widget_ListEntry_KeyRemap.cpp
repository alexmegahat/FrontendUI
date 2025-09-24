// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_KeyRemap.h"
#include "FrontendDebugHelper.h"
#include "FrontendFunctionLibrary.h"
#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_KeyRemap.h"
#include "Subsystems/FrontendUISubsystem.h"
#include "Widgets/Options/Widget_KeyRemapScreen.h"
#include "FrontendGameplayTags.h"

void UWidget_ListEntry_KeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UListDataObject_KeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UWidget_ListEntry_KeyRemap::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
                                                                EOptionsListDataModifyReason ModifyReason)
{
	//update button image
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UWidget_ListEntry_KeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();
	
	UFrontendUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		FrontendGameplayTags::Frontend_WidgetStack_Modal,
		UFrontendFunctionLibrary::GetFrontendSoftWidgetClassByTag(FrontendGameplayTags::Frontend_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState PushState, UWidget_ActivatableBase* PushedWidget)
		{
			if (PushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UWidget_KeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UWidget_KeyRemapScreen>(PushedWidget);

				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(this, &ThisClass::OnKeyToRemapPressed);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCancel.BindUObject(this, &ThisClass::OnKeyRemapCanceled);
				
				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(CachedOwningKeyRemapDataObject->GetDesiredInputKeyType());
				}
			}
		}
	);
}

void UWidget_ListEntry_KeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject) return;
	
	//Check if the current key is already the default key. Display an OK screen if this is already a default key.
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmScreenType::Ok,
			FText::FromString(TEXT("Reset Key Mapping")),  //TODO: localization
			FText::FromString(TEXT("The key binding for ") +
				CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() +
				TEXT(" is already set to default.")
			), //TODO: localization
			[](EConfirmScreenButtonType ClickedButton){}
		);

		return;
	}
	
	//Reset key binding back to default
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesNo,
		FText::FromString(TEXT("Reset Key Mapping")),  //TODO: localization
			FText::FromString(TEXT("Are you sure you want to reset key binding for ") +
				CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() +
				TEXT(" ?")
			), //TODO: localization
			[this](EConfirmScreenButtonType ClickedButton)
			{
				if (ClickedButton == EConfirmScreenButtonType::Confirmed)
				{
					CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
				}
			}
	);
}

void UWidget_ListEntry_KeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	//Debug::Print(TEXT("Valid Key to Remap Detected - ") + PressedKey.GetDisplayName().ToString());
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UWidget_ListEntry_KeyRemap::OnKeyRemapCanceled(const FString& CancelReason)
{
	UFrontendUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::Ok,
		FText::FromString("Key Remap"),              //TODO: localization
		FText::FromString(CancelReason),
		[](EConfirmScreenButtonType ClickedButton){}
	);
}
