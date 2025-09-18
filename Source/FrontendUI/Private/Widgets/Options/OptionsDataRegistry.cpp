// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "FrontendGameplayTags.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UListDataObject_Base*> UOptionsDataRegistry::GetListSourceItemsByTabID(const FName& InSelectedTabID) const
{
	UListDataObject_Collection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UListDataObject_Collection* AvailableTabCollection)->bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("No valid tab found under the ID %s"), *InSelectedTabID.ToString());
	UListDataObject_Collection* FoundTabCollection = *FoundTabCollectionPtr;

	return FoundTabCollection->GetAllChildListData();
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	
	//****** Game Specific Settings: ******//

	/*
	//Next commented lines are the full code to construct data interactor helper. To speed
	//things up here, we'll use a macro MAKE_OPTIONS_DATA_CONTROL to layout compiler
	//to write the code needed for us.
	//
	//TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper =
	// 	MakeShared<FOptionsDataInteractionHelper>(
	//		GET_FUNCTION_NAME_STRING_CHECKED(
	// 			UFrontendGameUserSettings,
	// 			GetCurrentGameDifficulty
	//		)
	//	);
	*/
	
	//******** Game Difficulty ********//
	{
		//init
		UListDataObject_String* GameDifficulty = NewObject<UListDataObject_String>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("Game Difficulty")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT("Adjust the game difficulty of the game experience.\n\n<Bold>Very Easy:</> Mom's good boy \n\n<Bold>Easy:</> Easy there, pal! \n\n<Bold>Normal:</> You're only human, after all. \n\n<Bold>Hard:</> Your dad will be proud. \n\n<Bold>Super Hard:</> Press F to pay respect...")));

		//add all the possible values
		GameDifficulty->AddDynamicOption(TEXT("Very Easy"), FText::FromString(TEXT("Very Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("Easy")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("Normal")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Super Hard"), FText::FromString(TEXT("Super Hard")));

		//default value
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));

		//game user settings specifics
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		//add setting to the list
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	
	//******** Test Item ********//
	{
	UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
	TestItem->SetDataID(FName("TestItem"));
	TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
	TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetFrontendOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
	GameplayTabCollection->AddChildListData(TestItem);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UOptionsDataRegistry::InitAudioCollectionTab()
{
	UListDataObject_Collection* AudioTabCollection = NewObject<UListDataObject_Collection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString("Audio"));

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlsTabCollection = NewObject<UListDataObject_Collection>();
	ControlsTabCollection->SetDataID(FName("ControlsTabCollection"));
	ControlsTabCollection->SetDataDisplayName(FText::FromString("Controls"));

	RegisteredOptionsTabCollections.Add(ControlsTabCollection);
}
