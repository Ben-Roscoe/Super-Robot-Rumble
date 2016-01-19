// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "WeaponActor.h"


//
// AWeaponActor::AWeaponActor
//
AWeaponActor::AWeaponActor( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	WeaponRoot		= ObjectInitializer.CreateDefaultSubobject<USceneComponent>( this, TEXT( "WeaponRoot" ) );
	RootComponent	= WeaponRoot;

	WeaponComponent = ObjectInitializer.CreateDefaultSubobject<UWeaponComponent>( this, TEXT( "WeaponComponent" ) );
}


//
// AWeaponActor::WorldLocationToFireOriginLocation
//
FVector AWeaponActor::WorldLocationToFireOriginLocation( FVector WorldLocation ) const
{
	if( RootComponent == nullptr || WeaponFireArrowComponent == nullptr )
	{
		return FVector::ZeroVector;
	}

	//auto FireOriginRelativeLocation = WorldLocation - WeaponRoot->GetComponentLocation();
	return WeaponFireArrowComponent->GetComponentLocation() + WeaponFireArrowComponent->ComponentToWorld.GetRotation() * WorldLocation;//FireOriginRelativeLocation;
}


//
// AWeaponActor::WorldRotationToFireOriginRotation
//
FRotator AWeaponActor::WorldRotationToFireOriginRotation( FRotator WorldRotation ) const
{
	if( RootComponent == nullptr || WeaponFireArrowComponent == nullptr )
	{
		return WorldRotation;
	}

	//auto FireOriginRelativeRotation = WorldRotation - WeaponRoot->GetComponentRotation();
	return WeaponFireArrowComponent->GetComponentRotation() + WorldRotation;//FireOriginRelativeRotation;
}