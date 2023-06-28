// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseLayer.h"
#include "AmbiverseProceduralElementData.h"
#include "AmbiverseProceduralElement.generated.h"

class UAmbiverseSubsystem;
UCLASS(MinimalAPI, Transient)
class UAmbiverseProceduralElement : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseProceduralElement, Log, All)

public:
	UPROPERTY()
	FAmbiverseProceduralElementData ProceduralElementData;
	
	float Time {0.0f};

	bool IsPlaying {false};
	
	/** The reference time the entry was initialized with.
	 *	We use this time value to be able to dynamically apply parameters in real time without breaking th existing queue. */
	float ReferenceTime {0.0f};

	void Initialize(const FAmbiverseProceduralElementData& InProceduralElementData)
	{
		ProceduralElementData = InProceduralElementData;
	}
	
	void SetTime(const float DensityMultiplier = 1.0f);
};
