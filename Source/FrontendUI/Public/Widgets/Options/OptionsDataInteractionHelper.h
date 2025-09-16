// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "PropertyPathHelpers.h"

class UFrontendGameUserSettings;

/**
 * Helper class for interacting with Game User Settings
 * FClassName - F being put before class's name - is Unreal Engine naming convention for classes not exposed to BP
 */
class FRONTENDUI_API FOptionsDataInteractionHelper
{
public:
	//constructor
	FOptionsDataInteractionHelper(const FString& InSetterOrGetterFuncPath);

	FString GetValueAsString() const;
	void SetValueAsString(const FString& InStringValue);

private:
	FCachedPropertyPath CachedDynamicFunctionPath;
	TWeakObjectPtr<UFrontendGameUserSettings> CachedWeakGameUserSettings;
};
