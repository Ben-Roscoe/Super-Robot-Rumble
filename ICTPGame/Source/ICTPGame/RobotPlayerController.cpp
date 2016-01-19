// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "RobotPlayerController.h"
#include "ICTPGameGameMode.h"
#include "HealthComponent.h"
#include "TeamComponent.h"


//
// ARobotPlayerController::ARobotPlayerController
//
ARobotPlayerController::ARobotPlayerController( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	TeamComponent				= ObjectInitializer.CreateDefaultSubobject<UTeamComponent>( this, TEXT( "TeamComponent" ) );
	TeamComponent->SetTeamName( ETeamName::Friendly );

	bAutoManageActiveCameraTarget	= false;
	bIsIn							= false;

	LastXInputValue					= 0.0f;
	LastYInputValue					= 0.0f;

	Score							= 0;
	Deaths							= 0;
}


//
// ARobotPlayerController::SetupInputComponent
//
void ARobotPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction( "DropIn", EInputEvent::IE_Pressed, this, &ARobotPlayerController::DropIn );
	InputComponent->BindAction( "DropOut", EInputEvent::IE_Pressed, this, &ARobotPlayerController::DropOut );

	InputComponent->BindAction( "Fire", EInputEvent::IE_Pressed, this, &ARobotPlayerController::StartFireCharacterWeapon );
	InputComponent->BindAction( "Fire", EInputEvent::IE_Released, this, &ARobotPlayerController::EndFireCharacterWeapon );

	FInputActionBinding& PauseToggle = InputComponent->BindAction( "Pause", EInputEvent::IE_Pressed, this, &ARobotPlayerController::ToggleInGamePause );
	PauseToggle.bExecuteWhenPaused = true;

	InputComponent->BindAxis( "MoveRight", this, &ARobotPlayerController::MoveRight );
	InputComponent->BindAxis( "MoveUp", this, &ARobotPlayerController::MoveUp );
	InputComponent->BindAxis( "FireRight", this, &ARobotPlayerController::FireRight );
	InputComponent->BindAxis( "FireUp", this, &ARobotPlayerController::FireUp );
}


//
// ARobotPlayerController::GetAudioListenerPosition
//
void ARobotPlayerController::GetAudioListenerPosition( FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir )
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto GameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( GameMode == nullptr )
	{
		return;
	}

	auto CameraActor = GameMode->GetCameraActor();
	if( CameraActor == nullptr )
	{
		return;
	}

	OutLocation = CameraActor->GetActorLocation();
	OutFrontDir = CameraActor->GetActorForwardVector();
	OutRightDir = CameraActor->GetActorRightVector();
}


//
// ARobotPlayerController::DropIn
//
void ARobotPlayerController::DropIn()
{
	// Already in game.
	if( bIsIn )
	{
		return;
	}

	StartRespawn();
	bIsIn = true;
}


//
// ARobotPlayerController::DropOut
//
void ARobotPlayerController::DropOut()
{
	// Already out.
	if( !bIsIn )
	{
		return;
	}

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto Robot = GetPawn();
	if( Robot == nullptr )
	{
		// Respawning. Cancel it.
		CancelRespawn();
	}
	else
	{
		UnPossess();
		World->DestroyActor( Robot );
	}
	bIsIn = false;
}


//
// ARobotPlayerController::IsRespawning
//
bool ARobotPlayerController::IsRespawning() const
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return false;
	}
	return World->GetTimerManager().IsTimerActive( RespawnTimerHandle );
}


//
// ARobotPlayerController::GetRemainingRespawnSeconds
//
float ARobotPlayerController::GetRemainingRespawnSeconds() const
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return 0.0f;
	}
	return World->GetTimerManager().GetTimerRemaining( RespawnTimerHandle );
}


//
// ARobotPlayerController::MoveRight
//
void ARobotPlayerController::MoveRight( float Value )
{
	auto Character = Cast<ACharacter>( GetPawn() );
	if( Character == nullptr )
	{
		return;
	}
	Character->AddMovementInput( FVector::RightVector, Value );
}


//
// ARobotPlayerController::MoveUp
//
void ARobotPlayerController::MoveUp( float Value )
{
	auto Character = Cast<ACharacter>( GetPawn() );
	if( Character == nullptr )
	{
		return;
	}
	Character->AddMovementInput( FVector::ForwardVector, Value );
}


//
// ARobotPlayerController::FireRight
//
void ARobotPlayerController::FireRight( float Value )
{
	LastXInputValue = Value;
	if( !ShouldRotateWithLastValues() )
	{
		return;
	}

	auto Character = Cast<ACharacter>( GetPawn() );
	if( Character == nullptr )
	{
		return;
	}
	
	FVector	NewForward	= GetControlRotation().Vector();
	NewForward.Y		= Value;
	NewForward.Normalize();
	SetControlRotation( NewForward.Rotation() );
}


//
// ARobotPlayerController::FireUp
//
void ARobotPlayerController::FireUp( float Value )
{
	LastYInputValue = Value;
	if( !ShouldRotateWithLastValues() )
	{
		return;
	}
		
	auto Character = Cast<ACharacter>( GetPawn() );
	if( Character == nullptr )
	{
		return;
	}
	
	FVector	NewForward	= GetControlRotation().Vector();
	NewForward.X		= -Value;
	NewForward.Normalize();
	SetControlRotation( NewForward.Rotation() );
}


//
// ARobotPlayerControlller::ToggleInGamePause
//
void ARobotPlayerController::ToggleInGamePause()
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto GameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( GameMode == nullptr )
	{
		return;
	}

	// Player one can pause only.
	if( GameMode->GetRobotPlayer( EPlayerID::One ) != this )
	{
		return;
	}

	if( !GameMode->IsInGamePaused() )
	{
		GameMode->EnableInGamePause();
	}
	else
	{
		GameMode->DisableInGamePause();
	}
}


//
// ARobotPlayerController::StartFireCharacterWeapon
//
void ARobotPlayerController::StartFireCharacterWeapon()
{
	auto RobotCharacter = Cast<ARobotHeroCharacter>( GetPawn() );
	if( RobotCharacter == nullptr )
	{
		return;
	}
	RobotCharacter->PressWeaponTrigger();
}


//
// ARobotPlayerController::EndFireCharacterWeapon
//
void ARobotPlayerController::EndFireCharacterWeapon()
{
	auto RobotCharacter = Cast<ARobotHeroCharacter>( GetPawn() );
	if( RobotCharacter == nullptr )
	{
		return;
	}
	RobotCharacter->ReleaseWeaponTrigger();
}


//
// ARobotPlayerController::StartRespawn
//
void ARobotPlayerController::StartRespawn()
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto GameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( GameMode == nullptr )
	{
		return;
	}

	World->GetTimerManager().SetTimer( RespawnTimerHandle, this, &ARobotPlayerController::SpawnCharacter, GameMode->GetRespawnSecondsForPlayer( this ), false );
}


//
// ARobotPlayerController::CancelRespawn
//
void ARobotPlayerController::CancelRespawn()
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	World->GetTimerManager().ClearTimer( RespawnTimerHandle );
}


//
// ARobotPlayerController::SpawnCharacter
//
void ARobotPlayerController::SpawnCharacter()
{
	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto GameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( GameMode == nullptr || GameMode->RobotHeroCharacterClass == nullptr )
	{
		return;
	}

	// Spawn the player's pawn at the best player start.
	auto Robot			= World->SpawnActor<APawn>( GameMode->RobotHeroCharacterClass );
	auto PlayerStart	= GameMode->FindPlayerStart( this );
	if( PlayerStart != nullptr )
	{
		Robot->SetActorTransform( PlayerStart->GetTransform() );
	}

	Possess( Robot );

	// Bind our on death callback if our pawn has a health component.
	auto HealthComponent = Robot->FindComponentByClass<UHealthComponent>();
	if( HealthComponent != nullptr )
	{
		HealthComponent->OnDeath.AddDynamic( this, &ARobotPlayerController::RobotCharacterDeathCallback );
	}

	OnCharacterSpawned.Broadcast();
}


//
// ARobotPlayerController::ShouldRotateWithLastValues
//
bool ARobotPlayerController::ShouldRotateWithLastValues() const
{
	return ( LastXInputValue > RotateInputAxisTolerance || LastXInputValue < -RotateInputAxisTolerance ) 
		|| ( LastYInputValue > RotateInputAxisTolerance || LastYInputValue < -RotateInputAxisTolerance );
}


//
// ARobotPlayerController::RobotCharacterDeathCallback
//
void ARobotPlayerController::RobotCharacterDeathCallback( float DamageAmount, AController* DamageInstigator, AActor* DamageCauser )
{
	OnCharacterDied.Broadcast();

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	auto GameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( GameMode == nullptr || GameMode->RobotHeroCharacterClass == nullptr )
	{
		return;
	}

	GameMode->OnRobotCharacterDeath( this );

	auto Robot = GetPawn();
	if( Robot != nullptr )
	{
		UnPossess();
		World->DestroyActor( Robot );
	}

	++Deaths;
	StartRespawn();
}