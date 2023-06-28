// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSubsystemComponent.h"
#include "AmbiverseElementManager.generated.h"

class UAmbiverseElementInstance;

UCLASS()
class UAmbiverseElementManager : public UAmbiverseSubsystemComponent
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseElementManager, Log, All)

private:
	UPROPERTY()
	TArray<UAmbiverseElementInstance*> ScheduledProceduralElements;

	UPROPERTY()
	TArray<UAmbiverseElementInstance*> PlayingProceduralElements;

public:
	virtual void Tick(const float DeltaTime) override;

	void RegisterElements(TArray<UAmbiverseElementInstance*> Elements);
	void UnregisterElements(TArray<UAmbiverseElementInstance*> Elements);

private:
	void PlayProceduralElement(UAmbiverseElementInstance* ProceduralElement);

	void ScheduleProceduralElement(UAmbiverseElementInstance* ProceduralElement);

	/** primes a group of elements. */
	void PrimeElements(TArray<UAmbiverseElementInstance*> Elements, uint16 PrimeCount = 5);
};


