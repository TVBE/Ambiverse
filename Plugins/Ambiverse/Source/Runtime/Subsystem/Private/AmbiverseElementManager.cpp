// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseElementManager.h"
#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseParameterManager.h"
#include "AmbiverseElementInstance.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseElementManager, LogAmbiverseElementManager);

void UAmbiverseElementManager::RegisterElements(TArray<UAmbiverseElementInstance*> Elements)
{
	if (Elements.IsEmpty()) { return; }
	
	PrimeElements(Elements);
	ScheduledProceduralElements.Append(Elements);

	for (const UAmbiverseElementInstance* Element : Elements)
	{
		UE_LOG(LogAmbiverseElementManager, VeryVerbose, TEXT("Registered and scheduled element: '%s'"), *Element->GetName());
	}
}

void UAmbiverseElementManager::UnregisterElements(TArray<UAmbiverseElementInstance*> Elements)
{
	if (Elements.IsEmpty()) { return; }

	for (UAmbiverseElementInstance* ProceduralElement : Elements)
	{
		ScheduledProceduralElements.Remove(ProceduralElement);
	}

	for (UAmbiverseElementInstance* ProceduralElement : Elements)
	{
		int32 i;
		PlayingProceduralElements.Find(ProceduralElement, i);

		PlayingProceduralElements[i]->IsPendingKill = true;
	}
}

void UAmbiverseElementManager::EvaluateFinishedElement(UAmbiverseElementInstance* Element)
{
	if (!Element) { return; }

	/** If the element was marked pending kill while being associated with an sound source, we destroy the element at this moment. */
	if (Element->IsPendingKill)
	{
		Element->ConditionalBeginDestroy();
		UE_LOG(LogAmbiverseElementManager, VeryVerbose, TEXT("EvaluateFinishedElement: Destroyed element due to being marked PendingKill: '%s'"), *Element->GetName());
	}

	if (Element->RuntimeData.IntervalMode == EIntervalMode::OnSpawn && ScheduledProceduralElements.Contains(Element))
	{
		UE_LOG(LogAmbiverseElementManager, VeryVerbose, TEXT("EvaluateFinishedElement: Element is already scheduled: '%s'"), *Element->GetName());
		return;
	}
	if (Element->RuntimeData.IntervalMode == EIntervalMode::OnFinished)
	{
		ScheduleProceduralElement(Element);
		UE_LOG(LogAmbiverseElementManager, VeryVerbose, TEXT("EvaluateFinishedElement: Rescheduled element: '%s'"), *Element->GetName());
	}
}

void UAmbiverseElementManager::Tick(const float DeltaTime)
{
	if (ScheduledProceduralElements.IsEmpty()) { return; }
	
	for (UAmbiverseElementInstance* ProceduralElement : ScheduledProceduralElements)
	{
		const float ScaleFactor {(ProceduralElement->Time - DeltaTime) / ProceduralElement->Time};
		ProceduralElement->ReferenceTime *= ScaleFactor;
		
		ProceduralElement->Time -= DeltaTime;

		if (ProceduralElement->Time <= 0)
		{
			if (!Owner || !ProceduralElement) { return; }
			
			if (UAmbiverseSoundSourceManager* SoundSourceManager {Owner->GetSoundSourceManager()})
			{
				SoundSourceManager->PlayElement(ProceduralElement);
			}
			
			ScheduleProceduralElement(ProceduralElement);
		}
	}
}

void UAmbiverseElementManager::ScheduleProceduralElement(UAmbiverseElementInstance* ProceduralElement)
{
	if (!ProceduralElement || !Owner) { return; }
	
	float DensityScalar {1.0f};

	if(UAmbiverseLayerAsset* Layer {ProceduralElement->AssociatedLayer}; 
		UAmbiverseParameterManager* ParameterManager {Owner->GetParameterManager()})
	{
		if (TArray<FAmbiverseParameterModifiers> Modifiers {Layer->Parameters}; !Modifiers.IsEmpty())
		{
			for (FAmbiverseParameterModifiers& ParameterModifiers : Modifiers)
			{
				DensityScalar = 1 / FMath::GetMappedRangeValueClamped(FVector2D(0, 1), ParameterModifiers.DensityRange,
					ParameterManager->GetParameterValue(ParameterModifiers.Parameter));
				DensityScalar = ParameterManager->GetParameterValue(ParameterModifiers.Parameter);
			}
		}
	}
	
	ProceduralElement->SetTime(DensityScalar);
}

void UAmbiverseElementManager::PrimeElements(TArray<UAmbiverseElementInstance*> Elements, uint16 PrimeCount)
{
	if (PrimeCount > 0 && Elements.Num() > 0)
	{
		for (int i {0}; i < PrimeCount; ++i)
		{
			UAmbiverseElementInstance* MinElement {Elements[0]};

			for (UAmbiverseElementInstance* ProceduralElement : Elements)
			{
				ScheduleProceduralElement(ProceduralElement);

				if (ProceduralElement->Time < MinElement->Time)
				{
					MinElement = ProceduralElement;
				}
			}

			for (UAmbiverseElementInstance* ProceduralElement : Elements)
			{
				if (ProceduralElement != MinElement)
				{
					ProceduralElement->Time -= MinElement->Time;
				}
			}
			ScheduleProceduralElement(MinElement);
		}
	}
}

