// Copyright (c) 2023-present Tim Verberne. All rights reserved.

#include "AmbiverseDistributorManager.h"
#include "AmbiverseDistributorAsset.h"
#include "AmbiverseSubsystem.h"

DEFINE_LOG_CATEGORY_CLASS(UAmbiverseDistributorManager, LogAmbiverseDistributorManager);

UAmbiverseDistributorAsset* UAmbiverseDistributorManager::GetDistributorByClass(TSubclassOf<UAmbiverseDistributorAsset> Class)
{
	if (!Owner) { return nullptr ; }
	
	for (UAmbiverseDistributorAsset* Distributor : Distributors)
	{
		if (Distributor->StaticClass() == Class)
		{
			UE_LOG(LogAmbiverseDistributorManager, VeryVerbose, TEXT("GetDistributorByClass: Found existing distributor of class: %s"), *Class->GetName());
			return Distributor;
		}
	}

	/** If no instance of the specified distributor class was found, we instance a new one and return it. */
	UAmbiverseDistributorAsset* Distributor = NewObject<UAmbiverseDistributorAsset>(this, Class.Get());
	UE_LOG(LogAmbiverseDistributorManager, Verbose, TEXT("GetDistributorByClass: Created new distributor of class: %s"), *Class->GetName());

	if (Distributor)
	{
		Distributor->Activate(Owner);
	}
	
	return Distributor;
}
