// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "ICTPGameGameMode.h"
#include "TeamComponent.h"
#include "ScoreComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "WidgetBlueprintLibrary.h"

#include "DrawDebugHelpers.h"


//
// AICTPGameGameMode::AICTPGameGameMode
//
AICTPGameGameMode::AICTPGameGameMode()
{
	RobotPlayerControllerClass	= ARobotPlayerController::StaticClass();

	RespawnTime					= 3.0f;
	DeathScorePenalty			= 500;
}


//
// AICTPGameGameMode::BeginPlay
//
void AICTPGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	bHasLevelEndTriggered = false;

	PlayerControllerClass = RobotPlayerControllerClass;
	Players.Add( InitialisePlayerController( EPlayerID::One ) );
	Players.Add( InitialisePlayerController( EPlayerID::Two ) );
	Players.Add( InitialisePlayerController( EPlayerID::Three ) );
	Players.Add( InitialisePlayerController( EPlayerID::Four ) );

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	if( RobotCameraActorClass != nullptr )
	{
		RobotCameraActor = World->SpawnActor<ARobotCameraActor>( RobotCameraActorClass );
		auto PlayerOne = Cast<ARobotPlayerController>( UGameplayStatics::GetPlayerController( World, static_cast<int32>( EPlayerID::One ) ) );
		if( RobotCameraActor != nullptr && PlayerOne != nullptr )
		{
			PlayerOne->SetViewTarget( RobotCameraActor );
		}
	}
}


//
// AICTPGameGameMode::RegisterActorDeath
//
void AICTPGameGameMode::RegisterActorDeath( float DamageAmount, AActor* DamagedActor, AActor* DamageCauser )
{
	OnActorDeath.Broadcast( DamageAmount, DamagedActor, DamageCauser );

	auto DamagerPlayerController = Cast<ARobotPlayerController>( DamageCauser->GetInstigatorController() );
	if( DamagerPlayerController == nullptr )
	{
		return;
	}

	auto ScoreComponent = DamagedActor->FindComponentByClass<UScoreComponent>();
	if( ScoreComponent == nullptr )
	{
		return;
	}

	DamagerPlayerController->AddScore( ScoreComponent->GetScore() );
}


//
// AICTPGameGameMode::OnRobotCharacterDeath
//
void AICTPGameGameMode::OnRobotCharacterDeath( ARobotPlayerController* RobotOwner )
{
	if( RobotOwner == nullptr )
	{
		return;
	}
	RobotOwner->AddScore( -DeathScorePenalty );
}


//
// AICTPGameGameMode::TriggerLevelEnd
//
void AICTPGameGameMode::TriggerLevelEnd( FName NextLevelName )
{
	NextLevel				= NextLevelName;
	bHasLevelEndTriggered	= true;
}


//
// AICTPGameGameMode::GoToNextLevel
//
void AICTPGameGameMode::GoToNextLevel()
{
	UGameplayStatics::OpenLevel( GetWorld(), NextLevel );
}


//
// AICTPGameGameMode::TryDamage
//
void AICTPGameGameMode::TryDamage( AActor* Damager, AActor* Damagee, float DamageAmount, TSubclassOf<UDamageType> DamageTypeClass ) const
{
	if( CanDamage( Damager, Damagee ) )
	{
		UGameplayStatics::ApplyDamage( Damagee, DamageAmount, Damager->GetInstigatorController(), Damager, DamageTypeClass );
	}
}


//
// AICTPGameGameMode::TryRadialDamage
//
void AICTPGameGameMode::TryRadialDamage( AActor* Damager, float DamageAmount, float DamageRadius, FVector Origin, TSubclassOf<UDamageType> DamageTypeClass ) const
{
	auto World = GetWorld();
	check( World );

	// Make sure we ignore actors on our team.
	TArray<AActor*>		   IgnoreActors;
	TArray<FOverlapResult> Overlaps;
	FCollisionQueryParams  Params;
	World->OverlapMultiByObjectType( Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams( FCollisionObjectQueryParams::AllDynamicObjects ), FCollisionShape::MakeSphere( DamageRadius ), Params );
	for( int i = 0; i < Overlaps.Num(); ++i )
	{
		auto Actor = Overlaps[i].Actor.Get();
		if( !CanDamage( Damager, Actor ) )
		{
			IgnoreActors.Add( Actor );
		}
	}

	// Apply radial damage on non-ignored actors.
	UGameplayStatics::ApplyRadialDamage( World, DamageAmount, Origin, DamageRadius, DamageTypeClass, IgnoreActors, Damager, 
		Damager == nullptr ? nullptr : Damager->GetInstigatorController() ); 
}


//
// AICTPGameGameMode::CanDamage
//
bool AICTPGameGameMode::CanDamage( AActor* Damager, AActor* Damagee ) const
{
	if( Damager == nullptr || Damagee == nullptr )
	{
		return false;
	}

	// Do they have instigators? If not, make the instigator the actor themselves.
	auto DamagerInstigator = Damager->GetInstigatorController();
	auto DamageeInstigator = Damagee->GetInstigatorController();

	if( DamagerInstigator == nullptr || DamageeInstigator == nullptr )
	{
		return true;
	}

	// Get team components on either the instigator controller, or the actor.
	auto DamagerTeamComponent = DamagerInstigator->FindComponentByClass<UTeamComponent>();
	auto DamageeTeamComponent = DamageeInstigator->FindComponentByClass<UTeamComponent>();

	if( DamagerTeamComponent == nullptr || DamageeTeamComponent == nullptr )
	{
		return true;
	}

	return UTeamComponent::AreEnemies( DamagerTeamComponent, DamageeTeamComponent );
}


//
// AICTPGameGameMode::GetWinner
//
ARobotPlayerController* AICTPGameGameMode::GetWinner() const
{
	if( Players.Num() <= 0 )
	{
		return nullptr;
	}

	ARobotPlayerController* Highest = Players[0];
	for( int i = 1; i < Players.Num(); ++i )
	{
		if( Players[i]->GetIsIn() && Players[i]->GetScore() > Highest->GetScore() )
		{
			Highest = Players[i];
		}
	}
	return Highest;
}


//
// AICTPGameGameMode::IsAnyonePlaying
//
bool AICTPGameGameMode::IsAnyonePlaying() const
{
	for( int i = 0; i < Players.Num(); ++i )
	{
		if( Players[i] != nullptr && Players[i]->GetIsIn() )
		{
			return true;
		}
	}
	return false;
}


//
// AICTPGameGameMode::GetRespawnSecondsForPlayer
//
float AICTPGameGameMode::GetRespawnSecondsForPlayer( const ARobotPlayerController* RobotPlayerController ) const
{
	return RespawnTime;
}


//
// AICTPGameGameMode::GetRobotPlayer
//
ARobotPlayerController* AICTPGameGameMode::GetRobotPlayer( EPlayerID PlayerID ) const
{
	int32 Index = static_cast<int32>( PlayerID );
	if( Index >= Players.Num() || Index < 0 )
	{
		return nullptr;
	}
	return Players[Index];
}


//
// AICTPGameGameMode::GetColourFromPlayerID
//
FColor AICTPGameGameMode::GetColourFromPlayerID( EPlayerID PlayerID )
{
	switch( PlayerID )
	{
		case EPlayerID::One:
			return PlayerOneColour;
		case EPlayerID::Two:
			return PlayerTwoColour;
		case EPlayerID::Three:
			return PlayerThreeColour;
		case EPlayerID::Four:
		default:
			return PlayerFourColour;
	}
}


//
// AICTPGameGameMode::SetGamePaused
//
void AICTPGameGameMode::SetGamePaused( bool bPaused ) const
{
	auto PlayerOne = GetRobotPlayer( EPlayerID::One );
	if( PlayerOne == nullptr )
	{
		return;
	}

	PlayerOne->SetPause( bPaused );
}


//
// AICTPGameGameMode::EnableInGamePause
//
void AICTPGameGameMode::EnableInGamePause()
{
	if( bIsInGamePaused || MatchState != MatchState::InProgress )
	{
		return;
	}
	bIsInGamePaused = true;

	SetGamePaused( true );
	if( InGamePauseWidgetClass == nullptr )
	{
		return;
	}

	InGameWidget->RemoveFromViewport();

	InGamePauseWidget = CreateWidget<UUserWidget>( GetWorld(), InGamePauseWidgetClass );
	InGamePauseWidget->AddToViewport();
}


//
// AICTPGameGameMode::DisableInGamePause
//
void AICTPGameGameMode::DisableInGamePause()
{
	if( !bIsInGamePaused )
	{
		return;
	}
	bIsInGamePaused = false;
	SetGamePaused( false );

	if( InGamePauseWidget == nullptr )
	{
		return;
	}

	InGamePauseWidget->RemoveFromViewport();
	InGamePauseWidget = nullptr;

	InGameWidget->AddToViewport();

	UWidgetBlueprintLibrary::SetFocusToGameViewport();
}


//
// AICTPGameGameMode::HandleMatchHasStarted
//
void AICTPGameGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	// Spawn in game UI.
	if( InGameWidgetClass != nullptr )
	{
		InGameWidget = CreateWidget<UUserWidget>( GetWorld(), InGameWidgetClass );
		check( InGameWidget );

		InGameWidget->AddToViewport();
	}
}


//
// AICTPGameGameMode::HandleMatchHasEnded
//
void AICTPGameGameMode::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();

	// Clear the in progress UI.
	if( InGameWidget != nullptr )
	{
		InGameWidget->RemoveFromViewport();
		InGameWidget = nullptr;
	}

	SetGamePaused( true );

	// Add the match results widget.
	if( MatchEndResultsWidgetClass != nullptr )
	{
		MatchEndResultsWidget = CreateWidget<UUserWidget>( GetWorld(), MatchEndResultsWidgetClass );
		check( MatchEndResultsWidget );

		MatchEndResultsWidget->AddToViewport();
	}	
}


//
// AICTPGameGameMode::HandleLeavingMap
//
void AICTPGameGameMode::HandleLeavingMap()
{
	Super::HandleLeavingMap();

	// Remove match results widget.
	if( MatchEndResultsWidget != nullptr )
	{
		MatchEndResultsWidget->RemoveFromViewport();
		MatchEndResultsWidget = nullptr;
	}
}


//
// AICTPGameGameMode::ReadyToEndMatch
//
bool AICTPGameGameMode::ReadyToEndMatch_Implementation()
{
	return bHasLevelEndTriggered;
}


//
// AICTPGameGameMode::InitialisePlayerController
//
ARobotPlayerController* AICTPGameGameMode::InitialisePlayerController( EPlayerID PlayerID )
{
	auto World = GetWorld();
	check( World );

	// Get or create the player controller.
	auto PlayerController = UGameplayStatics::GetPlayerController( World, static_cast<int32>( PlayerID ) );
	if( PlayerController == nullptr )
	{
		// Try and create the player controller, since it doesn't appear to exist.
		PlayerController = UGameplayStatics::CreatePlayer( World, static_cast<int32>( PlayerID ), true );
	}
	check( PlayerController );

	auto RobotPlayerController = Cast<ARobotPlayerController>( PlayerController );
	if( RobotPlayerController == nullptr )
	{
		return nullptr;
	}
	//check( RobotPlayerController );

	FColor Colour = GetColourFromPlayerID( PlayerID );
	RobotPlayerController->SetColour( Colour );
	RobotPlayerController->SetScore( 0 );

	return RobotPlayerController;
}