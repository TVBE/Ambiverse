// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseSoundDistributionData.h"

FTransform FAmbiverseSoundDistributionData::GetSoundTransform(const FAmbiverseSoundDistributionData& DistributionData,
	const FVector& ListenerLocation)
{
	FTransform Transform;

	const double Angle {FMath::RandRange(0.0, 2.0 * PI)};

	const double Radius {FMath::RandRange(DistributionData.HorizontalRange.X, DistributionData.HorizontalRange.Y)};

	const double X {Radius * FMath::Cos(Angle)};
	const double Y {Radius * FMath::Sin(Angle)};
	
	double Z {FMath::RandRange(DistributionData.VerticalRange * -0.5, DistributionData.VerticalRange * 0.5)};
	Z += DistributionData.VerticalOffset;

	const FVector Location {FVector(X, Y, Z) + ListenerLocation};

	Transform.SetLocation(Location);
	
	return Transform;
}
