// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin

#pragma once

#include "AmbiverseSoundDistributionData.generated.h"

/** Defines the area around the player that an AmbienceSoundSource can play in. */
USTRUCT(BlueprintType)
struct FAmbiverseSoundDistributionData
{
	GENERATED_USTRUCT_BODY()
	
	/** Defines the horizontal range in which a sound source can play in relation to the player. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Play Range", Meta = (ClampMin = "0"))
	FVector2D HorizontalRange {FVector2D(500, 1000)};

	/** Defines a vertical offset. This can be used to ensure that the sound is always played above or below the player. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Play Range")
	float VerticalOffset {0.0f};
	
	/** Defines the vertical range in which a sound source can play in relation to the player. Is not affected by ExclusionRadius. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Play Range")
	float VerticalRange {100.0f};
	
	static FTransform GetSoundTransform(const FAmbiverseSoundDistributionData& DistributionData);
};

inline FTransform FAmbiverseSoundDistributionData::GetSoundTransform(const FAmbiverseSoundDistributionData& DistributionData)
{
	FTransform Transform;

	const double X {FMath::RandRange(DistributionData.HorizontalRange.X, DistributionData.HorizontalRange.Y)};
	const double Y {FMath::RandRange(DistributionData.HorizontalRange.X, DistributionData.HorizontalRange.Y)};
	
	double Z {FMath::RandRange(DistributionData.VerticalRange * 0.5, DistributionData.VerticalRange * -0.5)};
	Z += DistributionData.VerticalOffset;

	Transform.SetLocation(FVector(X, Y, Z));
	return Transform;
}
