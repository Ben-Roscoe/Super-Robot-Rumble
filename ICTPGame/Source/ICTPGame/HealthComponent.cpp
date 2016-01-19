// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "HealthComponent.h"
#include "ICTPGameGameMode.h"


//
// UHealthComponent::UHealthComponent
//
UHealthComponent::UHealthComponent()
{
	bWantsBeginPlay = true;

	MaxHealth						= 100.0f;
	Health							= MaxHealth;
	ShouldDestroyOwnerOnDeath		= false;
}


//
// UHealthComponent::BeginPlay
//
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	auto Owner = GetOwner();
	if( Owner == nullptr )
	{
		return;
	}

	Owner->OnTakeAnyDamage.AddDynamic( this, &UHealthComponent::OnOwnerDamaged );
}


//
// UHealthComponent::OnOwnerDamaged
//
void UHealthComponent::OnOwnerDamaged( float DamageAmount, const UDamageType* DamageType, AController* Instigater, AActor* DamageCauser )
{
	// Don't die twice.
	if( IsDead() )
	{
		return;
	}

	Health -= DamageAmount;
	if( IsDead() )
	{
		OnDeathInternal( DamageAmount, Instigater, DamageCauser );
	}
}


//
// UHealthComponent::OnDeathInternal
//
void UHealthComponent::OnDeathInternal( float DamageAmount, AController* EventInstigator, AActor* DamageCauser )
{
	OnDeath.Broadcast( DamageAmount, EventInstigator, DamageCauser );

	auto HealthOwner = GetOwner();
	auto World		 = GetWorld();

	if( World == nullptr || HealthOwner == nullptr )
	{
		return;
	}

	auto ICTPGameMode = Cast<AICTPGameGameMode>( World->GetAuthGameMode() );
	if( ICTPGameMode != nullptr )
	{
		ICTPGameMode->RegisterActorDeath( DamageAmount, HealthOwner, DamageCauser );
	}

	if( IsRegistered() && ShouldDestroyOwnerOnDeath )
	{
		World->DestroyActor( GetOwner() );
	}
}


//
// UHealthComponent::GetHealth
//
float UHealthComponent::GetHealth() const
{
	return Health;
}


//
// UHealthComponent::GetMaxHealth
//
float UHealthComponent::GetMaxHealth() const
{
	return MaxHealth;
}


//
// UHealthComponent::IsDead
//
bool UHealthComponent::IsDead() const
{
	return Health <= 0.0f;
}