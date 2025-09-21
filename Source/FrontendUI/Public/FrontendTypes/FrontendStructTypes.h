#pragma once

#include "FrontendStructTypes.generated.h"

USTRUCT()
struct FOptionsDataEditConditionDescriptor
{
	GENERATED_BODY()
public:
	void SetEditConditionFunc(TFunction<bool()> InEditConditionFunc)
	{
		EditConditionFunc = InEditConditionFunc;
	};

	bool IsValid() const
	{
		return EditConditionFunc != nullptr;
	}

	bool IsEditConditionMet() const
	{
		if (IsValid())
		{
			//if the edit condition is not met, then the setting should not be editable,
			return EditConditionFunc();
		}
		//but editing should be allowed by default
		return true;
	}

	FString GetDisabledRichReason() const
	{
		return DisabledRichReason;
	};
	
	void SetDisabledRichReason(const FString& InNewDisabledRichReason)
	{
		DisabledRichReason = InNewDisabledRichReason;
	};
	
	bool HasForcedStringValue() const
	{
		return DisabledForcedStringValue.IsSet();
	}

	FString GetDisabledForcedStringValue() const
	{
		return DisabledForcedStringValue.GetValue();
	}

	void SetDisabledForcedStringValue(const FString& InNewDisabledForcedStringValue)
	{
		DisabledForcedStringValue = InNewDisabledForcedStringValue;
	}
	
private:
	TFunction<bool()> EditConditionFunc;
	FString DisabledRichReason;
	TOptional<FString> DisabledForcedStringValue;
};
