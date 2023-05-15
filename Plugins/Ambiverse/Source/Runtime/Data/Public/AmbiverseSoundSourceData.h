// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin

#pragma once

#include "MetasoundSource.h"
#include "AmbiverseSoundSourceData.generated.h"

/** Contains data that can be used by an AmbienceSoundSource instance. */
USTRUCT(BlueprintType)
struct FAmbiverseSoundSourceData
{
	GENERATED_USTRUCT_BODY()
	
	/** The MetaSoundSource to use for an AmbienceSoundSource. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound Source Data")
	UMetaSoundSource* Sound {nullptr};
	
	/** The volume to play an AmbienceSoundSource at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound Source Data")
	float Volume {1.0f};
	
	/** The transform to play an AmbienceSoundSource at. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Sound Source Data")
	FTransform Transform {FTransform()};
	
	/** Constructor with default values. */
	FAmbiverseSoundSourceData()
	{
	}

};
