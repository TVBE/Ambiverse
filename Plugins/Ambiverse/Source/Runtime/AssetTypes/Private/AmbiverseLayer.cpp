// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseLayer.h"

#include "AmbiverseParameterManager.h"
#include "AmbiverseProceduralElement.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseLayer, LogAmbiverseLayer);

bool UAmbiverseLayer::Initialize(UAmbiverseSubsystem* Subsystem)
{
	if (!Subsystem) { return false; }
	AmbiverseSubsystem = Subsystem;
	
	ProceduralElementData.RemoveAll([](const FAmbiverseProceduralElementData& Element){ return !Element.IsValid(); });
	
	if (ProceduralElementData.Num() > 0)
	{
		for(const FAmbiverseProceduralElementData& ElementData : ProceduralElementData)
		{
			UAmbiverseProceduralElement* NewElement = NewObject<UAmbiverseProceduralElement>(this, UAmbiverseProceduralElement::StaticClass());
			
			NewElement->ProceduralElementData = ElementData;
			
			this->ProceduralElements.Add(NewElement);
		}

		Prime();

		IsInitialized = true;
	}
	else
	{
		IsInitialized = false;
	}
	return IsInitialized;
}

void UAmbiverseLayer::Update(float DeltaTime)
{
	ActiveDuration += DeltaTime;
	if (EnableLifetime)
	{
		if (Lifetime != 0.0f)
		{
			LifetimeRatio = ActiveDuration / Lifetime;
		}
		else
		{
			LifetimeRatio = 0.0f;
		}
	}

	if (!AmbiverseSubsystem) {return; }
	
	if (ProceduralElements.IsEmpty()) { return; }
	
	for (UAmbiverseProceduralElement* ProceduralElement : ProceduralElements)
	{
		const float ScaleFactor {(ProceduralElement->Time - DeltaTime) / ProceduralElement->Time};
		ProceduralElement->ReferenceTime *= ScaleFactor;
		
		ProceduralElement->Time -= DeltaTime;

		if (ProceduralElement->Time <= 0)
		{
			AmbiverseSubsystem->PlayProceduralElement(ProceduralElement);
			SetNewTimeForProceduralElement(ProceduralElement);
		}
	}
}

void UAmbiverseLayer::Prime(const uint16 Count)
{
	if (Count > 0 && ProceduralElements.Num() > 0)
	{
		for (int i {0}; i < Count; ++i)
		{
			UAmbiverseProceduralElement* MinElement {ProceduralElements[0]};

			for (UAmbiverseProceduralElement* ProceduralElement : ProceduralElements)
			{
				SetNewTimeForProceduralElement(ProceduralElement);

				if (ProceduralElement->Time < MinElement->Time)
				{
					MinElement = ProceduralElement;
				}
			}

			for (UAmbiverseProceduralElement* Element : ProceduralElements)
			{
				if (Element != MinElement)
				{
					Element->Time -= MinElement->Time;
				}
			}

			SetNewTimeForProceduralElement(MinElement);
		}
	}
}

void UAmbiverseLayer::SetNewTimeForProceduralElement(UAmbiverseProceduralElement* ProceduralElement)
{
	if (!ProceduralElement) { return; }
	
	float DensityScalar {1.0f};
	float VolumeScalar {1.0f};
				
	if (AmbiverseSubsystem)
	{
		if (UAmbiverseParameterManager* ParameterManager {AmbiverseSubsystem->GetParameterManager()})
		{
			ParameterManager->GetScalarsForLayer(DensityScalar, VolumeScalar, this);
		}
		
	}
	ProceduralElement->SetTime(DensityScalar);
}

#if WITH_EDITOR
void UAmbiverseLayer::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(PropertyChangedEvent.Property != nullptr)
	{
		const FName PropertyName = PropertyChangedEvent.Property->GetFName();
		
		if(PropertyName == GET_MEMBER_NAME_CHECKED(UAmbiverseLayer, ProceduralElementData))
		{
			for(FAmbiverseProceduralElementData& Element : ProceduralElementData)
			{
				if(Element.IntervalRange.X > Element.IntervalRange.Y)
				{
					Element.IntervalRange.X = Element.IntervalRange.Y;
				}
			}
		}
	}
}
#endif

