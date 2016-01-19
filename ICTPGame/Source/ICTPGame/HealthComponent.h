// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FHealthComponentDeathEvent, float, DamageAmount, AController*, Instigator, AActor*, DamageCauser );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ICTPGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	


	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FHealthComponentDeathEvent OnDeath;


	UHealthComponent();

	virtual void BeginPlay() override;


	UFUNCTION()
	void OnOwnerDamaged( float DamageAmount, const UDamageType* DamageType, AController* Instigater, AActor* DamageCauser );

	UFUNCTION( BlueprintPure, Category = "Health" )
	float GetMaxHealth() const;

	UFUNCTION( BlueprintPure, Category = "Health" )
	float GetHealth() const;

	UFUNCTION( BlueprintPure, Category = "Health" )
	bool IsDead() const;

private:


	UPROPERTY( EditDefaultsOnly, Category = "Health" )
	float	Health;

	UPROPERTY( EditDefaultsOnly, Category = "Health" )
	float	MaxHealth;

	UPROPERTY( EditAnywhere, Category = "Health" )
	bool	ShouldDestroyOwnerOnDeath;

	void OnDeathInternal( float DamageAmount, AController* EventInstigator, AActor* DamageCauser );
};
