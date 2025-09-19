// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "OptionsDataRegistry.generated.h"

class UListDataObject_Collection;
class UListDataObject_Base;

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UOptionsDataRegistry : public UObject
{
	GENERATED_BODY()
	
public:
	//Gets called by options screen right after the object of type UOptionsDataRegistry is created,
	//and in this function we're handling data creation for each tab.
	void InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer);

	const TArray<UListDataObject_Collection*>& GetRegisteredOptionsTabCollections() const {return RegisteredOptionsTabCollections; }

	TArray<UListDataObject_Base*> GetListSourceItemsByTabID(const FName& InSelectedTabID) const;
	
private:
	void FindChildListDataRecursively(UListDataObject_Base* InParentData, TArray<UListDataObject_Base*>& OutFoundChildList) const;
	
	//Func for Gameplay Tab
	void InitGameplayCollectionTab();
	
	//Func for Audio Tab
	void InitAudioCollectionTab();
	
	//Func for Video Tab
	void InitVideoCollectionTab();
	
	//Func for Controls Tab
	void InitControlCollectionTab();

	UPROPERTY(Transient)
	TArray<UListDataObject_Collection*> RegisteredOptionsTabCollections;
};
