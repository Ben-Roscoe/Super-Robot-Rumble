// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "WeaponComponent.h"



//
// UWeaponComponent::UWeaponComponent
//
UWeaponComponent::UWeaponComponent()
{
	SecondsBetweenRounds = 0.1f;
	SecondsBetweenBursts = 0.0f;
	RoundsInBurst		 = 1;
	bIsTriggerPressed	 = false;
}


//
// UWeaponComponent::PressTrigger
//
void UWeaponComponent::PressTrigger()
{
	if( bIsTriggerPressed )
	{
		return;
	}
	bIsTriggerPressed	= true;
	OnTriggerPressed.Broadcast();

	if( !bIsFiring )
	{
		bIsFirstBurst		= true;
		bIsFiring			= true;
		StartBurst();
	}
}


//
// UWeaponComponent::ReleaseTrigger
//
void UWeaponComponent::ReleaseTrigger()
{
	if( !bIsTriggerPressed )
	{
		return;
	}
	bIsTriggerPressed = false;
	OnTriggerReleased.Broadcast();
}


//
// UWeaponComponent::Fire
//
void UWeaponComponent::Fire()
{
	// Make sure owner is valid.
	auto Owner = GetOwner();
	if( Owner == nullptr || Owner->IsPendingKillPending() )
	{
		return;
	}

	if( bIsTriggerPressed )
	{
		++RoundsFiredThisBurst;
		OnFire.Broadcast();
	}

	// Wait for another burst.
	if( RoundsFiredThisBurst >= RoundsInBurst )
	{
		auto World = GetWorld();
		if( World == nullptr )
		{
			return;
		}

		// Clear fire timer.
		World->GetTimerManager().ClearTimer( FireTimerHandle );

		if( SecondsBetweenBursts <= 0.0f )
		{
			StartBurst();
		}
		else
		{
			// Wait for burst delay.
			World->GetTimerManager().SetTimer( BurstTimerHandle, this, &UWeaponComponent::StartBurst, SecondsBetweenBursts, false );
		}
	}
}


//
// UWeaponComponent::StartBurst
//
void UWeaponComponent::StartBurst()
{
	if( !bIsTriggerPressed )
	{
  		EndFire();
		return;
	}

	RoundsFiredThisBurst = 0;

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	if( !bIsFirstBurst )
	{
		// Fire rounds repeating until RoundsInBurst is reached.
		World->GetTimerManager().SetTimer( FireTimerHandle, this, &UWeaponComponent::Fire, SecondsBetweenRounds, true );
	}
	else
	{
		// We want the first burst of a trigger press to be instant.
		bIsFirstBurst = false;
		World->GetTimerManager().SetTimer( FireTimerHandle, this, &UWeaponComponent::Fire, SecondsBetweenRounds, true, 0.0f );
	}
}


//
// UWeaponComponent::EndFire
//
void UWeaponComponent::EndFire()
{
	auto World = GetWorld();
	check( World );

	World->GetTimerManager().ClearTimer( FireTimerHandle );
	World->GetTimerManager().ClearTimer( BurstTimerHandle );
	RoundsFiredThisBurst = 0;

	bIsFiring = false;
}




