// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseProceduralSoundData.h"
#include "AmbiverseLayer.generated.h"

class UAmbiverseSoundSourceManager;
USTRUCT()
struct FAmbiverseLayerQueueEntry
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FAmbiverseProceduralSoundData SoundData {FAmbiverseProceduralSoundData()};

	UPROPERTY()
	float Time {0.0f};
};

UCLASS(Blueprintable, BlueprintType, ClassGroup = "Ambiverse", Meta = (DisplayName = "Ambiverse Layer"))
class AMBIVERSE_API UAmbiverseLayer : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbienceLayer, Log, All)

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (TitleProperty = "Name"))
	TArray<FAmbiverseProceduralSoundData> ProceduralSounds;

	UPROPERTY()
	FTimerHandle TimerHandle;

	UPROPERTY()
	TArray<FAmbiverseLayerQueueEntry> SoundQueue;

public:
	bool GetEntryWithLowestTime(FAmbiverseLayerQueueEntry& OutEntry);

	void SubtractTimeFromQueue(const float TimeToSubtract);
	
	void SortQueueDataByTime();
};


