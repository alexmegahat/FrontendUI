// Aleksei Miakishev All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "FrontendLoadingScreenSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class FRONTENDUI_API UFrontendLoadingScreenSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadingReasonUpdatedDelegate, const FString&, CurrentLoadingReason);

	UPROPERTY(BlueprintAssignable)
	FOnLoadingReasonUpdatedDelegate OnLoadingReasonUpdated;
	
	//~Begin USubsystem Interface
	virtual bool ShouldCreateSubsystem(UObject* Outer) const;

	//bind and unbind OnMapPreLoaded and OnPostLoadMap
	virtual void Initialize(FSubsystemCollectionBase& Collection);
	virtual void Deinitialize();
	//~End USubsystem Interface

	
	//~Begin FTickableGameObject Interface
	
	//ticking is needed to update the 'tickable reason'
	//that is displayed in the loading screen's widget
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(float DeltaTime) override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual bool IsTickable() const override;
	TStatId GetStatId() const override;
	//~End FTickableGameObject Interface

private:
	//these functions are used to enable and disable ticking for this subsystem
	void OnMapPreLoaded(const FWorldContext& WorldContext, const FString& MapName);
	void OnPostLoadMap(UWorld* LoadedWorld);

	//update the loading screen
	void TryUpdateLoadingScreen();

	//helper functions for TryUpdateLoadingScreen()
	bool IsPreLoadScreenActive() const;
	bool ShouldShowLoadingScreen();
	bool CheckTheNeedToShowLoadingScreen();
	void TryDisplayLoadingScreenIfNone();
	void TryRemoveLoadingScreen();
	void NotifyLoadingScreenVisibilityChanged(bool bIsVisible);

	bool bIsCurrentlyLoadingMap;
	float HoldLoadingScreenStartupTime = -1.f;
	FString CurrentLoadingReason;
	TSharedPtr<SWidget> CachedCreatedLoadingScreenWidget;
};
