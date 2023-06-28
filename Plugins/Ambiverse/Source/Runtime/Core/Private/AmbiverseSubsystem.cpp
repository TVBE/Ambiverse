// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseSubsystem.h"
#include "AmbiverseDistributor.h"
#include "AmbiverseDistributorManager.h"
#include "AmbiverseElement.h"
#include "AmbiverseLayer.h"
#include "AmbiverseLayerManager.h"
#include "AmbiverseParameterManager.h"
#include "AmbiverseProceduralElement.h"
#include "AmbiverseSoundSourceData.h"
#include "AmbiverseSoundSourceManager.h"
#include "AmbiverseVisualisationComponent.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseSubsystem, LogAmbiverseSubsystem);

UAmbiverseSubsystem::UAmbiverseSubsystem()
{
}

void UAmbiverseSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LayerManager = NewObject<UAmbiverseLayerManager>(this);
	if (LayerManager) { LayerManager->Initialize(this); }
	
	ParameterManager = NewObject<UAmbiverseParameterManager>(this);
	if (ParameterManager) { ParameterManager->Initialize(this); }

	SoundSourceManager = NewObject<UAmbiverseSoundSourceManager>(this);
	if (SoundSourceManager) { SoundSourceManager->Initialize(this); }

	DistributorManager = NewObject<UAmbiverseDistributorManager>(this);
	if (DistributorManager) { DistributorManager->Initialize(this); }

#if !UE_BUILD_SHIPPING
	VisualisationComponent.Reset(NewObject<UAmbiverseVisualisationComponent>(this));
#endif
}

void UAmbiverseSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UE_LOG(LogAmbiverseSubsystem, Log, TEXT("Adaptive Ambience System initialized successfully."))
}

void UAmbiverseSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LayerManager && LayerManager->IsInitialized)
	{
		LayerManager->Tick(DeltaTime);
	}
}

void UAmbiverseSubsystem::PlayProceduralElement(UAmbiverseProceduralElement* ProceduralElement)
{
	if (!ProceduralElement)
	{
		UE_LOG(LogAmbiverseSubsystem, Warning, TEXT("PlayProceduralElement: ProceduralElement is invalid."))
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
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: Unable to obtain valid camera location."));
		return;
	}

	/** Prepare the sound source data. */
	FAmbiverseSoundSourceData SoundSourceData{FAmbiverseSoundSourceData()};

	UAmbiverseElement* Element {ProceduralElement->ProceduralElementData.Element};

	if (!Element) { return; }

	SoundSourceData.Sound = Element->GetSound();
	SoundSourceData.Name = FName(Element->GetName());
	
	if (const TSubclassOf<UAmbiverseDistributor> DistributorClass{Element->GetDistributorClass()})
	{
		if (!DistributorManager)
		{
			UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: DistributorManager is nullptr."));
			return;
		}

		if (UAmbiverseDistributor* Distributor{DistributorManager->GetDistributorByClass(DistributorClass)})
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
	
	if (!SoundSourceManager)
	{
		UE_LOG(LogAmbiverseSubsystem, Error, TEXT("ProcessProceduralElement: SoundSourceManager is nullptr."));
		return;
	}

	SoundSourceManager->InitiateSoundSource(SoundSourceData);
}

// TODO: re-implement this.
/*void UAmbiverseSubsystem::HandleParameterChanged()
{
	if (!LayerManager || LayerManager->GetLayerRegistry().IsEmpty()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Parameter Changed!"))

	TArray<UAmbiverseLayer*> Layers {LayerManager->GetLayerRegistry()};
	for (UAmbiverseLayer* Layer : Layers)
	{
		for (FAmbiverseProceduralElementData Entry : Layer->ProceduralElementData)
		{
			float DensityScalar {1.0f};
			float VolumeScalar {1.0f};
			
			ParameterManager->GetScalarsForElement(DensityScalar, VolumeScalar, Layer, Entry);

			Entry.Time = Entry.ReferenceTime * DensityScalar;
		}
	}
}*/

void UAmbiverseSubsystem::Deinitialize()
{
	if (LayerManager)
	{
		LayerManager->Deinitialize(this);
		LayerManager = nullptr;
	}
	if (ParameterManager)
	{
		ParameterManager->Deinitialize(this);
		SoundSourceManager = nullptr;
	}
	if (SoundSourceManager)
	{
		SoundSourceManager->Deinitialize(this);
		SoundSourceManager = nullptr;
	}
	if (DistributorManager)
	{
		DistributorManager->Deinitialize(this);
		DistributorManager = nullptr;
	}

#if !UE_BUILD_SHIPPING
	if (VisualisationComponent.IsValid())
	{
		VisualisationComponent->ConditionalBeginDestroy();
		VisualisationComponent.Reset();
	}
#endif
	
	Super::Deinitialize();
}


