// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "AmbiverseSoundDistributionData.h"
#include "MetasoundSource.h"
#include "AmbiverseSoundSource.h"
#include "AmbiverseElementAsset.generated.h"

class UAmbiverseDistributorAsset;

/** An ambiverse element is a single procedural sound. It can be played directly, or used in a layer to create a procedural soundscape. */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "Ambiverse", Meta = (DisplayName = "Ambiverse Element",
	ShortToolTip = "A single sound element that can be used inside an Ambiverse Layer"))
class AMBIVERSE_API UAmbiverseElementAsset : public UObject
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseElement, Log, All)

private:
	UPROPERTY(EditAnywhere, Category = "Sound")
	TSoftObjectPtr<UMetaSoundSource> Sound;
	
	/** The distributor to use for this element. */
	UPROPERTY(EditAnywhere, Category = "Distribution", Meta = (BlueprintBaseOnly))
	TSubclassOf<UAmbiverseDistributorAsset> DistributorClass;
	
	/** PlayRange data for an AmbienceSystem preset entry. */
	UPROPERTY(EditAnywhere, Category = "Distribution", Meta = (EditCondition = "DistributorClass == nullptr"))
	FAmbiverseSoundDistributionData DistributionData;

	/** The SoundSource class to use to for this element. */
	UPROPERTY(EditAnywhere, Category = "Sound Source")
	TSubclassOf<AAmbiverseSoundSource> SoundSourceClass {AAmbiverseSoundSource::StaticClass()};

public:
	bool IsValid {true};

	FORCEINLINE UMetaSoundSource* GetSound()
	{
		if (!Sound.IsValid()) { Sound.LoadSynchronous(); }
		return Sound.Get();
	}
	FORCEINLINE TSubclassOf<UAmbiverseDistributorAsset> GetDistributorClass() const { return DistributorClass; }
	FORCEINLINE FAmbiverseSoundDistributionData GetDistributionData() const { return DistributionData; }
	FORCEINLINE TSubclassOf<AAmbiverseSoundSource> GetSoundSourceClass() const { return SoundSourceClass; }
};
