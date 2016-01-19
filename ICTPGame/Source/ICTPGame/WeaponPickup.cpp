// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "WeaponPickup.h"
#include "MultipleWeaponUserComponent.h"


//
// AWeaponPickup::AWeaponPickup
//
AWeaponPickup::AWeaponPickup( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this, TEXT( "BoxComponent" ) );
	RootComponent = BoxComponent;

	Mesh		 = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>( this, TEXT( "Mesh" ) );
	Mesh->AttachTo( RootComponent );
}


//
// AWeaponPickup::BeginPlay
//
void AWeaponPickup::BeginPlay()
{
	BoxComponent->OnComponentBeginOverlap.AddDynamic( this, &AWeaponPickup::BeginBoxComponentOverlap );
}


//
// AWeaponPickup::Pickup_Implementation
//
void AWeaponPickup::Pickup_Implementation( AActor* PickupActor )
{
	OnPickup.Broadcast( PickupActor );

	// Destroy self on pickup.
	auto World = GetWorld();
	check( World );
	World->DestroyActor( this );
}


//
// AWeaponPickup::BeginBoxComponentOverlap
//
void AWeaponPickup::BeginBoxComponentOverlap( AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	if( OtherActor == nullptr )
	{
		return;
	}

	auto WeaponUserComponent = OtherActor->FindComponentByClass<UMultipleWeaponUserComponent>();
	if( WeaponUserComponent == nullptr )
	{
		return;
	}

	// Don't pickup if they already have it.
	if( WeaponUserComponent->GetCurrentWeaponClass() == WeaponActorClass )
	{
		return;
	}

	WeaponUserComponent->ReplaceWeapon( WeaponActorClass );
	Pickup( OtherActor );
}
