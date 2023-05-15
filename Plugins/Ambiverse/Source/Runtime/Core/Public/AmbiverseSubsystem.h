// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AmbiverseSubsystem.generated.h"

class UAmbiverseSoundSourceManager;
class UAmbiverseLayer;

UCLASS(ClassGroup = "Ambiverse")
class AMBIVERSE_API UAmbiverseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAdaptiveAmbienceSystem, Log, All)

private:
	/** The sound source manager object. */
	UPROPERTY()
	UAmbiverseSoundSourceManager* SoundSourceManager {nullptr};
	
	/** The current active ambience layers. */
	UPROPERTY()
	TArray<UAmbiverseLayer*> ActiveLayers;

public:
	/** Adds a sound set*/
	void AddAmbienceLayer(UAmbiverseLayer* Layer);
	void PopAmbienceLayer(UAmbiverseLayer* Layer);

private:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	/** Processes an ambience event and updates the queue for an ambience layer.*/
	void ProcessAmbienceLayerQueue(UAmbiverseLayer* Layer);

	/** Checks if an ambience layer*/
	UAmbiverseLayer* FindActiveAmbienceLayer(const UAmbiverseLayer* LayerToFind) const;
};


