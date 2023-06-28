// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseElementManager.h"
#include "AmbiverseDistributorAsset.h"
#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseDistributorManager.h"
#include "AmbiverseParameterManager.h"
#include "AmbiverseElementInstance.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseElementManager, LogAmbiverseElementManager);

void UAmbiverseElementManager::RegisterElements(TArray<UAmbiverseElementInstance*> Elements)
{
	if (Elements.IsEmpty()) { return; }
	
	PrimeElements(Elements);
	ScheduledProceduralElements.Append(Elements);
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
			PlayProceduralElement(ProceduralElement);
			ScheduleProceduralElement(ProceduralElement);
		}
	}
}

void UAmbiverseElementManager::ScheduleProceduralElement(UAmbiverseElementInstance* ProceduralElement)
{
	if (!ProceduralElement || !Owner) { return; }
	
	float DensityScalar {1.0f};

	if(UAmbiverseLayerAsset* Layer {ProceduralElement->OwningLayer}; 
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

void UAmbiverseElementManager::PlayProceduralElement(UAmbiverseElementInstance* ProceduralElement)
{
	if (!Owner)
	{
		UE_LOG(LogAmbiverseElementManager, Warning, TEXT("PlayProceduralElement: Owner  is invalid."))
		return;
	}
	
	if (!ProceduralElement)
	{
		UE_LOG(LogAmbiverseElementManager, Warning, TEXT("PlayProceduralElement: ProceduralElement is invalid."))
		return;
	}
	
	FVector CameraLocation;
	bool IsCameraValid {false};

	if (APlayerController* PlayerController {GetWorld()->GetFirstPlayerController()})
	{
		if (const APlayerCameraManager* CameraManager{PlayerController->PlayerCameraManager})
		{
			CameraLocation = CameraManager->GetCameraLocation();
			IsCameraValid = true;
		}
	}

	if (!IsCameraValid)
	{
		UE_LOG(LogAmbiverseElementManager, Error, TEXT("ProcessProceduralElement: Unable to obtain valid camera location."));
		return;
	}

	/** Prepare the sound source data. */
	FAmbiverseSoundSourceData SoundSourceData{FAmbiverseSoundSourceData()};

	UAmbiverseElementAsset* Element {ProceduralElement->RuntimeData.Element};

	if (!Element) { return; }

	SoundSourceData.Sound = Element->GetSound();
	SoundSourceData.Name = FName(Element->GetName());
	
	if (const TSubclassOf<UAmbiverseDistributorAsset> DistributorClass{Element->GetDistributorClass()})
	{
		UAmbiverseDistributorManager* DistributorManager {Owner->GetDistributorManager()};
		if (!DistributorManager)
		{
			UE_LOG(LogAmbiverseElementManager, Error, TEXT("ProcessProceduralElement: DistributorManager is nullptr."));
			return;
		}

		if (UAmbiverseDistributorAsset* Distributor {DistributorManager->GetDistributorByClass(DistributorClass)})
		{
			FTransform Transform{};
			if (Distributor->ExecuteDistribution(this, Transform, CameraLocation, Element))
			{
				SoundSourceData.Transform = Transform;
			}
		}
	}
	else
	{
		SoundSourceData.Transform = FAmbiverseSoundDistributionData::GetSoundTransform(
			Element->GetDistributionData(), CameraLocation);
	}

	UAmbiverseSoundSourceManager* SoundSourceManager {Owner->GetSoundSourceManager()};
	if (!SoundSourceManager)
	{
		UE_LOG(LogAmbiverseElementManager, Error, TEXT("ProcessProceduralElement: SoundSourceManager is nullptr."));
		return;
	}

	SoundSourceManager->InitiateSoundSource(SoundSourceData);
}
