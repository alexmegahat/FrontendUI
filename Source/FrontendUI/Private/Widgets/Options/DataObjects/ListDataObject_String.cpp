// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/DataObjects/ListDataObject_String.h"

#include "FrontendDebugHelper.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"

void UListDataObject_String::AddDynamicOption(const FString& InStringValue, const FText& InDisplayText)
{
	AvailableOptionsStringArray.Add(InStringValue);
	AvailableOptionsTextArray.Add(InDisplayText);
}

void UListDataObject_String::AdvanceToNextOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty()) return;
	
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 NextIndexToDisplay = CurrentDisplayIndex + 1;

	const bool bIsNextIndexValid = AvailableOptionsStringArray.IsValidIndex(NextIndexToDisplay);
	if (bIsNextIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[NextIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	//notify Game User Settings that we want to save this value
	if (DataDynamicSetter)
	{
		Debug::Print(TEXT("DataDynamicSetter is used. The latest value from Getter: ") + DataDynamicGetter->GetValueAsString());
		
		DataDynamicSetter->SetValueAsString(CurrentStringValue);
		
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::BackToPreviousOption()
{
	if (AvailableOptionsStringArray.IsEmpty() || AvailableOptionsTextArray.IsEmpty()) return;
	
	const int32 CurrentDisplayIndex = AvailableOptionsStringArray.IndexOfByKey(CurrentStringValue);
	const int32 PrevIndexToDisplay = CurrentDisplayIndex - 1;

	const bool bIsPrevIndexValid = AvailableOptionsStringArray.IsValidIndex(PrevIndexToDisplay);
	if (bIsPrevIndexValid)
	{
		CurrentStringValue = AvailableOptionsStringArray[PrevIndexToDisplay];
	}
	else
	{
		CurrentStringValue = AvailableOptionsStringArray.Last();
	}
	
	TrySetDisplayTextFromStringValue(CurrentStringValue);

	//notify Game User Settings that we want to save this value
	if (DataDynamicSetter)
	{
		DataDynamicSetter->SetValueAsString(CurrentStringValue);

		Debug::Print(TEXT("DataDynamicSetter is used. The latest value from Getter: ") + DataDynamicGetter->GetValueAsString());
		
		NotifyListDataModified(this);
	}
}

void UListDataObject_String::OnDataObjectInitialized()
{
	if (!AvailableOptionsStringArray.IsEmpty())
	{
		CurrentStringValue = AvailableOptionsStringArray[0];
	}

	//TODO:Read from the saved string value and use it to set current string value
	if (DataDynamicGetter)
	{
		if (!DataDynamicGetter->GetValueAsString().IsEmpty())
		{
			CurrentStringValue = DataDynamicGetter->GetValueAsString();
		}
	}

	// if could not find display text
	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = FText::FromString(TEXT("Invalid Option"));
	}
	
}

bool UListDataObject_String::TrySetDisplayTextFromStringValue(const FString& InStringValue)
{
	//IndexOfByKey will work for FString but will not work for FText because FText doesnt overload the == operator
	const int32 CurrentFoundIntex = AvailableOptionsStringArray.IndexOfByKey(InStringValue);
	if (AvailableOptionsTextArray.IsValidIndex(CurrentFoundIntex))
	{
		CurrentDisplayText = AvailableOptionsTextArray[CurrentFoundIntex];
		return true;
	}
	return false;
}
