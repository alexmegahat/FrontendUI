// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FrontendLoadingScreenInterface.generated.h"

// This class does not need to be modified.
//BlueprintType - allow bp to implement this
UINTERFACE(BlueprintType, MinimalAPI)
class UFrontendLoadingScreenInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FRONTENDUI_API IFrontendLoadingScreenInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenActivated();
	/*virtual void OnLoadingScreenActivated_Implementation(); */

	UFUNCTION(BlueprintNativeEvent)
	void OnLoadingScreenDeactivated();
	/*virtual void OnLoadingScreenDeactivated_Implementation() */
	
};
