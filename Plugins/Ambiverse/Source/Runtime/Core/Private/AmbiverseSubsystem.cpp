// Copyright 2023 Nino Saglia & Tim Verberne

#include "AmbiverseSubsystem.h"
#include "AmbiverseLayer.h"
#include "AmbiverseSoundSourceManager.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseSubsystem, LogAdaptiveAmbienceSystem);

void UAmbiverseSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	SoundSourceManager = NewObject<UAmbiverseSoundSourceManager>(this);

	UE_LOG(LogAdaptiveAmbienceSystem, Log, TEXT("Adaptive Ambience System initialized successfully."))
}

void UAmbiverseSubsystem::AddAmbienceLayer(UAmbiverseLayer* Layer)
{
	if (!Layer)
	{
		UE_LOG(LogAdaptiveAmbienceSystem, Warning, TEXT("AddAmbienceLayer: No Layer provided."));
		return;
	}

	if (Layer->ProceduralSounds.IsEmpty())
	{
		UE_LOG(LogAdaptiveAmbienceSystem, Warning, TEXT("AddAmbienceLayer: Layer has no procedural sounds: '%s'."), *Layer->GetName());
		return;
	}

	for (FAmbiverseProceduralSoundData& SoundData : Layer->ProceduralSounds)
	{
		FAmbiverseProceduralSoundData::Validate(SoundData);
	}

	bool HasValidSoundData = false;
	for (const FAmbiverseProceduralSoundData& SoundData : Layer->ProceduralSounds)
	{
		if (SoundData.IsValid)
		{
			HasValidSoundData = true;
			break;
		}
	}

	if (!HasValidSoundData)
	{
		UE_LOG(LogAdaptiveAmbienceSystem, Warning, TEXT("AddAmbienceLayer: Layer has no valid procedural sounds: '%s'."), *Layer->GetName());
		return;
	}

	
	if (!FindActiveAmbienceLayer(Layer))
	{
		ActiveLayers.Add(Layer);
		UE_LOG(LogAdaptiveAmbienceSystem, Verbose, TEXT("AddAmbienceLayer: Layer added successfully: '%s'."), *Layer->GetName());
	}
}

void UAmbiverseSubsystem::PopAmbienceLayer(UAmbiverseLayer* Layer)
{
}

UAmbiverseLayer* UAmbiverseSubsystem::FindActiveAmbienceLayer(const UAmbiverseLayer* LayerToFind) const
{
	for (UAmbiverseLayer* Layer : ActiveLayers)
	{
		if (Layer == LayerToFind)
		{
			return Layer;
		}
	}

	return nullptr;
}

void UAmbiverseSubsystem::ProcessAmbienceLayerQueue(UAmbiverseLayer* Layer)
{
	if (!Layer) { return; }
	if (FAmbiverseLayerQueueEntry Entry; Layer->GetEntryWithLowestTime(Entry))
	{
		
		Entry.Time = FMath::RandRange(Entry.SoundData.DelayMin, Entry.SoundData.DelayMax);

		FVector CameraLocation;
		bool IsCameraValid {false};
		
		if (APlayerController* PlayerController {GetWorld()->GetFirstPlayerController()})
		{
			if (const APlayerCameraManager* CameraManager {PlayerController->PlayerCameraManager})
			{
				CameraLocation = CameraManager->GetCameraLocation();
				IsCameraValid = true;
			}
		}

		if (!IsCameraValid) { return; }

		// FTransform SoundTransform {FSoundDistributionData::GetSoundTransform(Entry.SoundData.DistributionData) + FTransform(CameraLocation)};
	}
}




