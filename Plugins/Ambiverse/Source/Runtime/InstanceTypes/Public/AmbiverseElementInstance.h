// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseLayerAsset.h"
#include "AmbiverseElementRuntimeData.h"
#include "AmbiverseElementInstance.generated.h"

class UAmbiverseSubsystem;
UCLASS(MinimalAPI, Transient)
class UAmbiverseElementInstance : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseProceduralElement, Log, All)

public:
	UPROPERTY()
	FAmbiverseElementRuntimeData RuntimeData;

	UPROPERTY()
	UAmbiverseLayerAsset* OwningLayer;
	
	float Time {0.0f};
	
	/** The reference time the entry was initialized with.
	 *	We use this time value to be able to dynamically apply parameters in real time without breaking th existing queue. */
	float ReferenceTime {0.0f};

	bool IsPlaying {false};
	bool IsPendingKill {false};

	void Initialize(const FAmbiverseElementRuntimeData& InProceduralElementData, UAmbiverseLayerAsset* Layer)
	{
		RuntimeData = InProceduralElementData;
		OwningLayer = Layer;
	}
	
	void SetTime(const float DensityMultiplier = 1.0f);

	static UAmbiverseElementInstance* CreateInstanceFromRuntimeData(UObject* Outer, FAmbiverseElementRuntimeData& RuntimeData);
};
