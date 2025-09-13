// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ListDataObject_Base.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const {return PropertyName;} \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName;}

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UListDataObject_Base : public UObject
{
	GENERATED_BODY()
public:
	LIST_DATA_ACCESSOR(FName, DataID);
	LIST_DATA_ACCESSOR(FText, DataDisplayName);
	LIST_DATA_ACCESSOR(FText, DescriptionRichText);
	LIST_DATA_ACCESSOR(FText, DisabledRichText);
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESSOR(UListDataObject_Base*, ParentData);

	void InitDataObject();

	//Empty in the base class. Child class ListDataObject_Collection should override it. The function should return all the child data the tab has.
	virtual TArray<UListDataObject_Base*> GetAllChildListData() const { return TArray<UListDataObject_Base*>(); }

	//Needs to be overridden in the child class.
	virtual bool HasAnyChildListData() const { return false; }

protected:
	//Empty in base class. The child classes should override it to handle the initialization needed for the child class
	virtual void OnDataObjectInitialized();
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText; //for options list
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;
	
	UPROPERTY(Transient)
	UListDataObject_Base* ParentData;
	
	
	
};
