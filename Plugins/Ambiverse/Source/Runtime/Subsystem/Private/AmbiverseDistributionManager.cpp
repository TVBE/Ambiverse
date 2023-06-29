// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseDistributionManager.h"
#include "AmbiverseDistributorAsset.h"
#include "AmbiverseElementInstance.h"
#include "AmbiverseElementAsset.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseDistributionManager, LogAmbiverseDistributionManager);

bool UAmbiverseDistributionManager::GetTransformForElement(FTransform& Transform, UAmbiverseElementInstance* ElementInstance)
{
	if (!ElementInstance) { return false; }

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
		UE_LOG(LogAmbiverseDistributionManager, Error, TEXT("ProcessProceduralElement: Unable to obtain valid camera location."));
		return false;
	}

	/** We first check if the element implements a custom distributor object. */
	if (const TSubclassOf<UAmbiverseDistributorAsset> DistributorClass {ElementInstance->RuntimeData.Element->GetDistributorClass()})
	{
		if (UAmbiverseDistributorAsset* Distributor {GetDistributorByClass(DistributorClass)})
		{
			if (Distributor->ExecuteDistribution(this, Transform, CameraLocation, ElementInstance->RuntimeData.Element))
			{
				return true;
			}
		}
	}
	else
	{
		const FAmbiverseSoundDistributionData& SoundDistributionData {ElementInstance->RuntimeData.Element->GetDistributionData()};

		switch (SoundDistributionData.DistributionMode)
		{
		case EDistributionMode::Random:
			
			Transform = FAmbiverseSoundDistributionData::GetSoundTransform(
			ElementInstance->RuntimeData.Element->GetDistributionData(), CameraLocation);
			return true;
		
		case EDistributionMode::Uniform:
			
			// TODO: Implement this.
			return false;

		case EDistributionMode::Static:

			/** There is an astronomical small change that this will evaluate to true if the vector is valid. */
			if (!ElementInstance->LastPlaylocation.IsZero())
			{
				const float Distance {static_cast<float>(FVector::Dist(CameraLocation, ElementInstance->LastPlaylocation))};
				if (Distance >= SoundDistributionData.Threshold)
				{
					Transform = FAmbiverseSoundDistributionData::GetSoundTransform(
					ElementInstance->RuntimeData.Element->GetDistributionData(), CameraLocation);
				}
				else
				{
					FVector Drift {
						FMath::RandRange(-0.5f * SoundDistributionData.Drift, 0.5f * SoundDistributionData.Drift), 
						FMath::RandRange(-0.5f * SoundDistributionData.Drift, 0.5f * SoundDistributionData.Drift), 
						0.0f
					};

					Transform.SetLocation(ElementInstance->LastPlaylocation + Drift);
				}
			}
			else
			{
				Transform = FAmbiverseSoundDistributionData::GetSoundTransform(
				ElementInstance->RuntimeData.Element->GetDistributionData(), CameraLocation);
			}
			return true;;
		}
	}
	return false;
};

UAmbiverseDistributorAsset* UAmbiverseDistributionManager::GetDistributorByClass(TSubclassOf<UAmbiverseDistributorAsset> Class)
{
	if (!Owner) { return nullptr ; }
	
	for (UAmbiverseDistributorAsset* Distributor : Distributors)
	{
		if (Distributor->StaticClass() == Class)
		{
			UE_LOG(LogAmbiverseDistributionManager, VeryVerbose, TEXT("GetDistributorByClass: Found existing distributor of class: %s"), *Class->GetName());
			return Distributor;
		}
	}

	/** If no instance of the specified distributor class was found, we instance a new one and return it. */
	UAmbiverseDistributorAsset* Distributor = NewObject<UAmbiverseDistributorAsset>(this, Class.Get());
	UE_LOG(LogAmbiverseDistributionManager, Verbose, TEXT("GetDistributorByClass: Created new distributor of class: %s"), *Class->GetName());

	if (Distributor)
	{
		Distributor->Activate(Owner);
	}
	
	return Distributor;
}


