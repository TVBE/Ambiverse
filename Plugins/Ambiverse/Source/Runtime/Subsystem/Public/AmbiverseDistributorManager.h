// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseDistributorManager.generated.h"

class UAmbiverseDistributorAsset;
class UAmbiverseSubsystem;

/** Manages distributor instances. */
UCLASS()
class UAmbiverseDistributorManager : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseDistributorManager, Log, All)

private:
	UPROPERTY()
	TArray<UAmbiverseDistributorAsset*> Distributors;

public:
	/** Searches for a distributor instance in the registry. Will instance one if no instance was found. */
	UAmbiverseDistributorAsset* GetDistributorByClass(TSubclassOf<UAmbiverseDistributorAsset> Class);
	
	FORCEINLINE TArray<UAmbiverseDistributorAsset*> GetDistributorRegistry() const { return Distributors; }
};
