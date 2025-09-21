// Aleksei Miakishev All Rights Reserved

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

	void OnRotatorInitiatedValueChange(const FText& InNewSelectedText);

protected:
	//~Begin UListDataObject_Base Interface
	virtual void OnDataObjectInitialized() override;
	
	virtual bool CanResetBackToDefaultValue() const override;
	virtual bool TryResetBackToDefaultValue() override;

	virtual bool CanSetToForcedStringValue(const FString& InForcedValue) const override;
	virtual void OnSetToForcedStringValue(const FString& InForcedValue) override;
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

//Child StringBool class - works for any boolean values
UCLASS()
class FRONTENDUI_API UListDataObject_StringBool : public UListDataObject_String
{
	GENERATED_BODY()
public:
	void OverrideTrueDisplayText(const FText& InNewTrueDisplayText);
	void OverrideFalseDisplayText(const FText& InNewFalseDisplayText);

	void SetTrueAsDefaultValue();
	void SetFalseAsDefaultValue();

	bool GetCurrentValueAsBool() const {return CurrentStringValue == TrueString; };

protected:
	//~Begin UListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;
	//~End UListDataObject_String Interface
private:
	void TryInitBoolValues();
	
	const FString TrueString = TEXT("true");
	const FString FalseString = TEXT("false");
};

//

//Child StringEnum class - works for any enum values (use case: Window Mode)
UCLASS()
class FRONTENDUI_API UListDataObject_StringEnum : public UListDataObject_String
{
	GENERATED_BODY()
public:
	template<typename EnumType>
	void AddEnumOption(EnumType InEnumOption, const FText& InDisplayText)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		//use func to convert enum to string
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		AddDynamicOption(ConvertedEnumString, InDisplayText);
	}

	template<typename EnumType>
	EnumType GetCurrentValueAsEnum() const
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		//use func to convert string to enum
		return (EnumType)StaticEnumOption->GetValueByNameString(CurrentStringValue); //c-style cast
	}

	template<typename EnumType>
	void SetDefaultValueFromEnumOption(EnumType InEnumOption)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();
		//use func to convert enum to string
		const FString ConvertedEnumString = StaticEnumOption->GetNameStringByValue(InEnumOption);

		SetDefaultValueFromString(ConvertedEnumString);
	}
};

//Child StringInteger class - for settings like game scalability (graphics)
UCLASS()
class FRONTENDUI_API UListDataObject_StringInteger : public UListDataObject_String
{
	GENERATED_BODY()
public:
	void AddIntegerOption(const int32& InIntegerValue, const FText& InDisplayText);
	
protected:
	//~Begin UListDataObject_String Interface
	virtual void OnDataObjectInitialized() override;
	virtual void OnEditDependencyDataModified(UListDataObject_Base* ModifiedDependencyData, EOptionsListDataModifyReason ModifyReason) override;
	//~End UListDataObject_String Interface
};

