// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "MultipleWeaponUserComponent.h"


//
// UMultipleWeaponUserComponent::UMultipleWeaponUserComponent
//
UMultipleWeaponUserComponent::UMultipleWeaponUserComponent( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	bWantsBeginPlay = true;
}


//
// UMultipleWeaponUserComponent::BeginPlay
//
void UMultipleWeaponUserComponent::BeginPlay()
{
	Super::BeginPlay();
	CreateWeapon();
}


//
// UMultipleWeaponUserComponent::EndPlay
//
void UMultipleWeaponUserComponent::EndPlay( const EEndPlayReason::Type EndPlayReason )
{
	Super::EndPlay( EndPlayReason );
	DestroyWeapon();
}


//
// UMultipleWeaponUserComponent::ReplaceWeapon
//
void UMultipleWeaponUserComponent::ReplaceWeapon( TSubclassOf<AWeaponActor> NewWeaponClass )
{
	CurrentWeaponClass = NewWeaponClass;
	DestroyWeapon();
	CreateWeapon();
}


//
// UMultipleWeaponUserComponent::CreateWeapon
//
void UMultipleWeaponUserComponent::CreateWeapon()
{
	if( CurrentWeaponClass == nullptr )
	{
		return;
	}

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	CurrentWeapon = World->SpawnActor<AWeaponActor>( CurrentWeaponClass );
	CurrentWeapon->Instigator = GetOwner()->Instigator;
	CurrentWeapon->SetWeaponFireArrowComponent( FireOrigin );

	OnWeaponCreated.Broadcast( CurrentWeapon );
}


//
// UMultipleWeaponUserComponent::DestroyWeapon
//
void UMultipleWeaponUserComponent::DestroyWeapon()
{
	if( CurrentWeapon == nullptr )
	{
		return;
	}

	auto World = GetWorld();
	if( World == nullptr )
	{
		return;
	}

	OnWeaponDestroyed.Broadcast( CurrentWeapon );
	World->DestroyActor( CurrentWeapon );
}



