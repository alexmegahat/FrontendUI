﻿// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ListDataObject_Value.h"
#include "ListDataObject_String.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_String : public UListDataObject_Value
{
	GENERATED_BODY()
public:
	void AddDynamicOption(const FString& InStringValue, const FText& InDisplayText);

	void AdvanceToNextOption();
	void BackToPreviousOption();

protected:
	//~Begin UListDataObject_Base Interface
	virtual void OnDataObjectInitialized() override;
	//~End UListDataObject_Base Interface

	bool TrySetDisplayTextFromStringValue(const FString& InStringValue);
	
	FString CurrentStringValue;
	FText CurrentDisplayText;

	//each element will have a corresponding element in another array
	TArray<FString> AvailableOptionsStringArray;
	TArray<FText> AvailableOptionsTextArray;

public:
	FORCEINLINE const TArray<FText>& GetAvailableOptionsTextArray() const { return AvailableOptionsTextArray; }
	FORCEINLINE FText GetCurrentDisplayText() const { return CurrentDisplayText; }

};
