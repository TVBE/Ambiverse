// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseDistributionManager.generated.h"

class UAmbiverseDistributorAsset;
class UAmbiverseSubsystem;
class UAmbiverseElementInstance;

/** Manages sound source distribution. */
UCLASS()
class UAmbiverseDistributionManager : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseDistributionManager, Log, All)

private:
	UPROPERTY()
	TArray<UAmbiverseDistributorAsset*> Distributors;

public:
	bool GetTransformForElement(FTransform& Transform, UAmbiverseElementInstance* ElementInstance);
	
	/** Searches for a distributor instance in the registry. Will instance one if no instance was found. */
	UAmbiverseDistributorAsset* GetDistributorByClass(TSubclassOf<UAmbiverseDistributorAsset> Class);
	
	FORCEINLINE TArray<UAmbiverseDistributorAsset*> GetDistributorRegistry() const { return Distributors; }
};


