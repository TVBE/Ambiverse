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

private:
	static bool PerformRandomDistribution(FTransform& OutTransform, const FTransform& ListenerTransform,
		UAmbiverseElementInstance* ElementInstance);
	
	static bool PerformUniformDistribution(FTransform& OutTransform, const FTransform& ListenerTransform,
		UAmbiverseElementInstance* ElementInstance, const TArray<FVector>& Vectors, const bool IgnoreZ);
	
	static bool PerformStaticDistribution(FTransform& OutTransform, const FTransform& ListenerTransform,
		UAmbiverseElementInstance* ElementInstance);

public:
	FORCEINLINE TArray<UAmbiverseDistributorAsset*> GetDistributors() const { return Distributors; }
};


