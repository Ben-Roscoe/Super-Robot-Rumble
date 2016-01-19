// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponActor.h"
#include "Components/ArrowComponent.h"
#include "MultipleWeaponUserComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMultipleWeaponUserComponentCreateWeaponEvent, AWeaponActor*, Weapon );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FMultipleWeaponUserComponentDestroyWeaponEvent, AWeaponActor*, Weapon );

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ICTPGAME_API UMultipleWeaponUserComponent : public UActorComponent
{
	GENERATED_BODY()

public:


	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FMultipleWeaponUserComponentCreateWeaponEvent OnWeaponCreated;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FMultipleWeaponUserComponentDestroyWeaponEvent OnWeaponDestroyed;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Multiple Weapon User" )
	UArrowComponent* FireOrigin;

	UMultipleWeaponUserComponent( const FObjectInitializer& ObjectInitializer );
	virtual void BeginPlay() override;
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason ) override;

	UFUNCTION( BlueprintCallable, Category = "Multiple Weapon User" )
	void ReplaceWeapon( TSubclassOf<AWeaponActor> NewWeaponClass );

	UFUNCTION( BlueprintPure, Category = "Multiple Weapon User" )
	bool HasWeapon() const { return CurrentWeapon != nullptr; }

	UFUNCTION( BlueprintPure, Category = "Multiple Weapon User" )
	AWeaponActor* GetCurrentWeaponActor() const { return CurrentWeapon; }

	UFUNCTION( BlueprintPure, Category = "Multiple Weapon User" )
	TSubclassOf<AWeaponActor> GetCurrentWeaponClass() const { return CurrentWeaponClass; }

	UFUNCTION( BlueprintPure, Category = "Multiple Weapon User" )
	bool IsFiringWeapon() const { return HasWeapon() && CurrentWeapon->WeaponComponent->IsTriggerPressed(); }

private:


	UPROPERTY( EditDefaultsOnly, Category = "Multiple Weapon User" )
	TSubclassOf<AWeaponActor>		CurrentWeaponClass;

	UPROPERTY( VisibleAnywhere, Category = "Multiple Weapon User" )
	AWeaponActor*					CurrentWeapon;

	void CreateWeapon();
	void DestroyWeapon();
};
