// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "RobotHeroCharacter.h"
#include "HealthComponent.h"
#include "MultipleWeaponUserComponent.h"
#include "BadSpawnArea.h"


//
// ARobotHeroCharacter::ARobotHeroCharacter
//
ARobotHeroCharacter::ARobotHeroCharacter( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	HealthComponent				= ObjectInitializer.CreateDefaultSubobject<UHealthComponent>( this, TEXT( "HealthComponent" ) );
	MultipleWeaponUserComponent = ObjectInitializer.CreateDefaultSubobject<UMultipleWeaponUserComponent>( this, TEXT( "MultipleWeaponUserComponent" ) );
	WeaponFireArrowComponent	= ObjectInitializer.CreateDefaultSubobject<UArrowComponent>( this, TEXT( "WeaponFireArrowComponent" ) );
	WeaponFireArrowComponent->AttachTo( RootComponent );

	MultipleWeaponUserComponent->FireOrigin = WeaponFireArrowComponent;

	// We're responsible for any damage we do.
	Instigator					= this;
}


//
// ARobotHeroCharacter::PressWeaponTrigger
//
void ARobotHeroCharacter::PressWeaponTrigger()
{
	if( !MultipleWeaponUserComponent->HasWeapon() )
	{
		return;
	}
	MultipleWeaponUserComponent->GetCurrentWeaponActor()->WeaponComponent->PressTrigger();
}


//
// ARobotHeroCharacter::ReleaseWeaponTrigger
//
void ARobotHeroCharacter::ReleaseWeaponTrigger()
{
	if( !MultipleWeaponUserComponent->HasWeapon() )
	{
		return;
	}
	MultipleWeaponUserComponent->GetCurrentWeaponActor()->WeaponComponent->ReleaseTrigger();
}


//
// ARobotHeroCharacter::IsInBadSpawn
//
bool ARobotHeroCharacter::IsInBadSpawn() const
{
	TArray<AActor*> OverlappingActors;
	GetCapsuleComponent()->GetOverlappingActors( OverlappingActors, ABadSpawnArea::StaticClass() );
	return OverlappingActors.Num() > 0;
}