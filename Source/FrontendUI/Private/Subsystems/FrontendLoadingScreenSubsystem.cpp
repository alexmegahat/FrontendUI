// Aleksei Miakishev All Rights Reserved


#include "Subsystems/FrontendLoadingScreenSubsystem.h"
#include "PreLoadScreenManager.h"
#include "FrontendSettings/FrontendLoadingScreenSettings.h"
#include "Blueprint/UserWidget.h"
#include "FrontendDebugHelper.h"
#include "IMediaCache.h"
#include "Interfaces/FrontendLoadingScreenInterface.h"

bool UFrontendLoadingScreenSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses);

		return FoundClasses.IsEmpty();
	}
	return false;
}

void UFrontendLoadingScreenSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	FCoreUObjectDelegates::PreLoadMapWithContext.AddUObject(this, &ThisClass::OnMapPreLoaded);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostLoadMap);
}

void UFrontendLoadingScreenSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMapWithContext.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
}

UWorld* UFrontendLoadingScreenSubsystem::GetTickableGameObjectWorld() const
{
	if (UGameInstance* OwningGameInstance = GetGameInstance())
	{
		return OwningGameInstance->GetWorld();
	}
	return nullptr;
}

void UFrontendLoadingScreenSubsystem::Tick(float DeltaTime)
{
	//Debug::Print("UFrontendLoadingScreenSubsystem ticking!");

	TryUpdateLoadingScreen();
}

ETickableTickType UFrontendLoadingScreenSubsystem::GetTickableTickType() const
{
	if (IsTemplate())
	{
		return ETickableTickType::Never;
	}
	return ETickableTickType::Conditional;
}

bool UFrontendLoadingScreenSubsystem::IsTickable() const
{
	//allow ticking if there is a valid game instance and valid game viewport associated with this game instance
	return GetGameInstance() && GetGameInstance()->GetGameViewportClient();
}

TStatId UFrontendLoadingScreenSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UFrontendLoadingScreenSubsystem, STATGROUP_Tickables);
}

void UFrontendLoadingScreenSubsystem::OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName)
{
	//Debug::Print("OnMapPreLoaded");
	if (WorldContext.OwningGameInstance != GetGameInstance())
	{
		return;
	}
	SetTickableTickType(ETickableTickType::Conditional);

	bIsCurrentlyLoadingMap = true;

	TryUpdateLoadingScreen();
}

void UFrontendLoadingScreenSubsystem::OnPostLoadMap(UWorld* LoadedWorld)
{
	//Debug::Print("OnPostLoadMap");

	//this means that the loading is complete
	if (LoadedWorld && LoadedWorld->GetGameInstance() == GetGameInstance())
	{
		bIsCurrentlyLoadingMap = false;
	}
}

void UFrontendLoadingScreenSubsystem::TryUpdateLoadingScreen()
{
	//Check if there is any startup loading screen that's currently active
	//and if the answer is true, then we don't need to show any loading screen.
	//We should wait for this startup loading screen to disappear,
	//only then we can show our loading screen.
	if (IsPreLoadScreenActive())
	{
		return;
	}

	
	//Check if we should show the loading screen
	if (ShouldShowLoadingScreen())
	{
		//Try display loading screen
		TryDisplayLoadingScreenIfNone();
		
		OnLoadingReasonUpdated.Broadcast(CurrentLoadingReason);
	}
	else
	{
		//Try to remove active loading screen
		TryRemoveLoadingScreen();
		HoldLoadingScreenStartupTime = -1.f;
		
		//Notify the loading is complete
		NotifyLoadingScreenVisibilityChanged(false);

		//Disable ticking
		SetTickableTickType(ETickableTickType::Never);
	}
	
}

bool UFrontendLoadingScreenSubsystem::IsPreLoadScreenActive() const
{
	if (FPreLoadScreenManager* PreloadScreenManager = FPreLoadScreenManager::Get())
	{
		return PreloadScreenManager->HasValidActivePreLoadScreen();
	}
	return false;
}

bool UFrontendLoadingScreenSubsystem::ShouldShowLoadingScreen()
{
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();

	//check config variable for the case of in-editor
	if (GIsEditor && !LoadingScreenSettings->bShowLoadingScreenInEditor)
	{
		return false;
	}

	//check if the objects in the world need a loading screen
	if (CheckTheNeedToShowLoadingScreen())
	{
		//if we need to show loading screen, then we don't need to render the world to viewport
		GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = true;
		return true;
	}

	CurrentLoadingReason = TEXT("Waiting for Texture Streaming");

	//There's no need to show loading screen -> allow world to get rendered to the viewport.
	GetGameInstance()->GetGameViewportClient()->bDisableWorldRendering = false;

	float CurrentTime = FPlatformTime::Seconds();

	if (HoldLoadingScreenStartupTime<0.f) //this means that this var hasn't been set yet
	{
		HoldLoadingScreenStartupTime = CurrentTime;
	}
	
	const float ElapsedTime = CurrentTime - HoldLoadingScreenStartupTime;

	if (ElapsedTime < LoadingScreenSettings->HoldLoadingScreenExtraSeconds)
	{
		return true;
	}
	
	return false;
}

bool UFrontendLoadingScreenSubsystem::CheckTheNeedToShowLoadingScreen()
{
	if (bIsCurrentlyLoadingMap)
	{
		CurrentLoadingReason = TEXT("Loading Level");
		return true;
	}

	UWorld* OwningWorld = GetGameInstance()->GetWorld();

	if (!OwningWorld)
	{
		CurrentLoadingReason = TEXT("Initializing World");
		return true;
	}

	if (!OwningWorld->HasBegunPlay())
	{
		CurrentLoadingReason = TEXT("World hasn't begun play yet");
		return true;
	}

	if (!OwningWorld->GetFirstPlayerController())
	{
		CurrentLoadingReason = TEXT("Initializing Player Controller");
		return true;
	}

	//Check if the game states/player states/character components are ready
	// ---
	// %%%
	// ---
	
	return false;
}

void UFrontendLoadingScreenSubsystem::TryDisplayLoadingScreenIfNone()
{
	//If there is already active loading screen, return early
	if (CachedCreatedLoadingScreenWidget)
	{
		return;
	}
	
	
	const UFrontendLoadingScreenSettings* LoadingScreenSettings = GetDefault<UFrontendLoadingScreenSettings>();
	
	TSubclassOf<UUserWidget> LoadedWidgetClass = LoadingScreenSettings->GetLoadingScreenWidgetClassChecked();

	//because we're drawing the widget during the level transition,
	//we can no longer rely on the world or the player controller,
	//we'll have to rely on game instance
	UUserWidget* CreatedWidget = UUserWidget::CreateWidgetInstance(*GetGameInstance(), LoadedWidgetClass, NAME_None);

	check(CreatedWidget);

	//get underlying slate widget
	CachedCreatedLoadingScreenWidget = CreatedWidget->TakeWidget();
	
	GetGameInstance()->GetGameViewportClient()->AddViewportWidgetContent(
		CachedCreatedLoadingScreenWidget.ToSharedRef(),
		1000 //guarantee that the loading screen is on the very top
	);

	NotifyLoadingScreenVisibilityChanged(true);
}

void UFrontendLoadingScreenSubsystem::TryRemoveLoadingScreen()
{
	//if no widget to remove
	if (!CachedCreatedLoadingScreenWidget) return;

	GetGameInstance()->GetGameViewportClient()->RemoveViewportWidgetContent(CachedCreatedLoadingScreenWidget.ToSharedRef());

	CachedCreatedLoadingScreenWidget.Reset();
}

void UFrontendLoadingScreenSubsystem::NotifyLoadingScreenVisibilityChanged(bool bIsVisible)
{
	for (ULocalPlayer* ExistingLocalPlayer : GetGameInstance()->GetLocalPlayers())
	{
		if (!ExistingLocalPlayer) continue;

		if (APlayerController* PC = ExistingLocalPlayer->GetPlayerController(GetGameInstance()->GetWorld()))
		{
			//Notify objects like PC, Pawns, that implement the interface UFrontendLoadingScreenInterface
			
			
			//Query if the player controller implements the interface.
			//Call the function through the interface to notify the loading status if yes.
			if (PC->Implements<UFrontendLoadingScreenInterface>())
			{
				if (bIsVisible)
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(PC);
				}
				else
				{
					IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(PC);
				}
			}
			//or like this
			//if (IFrontendLoadingScreenInterface* LoadingScreenInterface = Cast<IFrontendLoadingScreenInterface>(PC))
			//{
			//	LoadingScreenInterface->OnLoadingScreenActivated(); //and add bIsVisible if...else for deactivated
			//}
			
			if (APawn* OwningPawn = PC->GetPawn())
			{
				if (OwningPawn->Implements<UFrontendLoadingScreenInterface>())
				{
					if (bIsVisible)
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenActivated(OwningPawn);
					}
					else
					{
						IFrontendLoadingScreenInterface::Execute_OnLoadingScreenDeactivated(OwningPawn);
					}
				}
			}

			//The code for notifying other objects in the world goes here
		}
	}
}
