// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "FrontendGameplayTags.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings, SetterOrGetterFuncName))

void UOptionsDataRegistry::InitOptionsDataRegistry(ULocalPlayer* InOwningLocalPlayer)
{
	/*
	//Next commented lines are the full code to construct data interactor helper (used for each list entry that is changing settings in Init...Tab() funcs).
	//To speed things up here, we'll use a macro MAKE_OPTIONS_DATA_CONTROL to layout compiler
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

	TArray<UListDataObject_Base*> AllChildListItems;

	for (UListDataObject_Base* ChildListData : FoundTabCollection->GetAllChildListData())
	{
		if (!ChildListData) continue;

		AllChildListItems.Add(ChildListData); // add this list data object

		if (ChildListData->HasAnyChildListData()) //add sub list data objects
		{
			FindChildListDataRecursively(ChildListData, AllChildListItems);
		}
	}
	return AllChildListItems;
}

void UOptionsDataRegistry::FindChildListDataRecursively(UListDataObject_Base* InParentData,
	TArray<UListDataObject_Base*>& OutFoundChildList) const
{
	if (!InParentData || !InParentData->HasAnyChildListData()) return; //without these terminating conditions we'll get stack overflow

	for (UListDataObject_Base* SubChildListData : InParentData->GetAllChildListData())
	{
		if (!SubChildListData) continue;

		OutFoundChildList.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildList);
		}
	}
}

void UOptionsDataRegistry::InitGameplayCollectionTab()
{
	UListDataObject_Collection* GameplayTabCollection = NewObject<UListDataObject_Collection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString("Gameplay"));

	
	//****** Game Specific Settings: ******//
	
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

	//****** Volume Category: ******//
	{
		UListDataObject_Collection* VolumeCategoryCollection = NewObject<UListDataObject_Collection>();
		//VolumeCategoryCollection->AddChildListData() for the setting - means group the settings under this volume category together.
		//With this setup AudioTabCollection will be top-level parent and under this tab we will have different
		//categories as the subcategory.
		VolumeCategoryCollection->SetDataID(FName("VolumeCategoryCollection"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("Volume"))); //name that we'll display in the header ui
		
		//Overall Volume
		{
			UListDataObject_Scalar* OverallVolume = NewObject<UListDataObject_Scalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("Overall Volume")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("Affects overall volume of the game, including in-game sound effects and background music.")));

			//Data Object - Scalar specific
			OverallVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f)); //this is how we can set default value, using float
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);

			//when displaying a percentage value with No Decimal it will look like: 50%, but with OneDecimal it can be: 50.5%, etc..
			OverallVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			//OverallVolume->SetShouldApplySettingsImmediately(false); false by default, handled in the game user settings class separately
			
			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		//Music Volume
		{
			UListDataObject_Scalar* MusicVolume = NewObject<UListDataObject_Scalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("Music Volume")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("Affects game's background music volume, in-game and in the menus.")));

			//Data Object - Scalar specific
			MusicVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f)); //this is how we can set default value, using float
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);

			//when displaying a percentage value with No Decimal it will look like: 50%, but with OneDecimal it can be: 50.5%, etc..
			MusicVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			//MusicVolume->SetShouldApplySettingsImmediately(false); false by default, handled in the game user settings class separately
			
			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		//SoundFX Volume
		{
			UListDataObject_Scalar* SoundFXVolume = NewObject<UListDataObject_Scalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("Sound Effects Volume")));
			SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("Affects in-game SFX volume: in-game sounds like characters running or attacking, nature sounds, background soundscapes, etc. Does not include background music.")));

			//Data Object - Scalar specific
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.0f, 1.0f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f)); //this is how we can set default value, using float
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);

			//when displaying a percentage value with No Decimal it will look like: 50%, but with OneDecimal it can be: 50.5%, etc..
			SoundFXVolume->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			//SoundFXVolume->SetShouldApplySettingsImmediately(false); false by default, handled in the game user settings class separately
			
			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}
		
		AudioTabCollection->AddChildListData(VolumeCategoryCollection); // means attach to parent
	}

	//****** Sound Category: ******//
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString("Sound"));

		//Allow Background Audio
		{
			UListDataObject_StringBool* AllowBackgroundAudio = NewObject<UListDataObject_StringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString("Allow Background Audio"));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}
		
		AudioTabCollection->AddChildListData(SoundCategoryCollection);
	}

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
