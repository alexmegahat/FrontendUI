// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/OptionsDataRegistry.h"

#include "FrontendDebugHelper.h"
#include "FrontendFunctionLibrary.h"
#include "FrontendSettings/FrontendGameUserSettings.h"
#include "Widgets/Options/OptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/ListDataObject_Collection.h"
#include "Widgets/Options/DataObjects/ListDataObject_String.h"
#include "Widgets/Options/DataObjects/ListDataObject_Scalar.h"
#include "Widgets/Options/DataObjects/ListDataObject_StringResolution.h"
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

	/*
	 *TODO:
	 * 1) Figure out a way to make localization (Look at these settings, then ListDataObject_StringInteger ("Custom"), then at individual buttons names in the ue)
	 * 2) Use Enhanced Input
	 * 
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
		GameDifficulty->AddDynamicOption(TEXT("Medium"), FText::FromString(TEXT("Medium")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("Hard")));
		GameDifficulty->AddDynamicOption(TEXT("Super Hard"), FText::FromString(TEXT("Super Hard")));

		//default value
		GameDifficulty->SetDefaultValueFromString(TEXT("Medium"));

		//game user settings specifics
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplySettingsImmediately(true);

		//add setting to the list
		GameplayTabCollection->AddChildListData(GameDifficulty);
	}
	
	//******** Test Item ********//
	//{
	//UListDataObject_String* TestItem = NewObject<UListDataObject_String>();
	//TestItem->SetDataID(FName("TestItem"));
	//TestItem->SetDataDisplayName(FText::FromString("Test Image Item"));
	//TestItem->SetSoftDescriptionImage(UFrontendFunctionLibrary::GetFrontendOptionsSoftImageByTag(FrontendGameplayTags::Frontend_Image_TestImage));
	//GameplayTabCollection->AddChildListData(TestItem);
	//}

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

		AudioTabCollection->AddChildListData(VolumeCategoryCollection); // means attach to parent
		
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
	}

	//****** Sound Category: ******//
	{
		UListDataObject_Collection* SoundCategoryCollection = NewObject<UListDataObject_Collection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategoryCollection"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString("Sound"));

		AudioTabCollection->AddChildListData(SoundCategoryCollection); // means attach to parent

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
		
		//Use HDR Audio
		{
			UListDataObject_StringBool* UseHDRAudio = NewObject<UListDataObject_StringBool>();
			UseHDRAudio->SetDataID(FName("UseHDRAudio"));
			UseHDRAudio->SetDataDisplayName(FText::FromString("Use HDR Audio"));
			UseHDRAudio->OverrideTrueDisplayText(FText::FromString("Enabled"));
			UseHDRAudio->OverrideFalseDisplayText(FText::FromString("Disabled"));
			UseHDRAudio->SetFalseAsDefaultValue();
			UseHDRAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudio));
			UseHDRAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudio));
			UseHDRAudio->SetShouldApplySettingsImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudio);
		}
	}
	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UOptionsDataRegistry::InitVideoCollectionTab()
{
	UListDataObject_Collection* VideoTabCollection = NewObject<UListDataObject_Collection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString("Video"));

	
	//Display Category
	{
		UListDataObject_Collection* DisplayCategoryCollection = NewObject<UListDataObject_Collection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategoryCollection"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString("Display"));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		UListDataObject_StringEnum* CachedWindowMode = nullptr;
		UListDataObject_StringBool* CachedVSync = nullptr;

		FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
		PackagedBuildOnlyCondition.SetEditConditionFunc(
			[]()->bool
			{
				const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld; //either way means we're in editor
				return !bIsInEditor;
			}
		);
		PackagedBuildOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted in the packaged build.</>"));

		//Window Mode
		{
			UListDataObject_StringEnum* WindowMode = NewObject<UListDataObject_StringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString("Window Mode"));
			WindowMode->SetDescriptionRichText(FText::FromString(TEXT("Change between Fullscreen, Borderless Windowed or Windowed modes.")));

			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("Fullscreen")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("Borderless Windowed")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("Windowed")));

			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen); //preferred default for most of the games nowadays
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplySettingsImmediately(true);

			WindowMode->AddEditCondition(PackagedBuildOnlyCondition);

			CachedWindowMode = WindowMode;
			
			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		//Screen Resolution
		{
			UListDataObject_StringResolution* ScreenResolution = NewObject<UListDataObject_StringResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString("Screen Resolution"));
			ScreenResolution->SetDescriptionRichText(FText::FromString(TEXT("Switch between available screen resolutions.")));

			ScreenResolution->InitResolutionValues();

			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescriptor NotBorderlessWindowedCondition;
			NotBorderlessWindowedCondition.SetEditConditionFunc(
				[CachedWindowMode]()->bool
				{
					const bool bIsGameWindowed = CachedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen; /*UFrontendGameUserSettings::Get()->GetFullscreenMode() == EWindowMode::WindowedFullscreen;*/
					return !bIsGameWindowed;
				}
			);
			NotBorderlessWindowedCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting can only be adjusted when the game is in the Fullscreen or Windowed mode.</>"));
			NotBorderlessWindowedCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaximumAllowedResolution());

			ScreenResolution->AddEditCondition(PackagedBuildOnlyCondition);
			ScreenResolution->AddEditCondition(NotBorderlessWindowedCondition);
			ScreenResolution->AddEditDependencyData(CachedWindowMode);
			
			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
		
		//VSync
		{
			UListDataObject_StringBool* VSync = NewObject<UListDataObject_StringBool>();
			VSync->SetDataID(FName("VSync"));
			VSync->SetDataDisplayName(FText::FromString("V-Sync"));
			VSync->SetDescriptionRichText(FText::FromString(TEXT("Synchronizes the game’s frame rate with your display’s refresh rate to prevent screen tearing. May introduce a bit of input latency and can cause stutter if FPS drops below the refresh rate.")));
			
			VSync->OverrideTrueDisplayText(FText::FromString("On"));
			VSync->OverrideFalseDisplayText(FText::FromString("Off"));
			VSync->SetFalseAsDefaultValue();
			
			VSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VSync->SetShouldApplySettingsImmediately(true);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CachedWindowMode]()->bool
				{
					const bool bIsGameFullscreen = CachedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
					return bIsGameFullscreen;
				}
			);
			FullscreenOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting only works when the 'Window Mode' is set to 'Fullscreen'.</>"));
			FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));
			VSync->AddEditCondition(FullscreenOnlyCondition);
			VSync->AddEditDependencyData(CachedWindowMode);

			CachedVSync = VSync;
			
			DisplayCategoryCollection->AddChildListData(VSync);
		}

		//Framerate limit
		{
			UListDataObject_String* FramerateLimit = NewObject<UListDataObject_String>();
			FramerateLimit->SetDataID(FName("FramerateLimit"));
			FramerateLimit->SetDataDisplayName(FText::FromString("Frame Rate Limit"));
			FramerateLimit->SetDescriptionRichText(FText::FromString(TEXT("The maximum amount of FPS allowed to display.")));

			FramerateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(75.f), FText::FromString(TEXT("75 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(100.f), FText::FromString(TEXT("100 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(144.f), FText::FromString(TEXT("144 FPS")));
			FramerateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("Uncapped")));
			FramerateLimit->SetShouldApplySettingsImmediately(true);

			FramerateLimit->SetDefaultValueFromString(LexToString(0.f));

			FramerateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FramerateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));

			FOptionsDataEditConditionDescriptor VSyncDisabledOnlyCondition;
			VSyncDisabledOnlyCondition.SetEditConditionFunc(
				[CachedVSync]()->bool
				{
					return CachedVSync->GetCurrentValueAsBool() == false;
				}
			);
			VSyncDisabledOnlyCondition.SetDisabledRichReason(TEXT("\n\n<Disabled>This setting only works when the 'VSync' is disabled.</>"));
			VSyncDisabledOnlyCondition.SetDisabledForcedStringValue(LexToString(0.f));
			FramerateLimit->AddEditCondition(VSyncDisabledOnlyCondition);
			FramerateLimit->AddEditDependencyData(CachedVSync);

			DisplayCategoryCollection->AddChildListData(FramerateLimit);
		}
	}

	//Graphics Category
	{
		UListDataObject_Collection* GraphicsCategoryCollection = NewObject<UListDataObject_Collection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategoryCollection"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString("Graphics"));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		UListDataObject_StringInteger* CachedOverallQuality = nullptr;

		//Display Gamma (Brightness)
		{
			UListDataObject_Scalar* DisplayGamma = NewObject<UListDataObject_Scalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString("Display Gamma"));
			DisplayGamma->SetDescriptionRichText(FText::FromString(TEXT("Affects overall brightness of the game's window.")));

			//Data Object - Scalar specific
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.0f, 1.0f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); //between default
			DisplayGamma->SetSliderStepSize(0.01f);
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f)); //The default value Unreal has for this setting is 2.2f
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);

			//when displaying a percentage value with No Decimal it will look like: 50%, but with OneDecimal it can be: 50.5%, etc..
			DisplayGamma->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			//OverallVolume->SetShouldApplySettingsImmediately(true); //updated by default from GEngine

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}
		
		//Overall Quality
		{
			UListDataObject_StringInteger* OverallQuality = NewObject<UListDataObject_StringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString("Overall Quality"));
			OverallQuality->SetDescriptionRichText(FText::FromString(TEXT("Affects overall graphics quality of the game.")));

			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplySettingsImmediately(true);

			CachedOverallQuality = OverallQuality;

			GraphicsCategoryCollection->AddChildListData(OverallQuality);
		}

		//Resolution Scale
		{
			UListDataObject_Scalar* ResolutionScale = NewObject<UListDataObject_Scalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString("3D Resolution"));
			ResolutionScale->SetDescriptionRichText(FText::FromString(TEXT("Affects overall resolution scale of the game window.")));

			//Data Object - Scalar specific
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.5f, 1.0f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.5f, 1.0f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDefaultValueFromString(LexToString(1.0f));
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);

			//when displaying a percentage value with No Decimal it will look like: 50%, but with OneDecimal it can be: 50.5%, etc..
			ResolutionScale->SetNumberFormattingOptions(UListDataObject_Scalar::NoDecimal());

			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplySettingsImmediately(true);

			ResolutionScale->AddEditDependencyData(CachedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		//View Distance Quality
		{
			UListDataObject_StringInteger* ViewDistanceQuality = NewObject<UListDataObject_StringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString("View Distance"));
			ViewDistanceQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls how far into the world terrain, objects, and effects are rendered. Higher settings increase long-range clarity and reduce pop-in, but use more computer resources.")));

			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("Near")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("Far")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("Very Far")));

			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplySettingsImmediately(true);

			ViewDistanceQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		//Post Processing Quality
		{
			UListDataObject_StringInteger* PostProcessingQuality = NewObject<UListDataObject_StringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString("Post Processing Quality"));
			PostProcessingQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls image effects applied after the scene is rendered—such as bloom, color grading, tone mapping, ambient occlusion, depth of field, vignette, and motion blur. Higher settings enhance visual polish and cinematic look but can reduce performance; lower settings minimize or disable effects for clearer, faster visuals.")));

			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplySettingsImmediately(true);

			PostProcessingQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}

		//Anti-Aliasing Quality (TSR)
		{
			UListDataObject_StringInteger* AntiAliasingQuality = NewObject<UListDataObject_StringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString("Anti-Aliasing Quality"));
			AntiAliasingQuality->SetDescriptionRichText(FText::FromString(TEXT("Uses Temporal Super Resolution (TSR) to smooth jagged edges and enhance image clarity. This method combines information across multiple frames to deliver sharper visuals at a lower rendering cost. Higher settings provide cleaner edges and finer detail but may impact performance.")));

			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplySettingsImmediately(true);

			AntiAliasingQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		//Shadow Quality
		{
			UListDataObject_StringInteger* ShadowQuality = NewObject<UListDataObject_StringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString("Shadow Quality"));
			ShadowQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the detail and accuracy of shadows cast by objects and characters. Higher settings make shadows appear sharper, more defined, and more realistic, while lower settings may look softer or blocky but improve performance.")));

			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplySettingsImmediately(true);

			ShadowQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}
		
		//Global Illumination Quality
		{
			UListDataObject_StringInteger* GlobalIlluminationQuality = NewObject<UListDataObject_StringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString("Global Illumination"));
			GlobalIlluminationQuality->SetDescriptionRichText(FText::FromString(TEXT("Enables advanced lighting that simulates how light bounces and scatters through the environment. Global Illumination makes scenes appear more realistic as indirect light brightens dark areas and colors subtly influence each other. Higher values improve accuracy but may reduce performance.")));

			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplySettingsImmediately(true);

			GlobalIlluminationQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		//Reflection Quality
		{
			UListDataObject_StringInteger* ReflectionQuality = NewObject<UListDataObject_StringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString("Reflections Quality"));
			ReflectionQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the detail and accuracy of reflections on water, glass, metal, and other shiny surfaces. Higher settings produce sharper, more stable reflections with wider coverage and may use ray-traced methods on supported hardware, but can significantly reduce performance. Lower settings rely more on screen-space or probe-based reflections, which are faster but less accurate and can show artifacts.")));

			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplySettingsImmediately(true);

			ReflectionQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		//Texture Quality
		{
			UListDataObject_StringInteger* TextureQuality = NewObject<UListDataObject_StringInteger>();
			TextureQuality->SetDataID(FName("TexturesQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString("Textures Quality"));
			TextureQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the resolution of textures on characters, environments, and props. Higher settings use larger textures for sharper detail, but require more video memory (VRAM) and can increase loading/streaming time. Lower settings reduce memory usage and may improve performance on GPUs with limited VRAM.")));

			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplySettingsImmediately(true);

			TextureQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		//Visual Effects Quality
		{
			UListDataObject_StringInteger* VisualEffectsQuality = NewObject<UListDataObject_StringInteger>();
			VisualEffectsQuality->SetDataID(FName("VisualEffectsQuality"));
			VisualEffectsQuality->SetDataDisplayName(FText::FromString("Visual Effects Quality"));
			VisualEffectsQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the quality and complexity of effects like explosions, fire, smoke, weather, particles. Higher settings increase detail, density, and lighting interaction but can reduce performance and use more memory; lower settings simplify or disable some effects for smoother framerates.")));

			VisualEffectsQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			VisualEffectsQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			VisualEffectsQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			VisualEffectsQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			VisualEffectsQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			VisualEffectsQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectsQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectsQuality->SetShouldApplySettingsImmediately(true);

			VisualEffectsQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(VisualEffectsQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectsQuality);
		}

		//Foliage Quality
		{
			UListDataObject_StringInteger* FoliageQuality = NewObject<UListDataObject_StringInteger>();
			FoliageQuality->SetDataID(FName("FoliageQuality"));
			FoliageQuality->SetDataDisplayName(FText::FromString("Foliage Quality"));
			FoliageQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the density and shading of grass, trees, and other vegetation. Higher settings increase detail, like wind animation, and self-shadowing with less pop-in, but use more CPU/GPU and memory. Lower settings thin out foliage.")));

			FoliageQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			FoliageQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			FoliageQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			FoliageQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			FoliageQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			FoliageQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFoliageQuality));
			FoliageQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFoliageQuality));
			FoliageQuality->SetShouldApplySettingsImmediately(true);

			FoliageQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(FoliageQuality);

			GraphicsCategoryCollection->AddChildListData(FoliageQuality);
		}

		//Shading Quality
		{
			UListDataObject_StringInteger* ShadingQuality = NewObject<UListDataObject_StringInteger>();
			ShadingQuality->SetDataID(FName("ShadingQuality"));
			ShadingQuality->SetDataDisplayName(FText::FromString("Shading Quality"));
			ShadingQuality->SetDescriptionRichText(FText::FromString(TEXT("Controls the complexity and precision of material and lighting calculations. Higher settings enable richer shading models—more accurate PBR, smoother lighting gradients, detailed normal/parallax mapping, subsurface scattering, and anisotropy—improving realism but increasing GPU cost. Lower settings simplify shaders, which can flatten materials and reduce lighting nuance for better performance.")));

			ShadingQuality->AddIntegerOption(0, FText::FromString(TEXT("Potato")));
			ShadingQuality->AddIntegerOption(1, FText::FromString(TEXT("Low")));
			ShadingQuality->AddIntegerOption(2, FText::FromString(TEXT("Medium")));
			ShadingQuality->AddIntegerOption(3, FText::FromString(TEXT("High")));
			ShadingQuality->AddIntegerOption(4, FText::FromString(TEXT("Very High")));

			ShadingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadingQuality));
			ShadingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadingQuality));
			ShadingQuality->SetShouldApplySettingsImmediately(true);

			ShadingQuality->AddEditDependencyData(CachedOverallQuality);
			
			CachedOverallQuality->AddEditDependencyData(ShadingQuality);

			GraphicsCategoryCollection->AddChildListData(ShadingQuality);
		}
	}
	
	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UOptionsDataRegistry::InitControlCollectionTab()
{
	UListDataObject_Collection* ControlsTabCollection = NewObject<UListDataObject_Collection>();
	ControlsTabCollection->SetDataID(FName("ControlsTabCollection"));
	ControlsTabCollection->SetDataDisplayName(FText::FromString("Controls"));

	RegisteredOptionsTabCollections.Add(ControlsTabCollection);
}
