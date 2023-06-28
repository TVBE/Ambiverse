// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AmbiverseSubsystem.generated.h"

class UAmbiverseProceduralElement;
class UAmbiverseVisualisationComponent;
class UAmbiverseDistributorManager;
class UAmbiverseLayerManager;
class UAmbiverseParameterManager;
class UAmbiverseSoundSourceManager;
class UAmbiverseLayer;

UCLASS(Transient, ClassGroup = "Ambiverse")
class AMBIVERSE_API UAmbiverseSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

	DECLARE_LOG_CATEGORY_CLASS(LogAmbiverseSubsystem, Log, All)

private:
	UPROPERTY()
	UAmbiverseLayerManager* LayerManager {nullptr};
	
	UPROPERTY()
	UAmbiverseParameterManager* ParameterManager {nullptr};

	UPROPERTY()
	UAmbiverseSoundSourceManager* SoundSourceManager {nullptr};

	UPROPERTY()
	UAmbiverseDistributorManager* DistributorManager {nullptr};

#if !UE_BUILD_SHIPPING
	TStrongObjectPtr<UAmbiverseVisualisationComponent> VisualisationComponent {nullptr};
#endif

public:
	UAmbiverseSubsystem();
	
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UAmbiverseSubsystem, STATGROUP_Tickables);
	}
	
	UFUNCTION()
	void PlayProceduralElement(UAmbiverseProceduralElement* ProceduralElement);

private:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	virtual void Tick(float DeltaTime) override;
	
	//UFUNCTION()
	//void HandleParameterChanged();

public:
	FORCEINLINE UAmbiverseLayerManager* GetLayerManager() const { return LayerManager; }
	FORCEINLINE UAmbiverseParameterManager* GetParameterManager() const { return ParameterManager; }
	FORCEINLINE UAmbiverseSoundSourceManager* GetSoundSourceManager() const { return SoundSourceManager; }
	FORCEINLINE UAmbiverseDistributorManager* GetDistributorManager() const { return DistributorManager; }
};



