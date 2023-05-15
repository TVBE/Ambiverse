
#include "AmbiverseProceduralSoundData.h"

DEFINE_LOG_CATEGORY(LogProceduralSoundData)

bool FAmbiverseProceduralSoundData::Validate(FAmbiverseProceduralSoundData& SoundData)
{
	if (!SoundData.Use) { return false; }
	
	if (SoundData.Sounds.Num() == 0)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("No sounds provided in the Procedural Sound Data."));
	}

	if (SoundData.DelayMin <= 0)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("DelayMin must be greater than 0."));
	}

	if (SoundData.DelayMax <= 0)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("DelayMax must be greater than 0."));
	}

	if (SoundData.DelayMax < SoundData.DelayMin)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("DelayMax must be greater than or equal to DelayMin."));
	}

	if(SoundData.DistributionData.VerticalRange < 0)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("VerticalRange must be greater than or equal to 0."));
	}

	if(SoundData.DistributionData.HorizontalRange.X > SoundData.DistributionData.HorizontalRange.Y)
	{
		SoundData.IsValid = false;
		UE_LOG(LogProceduralSoundData, Warning, TEXT("HorizontalRange X must be less than or equal to HorizontalRange Y."));
	}
	return SoundData.IsValid;
}