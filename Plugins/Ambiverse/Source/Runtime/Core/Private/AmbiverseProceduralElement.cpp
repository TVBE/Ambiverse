// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseProceduralElement.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseProceduralElement, LogAmbiverseProceduralElement);

void UAmbiverseProceduralElement::SetTime(const float DensityMultiplier)
{
	if (ProceduralElementData.IntervalRange.X <= 0 || ProceduralElementData.IntervalRange.Y <= 0)
	{
		UE_LOG(LogAmbiverseProceduralElement, Warning, TEXT("SetNewTime: Interval ranges must be greater than 0."));
		return;
	}
	
	ReferenceTime = FMath::RandRange(ProceduralElementData.IntervalRange.X,ProceduralElementData.IntervalRange.Y);

	Time = ReferenceTime * DensityMultiplier;
}

