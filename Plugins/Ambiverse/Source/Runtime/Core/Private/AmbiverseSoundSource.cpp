// Copyright (c) 2022-present Tim Verberne
// This source code is part of the Adaptive Ambience System plugin

#include "AmbiverseSoundSource.h"

DEFINE_LOG_CATEGORY_CLASS(AAmbiverseSoundSource, LogAmbiverseSoundSource);

AAmbiverseSoundSource::AAmbiverseSoundSource()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	if (AudioComponent)
	{
		RootComponent = AudioComponent;
	}
}

void AAmbiverseSoundSource::Initialize(UAmbiverseSoundSourceManager* Manager,
	FAmbiverseSoundSourceData& Data)
{
	if (!Manager)
	{
		return;
	}
	SoundSourceManager = Manager;

	if (!Data.Sound)
	{
		return;
	}
	SoundSourceData = Data;
	
	SetSound(SoundSourceData.Sound);
	SetActorTransform(SoundSourceData.Transform);
	SetVolume(SoundSourceData.Volume);
	SoundSourceName = SoundSourceData.Name;
	AmbiverseLayer = SoundSourceData.Layer;

	if(AudioComponent)
	{
		AudioComponent->Play();
	}
}

void AAmbiverseSoundSource::BeginPlay()
{
	Super::BeginPlay();

	if(AudioComponent)
	{
		AudioComponent->OnAudioFinished.AddDynamic(this, &AAmbiverseSoundSource::HandleOnAudioFinishedPlaying);
	}
	
}

void AAmbiverseSoundSource::HandleOnAudioFinishedPlaying()
{
	
}


void AAmbiverseSoundSource::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

