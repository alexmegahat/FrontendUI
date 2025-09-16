// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/ListEntries/Widget_ListEntry_String.h"

#include "Widgets/Components/FrontendCommonButtonBase.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Components/Frontend_CommonRotator.h"

#include "FrontendDebugHelper.h"

void UWidget_ListEntry_String::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_PreviousOption->OnClicked().AddUObject(this, &ThisClass::OnPreviousOptionButtonClicked);
	CommonButton_NextOption->OnClicked().AddUObject(this, &ThisClass::OnNextOptionButtonClicked);

	//every time common rotator is clicked -> select this entry widget
	CommonRotator_AvailableOptions->OnClicked().AddLambda([this]() {SelectThisEntryWidget();});
}

void UWidget_ListEntry_String::OnOwningListDataObjectSet(UListDataObject_Base* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningStringDataObject = CastChecked<UListDataObject_String>(InOwningListDataObject);

	CommonRotator_AvailableOptions->PopulateTextLabels(CachedOwningStringDataObject->GetAvailableOptionsTextArray());
	CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
}

void UWidget_ListEntry_String::OnOwningListDataObjectModified(UListDataObject_Base* OwningModifiedData,
	EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningStringDataObject)
	{
		CommonRotator_AvailableOptions->SetSelectedOptionByText(CachedOwningStringDataObject->GetCurrentDisplayText());
	}
}

void UWidget_ListEntry_String::OnPreviousOptionButtonClicked()
{
	//Debug::Print("UWidget_ListEntry_String::OnPreviousOptionButtonClicked");
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->BackToPreviousOption();
	}
	SelectThisEntryWidget();
}

void UWidget_ListEntry_String::OnNextOptionButtonClicked()
{
	//Debug::Print("UWidget_ListEntry_String::OnNextOptionButtonClicked");
	if (CachedOwningStringDataObject)
	{
		CachedOwningStringDataObject->AdvanceToNextOption();
	}
	SelectThisEntryWidget();
}
