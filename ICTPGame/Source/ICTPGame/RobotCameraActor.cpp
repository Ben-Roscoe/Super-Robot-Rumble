// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "RobotCameraActor.h"
#include "RobotPlayerController.h"



//
// ARobotCameraActor::ARobotCameraActor
//
ARobotCameraActor::ARobotCameraActor( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PrimaryActorTick.bCanEverTick = true;


	MinArmLength		= 500.0f;
	MaxArmLength		= 1000.0f;
	BoundsScale			= 1.25f;
	ExtrapolateSpeed	= 10.0f;

	LookAtSceneComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>( this, TEXT( "LookAtSceneComponent" ) );
	SetRootComponent( LookAtSceneComponent );

	SpringArmComponent	 = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>( this, TEXT( "SpringArmComponent" ) );
	SpringArmComponent->AttachTo( LookAtSceneComponent );

	CameraComponent		 = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>( this, TEXT( "CameraComponent" ) );
	CameraComponent->AttachTo( SpringArmComponent );
}


//
// ARobotCameraActor::Tick
//
void ARobotCameraActor::Tick( float DeltaSeconds )
{
	auto	World = GetWorld();

	if( World != nullptr )
	{
		// Get each player's pawn and add to the cumulative position. We'll look at the average of
		// this.
		uint8	PositionCount		= 0;

		FVector	MinLocation			= FVector::ZeroVector;
		FVector MaxLocation			= FVector::ZeroVector;
		for( auto Iterator = World->GetPlayerControllerIterator(); Iterator; ++Iterator )
		{
			auto RobotPlayer = Cast<ARobotPlayerController>( *Iterator );
			if( RobotPlayer == nullptr )
			{
				continue;
			}
			if( !RobotPlayer->GetIsIn() )
			{
				continue;
			}
			
			auto Pawn = RobotPlayer->GetPawn();
			if( Pawn == nullptr )
			{
				continue;
			}

			auto PawnLocation = Pawn->GetActorLocation();
			++PositionCount;

			if( PositionCount == 1 )
			{
				MinLocation = PawnLocation;
				MaxLocation	= PawnLocation;
				continue;
			}

			// Find bounds.
			if( PawnLocation.X < MinLocation.X )
			{
				MinLocation.X = PawnLocation.X;
			}
			if( PawnLocation.X > MaxLocation.X )
			{
				MaxLocation.X = PawnLocation.X;
			}
			if( PawnLocation.Y < MinLocation.Y )
			{
				MinLocation.Y = PawnLocation.Y;
			}
			if( PawnLocation.Y > MaxLocation.Y )
			{
				MaxLocation.Y = PawnLocation.Y;
			}

			// Make sure we look at the top of the box so we can see the entire thing.
			if( PawnLocation.Z > MaxLocation.Z )
			{
				MaxLocation.Z = PawnLocation.Z;
			}
		}

		// Create a box outlining the bounds of each player character. Scale the bounds so characters aren't on the very edge of the screen.
		FVector	Size						= FVector( FMath::Abs( MaxLocation.X - MinLocation.X ), FMath::Abs( MaxLocation.Y - MinLocation.Y ), 0.0f );
		FVector Center						= FVector( MinLocation.X + ( Size.X / 2.0f ), MinLocation.Y + ( Size.Y / 2.0f ), 0.0f );

		FVector	ScaledMax					= MaxLocation * BoundsScale;
		FVector	ScaledMin					= MinLocation * BoundsScale;
		FVector ScaledSize					= Size * BoundsScale;

		// How far do we need to zoom to make sure everyone fits?
		float	MaxHorizontal				= ScaledSize.Y / ( 2 * FMath::Tan( FMath::DegreesToRadians( CameraComponent->FieldOfView / 2 ) ) );
		float	MaxVertical					= ScaledSize.X / ( 2 * FMath::Tan( FMath::DegreesToRadians( ( CameraComponent->FieldOfView / CameraComponent->AspectRatio ) / 2 ) ) );
		float	WantedArmLength				= FMath::Max<float>( MaxHorizontal, MaxVertical );
		NextArmLength						= FMath::Clamp<float>( WantedArmLength, MinArmLength, MaxArmLength );

		// Look at center of the box.
		if( PositionCount != 0 )
		{
			FVector LastLocation	= GetActorLocation();
			NextPosition			= FVector( MaxVertical <= MaxArmLength ? Center.X : LastLocation.X, MaxHorizontal <= MaxArmLength ? Center.Y : LastLocation.Y, MaxLocation.Z );
		}
	}

	// Extrapolate to next length/location.
	SetActorLocation( FMath::Lerp( GetActorLocation(), NextPosition, FMath::Min( DeltaSeconds * ExtrapolateSpeed, 1.0f ) ) );
	SpringArmComponent->TargetArmLength = FMath::Lerp( SpringArmComponent->TargetArmLength, NextArmLength, FMath::Min( DeltaSeconds * ExtrapolateSpeed, 1.0f ) );
}
