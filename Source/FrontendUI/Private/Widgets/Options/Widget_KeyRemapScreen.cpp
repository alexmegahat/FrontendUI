// Aleksei Miakishev All Rights Reserved


#include "Widgets/Options/Widget_KeyRemapScreen.h"

#include "CommonUITypes.h"
#include "CommonInputSubsystem.h"
#include "CommonRichTextBlock.h"
#include "Framework/Application/IInputProcessor.h"
#include "FrontendDebugHelper.h"
#include "ICommonInputModule.h"

//****** Input Preprocessor *******//
class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	//constructor
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* LocalPlayer)
		: CachedInputTypeToListenTo(InInputTypeToListenTo)
		, CachedWeakOwningLocalPlayer(LocalPlayer)
	{};

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /*PressedKey*/)
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCancelledDelegate, const FString& /*CanceledReason*/)
	FOnInputPreProcessorKeySelectCancelledDelegate OnInputPreProcessorKeySelectCancelled;
	
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		//DEBUG STUFF
		//Debug::Print(TEXT("Pressed Key: ") + InKeyEvent.GetKey().GetDisplayName().ToString());
		//UEnum* StaticCommonInputType = StaticEnum<ECommonInputType>();
		//Debug::Print(TEXT("Desired input key type: ") + StaticCommonInputType->GetValueAsString(CachedInputTypeToListenTo));

		ProcessPressedKey(InKeyEvent.GetKey());
		
		return true;
	}

	virtual bool HandleMouseButtonDownEvent( FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		//DEBUG STUFF
		//Debug::Print(TEXT("Pressed Mouse Button: ") + MouseEvent.GetEffectingButton().GetDisplayName().ToString());

		ProcessPressedKey(MouseEvent.GetEffectingButton());

		return false;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		//if we want to cancel the action ourselves
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectCancelled.ExecuteIfBound(TEXT("Key Remap has been cancelled.")); //TODO: localization

			return;
		}

		//eliminate the situation where when we press on a Confirm on the gamepad, we're getting keyboard confirm input
		//this is because Confirm action is hardcoded to a specific button on the gamepad.
		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());
		check(CommonInputSubsystem);
		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();
		
		//if we are pressing wrong keys, then also cancel
		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectCancelled.ExecuteIfBound(TEXT("Detected Gamepad Key pressed for keyboard inputs. Key Remap has been canceled.")); //TODO: localization
				return;
			}
			break;
		case ECommonInputType::Gamepad:
			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{   //if we can get inside this if check, this means we pressed Confirm on the Gamepad
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionData);

				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}
			if (!InPressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectCancelled.ExecuteIfBound(TEXT("Detected Non-Gamepad Key pressed for gamepad inputs. Key Remap has been canceled.")); //TODO: localization
				return;
			}
			break;
		default:
			break;
		}

		//and finally if we are pressing the right keys, then continue with remapping. 
		OnInputPreProcessorKeyPressed.Execute(InPressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;

	UPROPERTY()
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};

//****** Input Preprocessor *******//


//****** UWidget_KeyRemapScreen *******//

void UWidget_KeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UWidget_KeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	//Display message
	FString InputDeviceName;
	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = "Mouse & Keyboard"; //TODO: localization
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = "Gamepad"; //TODO: localization
		break;
	default:
		break;
	}

	const FString DisplayRichMessage = FString::Printf(
		TEXT("<KeyRemapDefault>Press any</> <KeyRemapHighlight>%s</> <KeyRemapDefault>key.</>"),
		*InputDeviceName
	);

	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
	
	//Input preprocessor
	//MakeShared instead of NewObject
	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(CachedDesiredInputType, GetOwningLocalPlayer());

	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectCancelled.BindUObject(this, &ThisClass::OnKeySelectCancelled);
	
	//we need this preprocessor to be on the top, to look out for all inputs before everything else,
	//hence the -1 index
	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);
}

void UWidget_KeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);
		CachedInputPreprocessor.Reset();
	}
}

void UWidget_KeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactivateWidget(
		[this, PressedKey]()
		{
			Debug::Print(TEXT("Pressed key ") + PressedKey.GetDisplayName().ToString());
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UWidget_KeyRemapScreen::OnKeySelectCancelled(const FString& CanceledReason)
{
	RequestDeactivateWidget(
		[this, CanceledReason]()
		{
			Debug::Print(CanceledReason);
			OnKeyRemapScreenKeySelectCancel.ExecuteIfBound(CanceledReason);
		}
	);
}

void UWidget_KeyRemapScreen::RequestDeactivateWidget(TFunction<void()> PreDeactivateCallback)
{
	//Delay a tick before broadcasting to make sure the input is processed correctly
	//FTSTicker::GetCoreTicker().AddTicker(...) function takes 2 inputs,
	//first one being the delegate that fires a function that fires after the delay,
	//second one being the delay, 0.f meaning 'next frame'
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[PreDeactivateCallback, this](float DeltaTime)->bool
			{   //getting to this point -> a frame later after the RequestDeactivateWidget gets called (delay by one frame)

				PreDeactivateCallback();

				DeactivateWidget();

				return false; //return false means stop ticking
			}
		)
	);
}

//****** UWidget_KeyRemapScreen *******//
