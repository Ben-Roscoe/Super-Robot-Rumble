// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "RobotHeroCharacterStart.h"
#include "GameFramework/Character.h"


//
// ARobotHeroCharacterStart::ARobotHeroCharacterStart
//
ARobotHeroCharacterStart::ARobotHeroCharacterStart( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	FollowPlayerID = EPlayerID::One;
	RootComponent->SetMobility( EComponentMobility::Type::Movable );

	TeleportHeight = 0.0f;
}


//
// ARobotHeroCharacterStart::BeginPlay
//
void ARobotHeroCharacterStart::BeginPlay()
{
	auto World = GetWorld();
	if( World != nullptr )
	{
		World->GetTimerManager().SetTimer( TeleportTimerHandle, this, &ARobotHeroCharacterStart::TeleportToRobotHero, SecondsBetweenTeleport, true );
	}
}


//
// ARobotHeroCharacterStart::TeleportToRobotHero
//
void ARobotHeroCharacterStart::TeleportToRobotHero()
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto ICTPGameMode			= World->GetAuthGameMode<AICTPGameGameMode>();
	if( ICTPGameMode == nullptr )
	{
		return;
	}

	// Get the player this start belongs to.
	auto RobotPlayerController = ICTPGameMode->GetRobotPlayer( FollowPlayerID );
	if( TryTeleportToRobotControllerPawn( RobotPlayerController ) )
	{
		return;
	}

	// If our target player is not alive for whatever reason, we'll find someone who is and move to them instead.
	for( int i = 0; i < ICTPGameMode->Players.Num(); ++i )
	{
		// Don't check this one again.
		if( i == static_cast<int32>( FollowPlayerID ) )
		{
			continue;
		}

		RobotPlayerController = ICTPGameMode->Players[i];
		if( TryTeleportToRobotControllerPawn( RobotPlayerController ) )
		{
			break;
		}
	}
}


//
// ARobotHeroCharacterStart::TryTeleportToRobotControllerPawn
//
bool ARobotHeroCharacterStart::TryTeleportToRobotControllerPawn( ARobotPlayerController* RobotPlayerController )
{
	if( RobotPlayerController != nullptr && RobotPlayerController->IsCurrentlyPlaying() )
	{
		// Don't teleport to the robot hero if it's falling.
		auto RobotPawn				= Cast<ARobotHeroCharacter>( RobotPlayerController->GetPawn() );
		if( RobotPawn == nullptr || RobotPawn->GetCharacterMovement()->IsFalling() || RobotPawn->IsInBadSpawn() )
		{
			return false;
		}

		SetActorTransform( RobotPawn->GetTransform() );

		// Add our height property to our location, so players don't spawn on top of each other.
		auto HeightAdjustedLocation  = GetActorLocation();
		HeightAdjustedLocation.Z	+= TeleportHeight;
		SetActorLocation( HeightAdjustedLocation );

		return true;
	}
	return false;
}


