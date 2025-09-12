// Aleksei Miakishev All Rights Reserved

#include "AsyncActions/AsyncActionPushConfirmScreen.h"
#include "Subsystems/FrontendUISubsystem.h"

UAsyncActionPushConfirmScreen* UAsyncActionPushConfirmScreen::PushConfirmScreen(const UObject* WorldContextObject,
	EConfirmScreenType ScreenType, FText InScreenTitle, FText InScreenMessage)
{
	if (GEngine)
	{
		if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
		{
			UAsyncActionPushConfirmScreen* Node = NewObject<UAsyncActionPushConfirmScreen>();

			Node->CachedOwningWorld = World;
			Node->CachedScreenType = ScreenType;
			Node->CachedScreenTitle = InScreenTitle;
			Node->CachedScreenMessage = InScreenMessage;

			Node->RegisterWithGameInstance(World);
			return Node;
		}
	}
	return nullptr;
}

void UAsyncActionPushConfirmScreen::Activate()
{
	UFrontendUISubsystem::Get(CachedOwningWorld.Get())->PushConfirmScreenToModalStackAsync(
		CachedScreenType,
		CachedScreenTitle,
		CachedScreenMessage,
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			OnButtonClicked.Broadcast(ClickedButtonType);
			SetReadyToDestroy();
		});
}
