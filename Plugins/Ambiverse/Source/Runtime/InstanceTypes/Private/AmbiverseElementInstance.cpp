// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseElementInstance.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseElementInstance, LogAmbiverseProceduralElement);

void UAmbiverseElementInstance::SetTime(const float DensityMultiplier)
{
	if (RuntimeData.IntervalRange.X <= 0 || RuntimeData.IntervalRange.Y <= 0)
	{
		UE_LOG(LogAmbiverseProceduralElement, Warning, TEXT("SetNewTime: Interval ranges must be greater than 0."));
		return;
	}
	
	ReferenceTime = FMath::RandRange(RuntimeData.IntervalRange.X,RuntimeData.IntervalRange.Y);

	Time = ReferenceTime * DensityMultiplier;
}

UAmbiverseElementInstance* UAmbiverseElementInstance::CreateInstanceFromRuntimeData(UObject* Outer, FAmbiverseElementRuntimeData& RuntimeData)
{
	if (!RuntimeData.Element) { return nullptr; }
	
	UAmbiverseElementInstance* ElementInstance = {NewObject<UAmbiverseElementInstance>(Outer)};
	
	ElementInstance->RuntimeData = RuntimeData;
	
	return ElementInstance;
}

