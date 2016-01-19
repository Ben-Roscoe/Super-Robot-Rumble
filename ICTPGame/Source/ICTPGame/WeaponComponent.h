// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE( FWeaponComponentTriggerPressedEvent );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FWeaponComponentTriggerReleasedEvent );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FWeaponComponentFireEvent );


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ICTPGAME_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	


	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FWeaponComponentTriggerPressedEvent OnTriggerPressed;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FWeaponComponentTriggerReleasedEvent OnTriggerReleased;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FWeaponComponentFireEvent OnFire;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Weapon" )
	float SecondsBetweenRounds;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Weapon" )
	float SecondsBetweenBursts;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Weapon" )
	int32 RoundsInBurst;

	UWeaponComponent();


	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void PressTrigger();

	UFUNCTION( BlueprintCallable, Category = "Weapon" )
	void ReleaseTrigger();

	UFUNCTION( BlueprintPure, Category = "Weapon" )
	bool IsTriggerPressed() const { return bIsTriggerPressed; }

	UFUNCTION( BlueprintPure, Category = "Weapon" )
	int32 GetRoundsFiredThisBurst() const { return RoundsFiredThisBurst; }

		
private:


	bool				bIsTriggerPressed;
	bool				bIsFirstBurst;
	bool				bIsFiring;	
	int32				RoundsFiredThisBurst;
	FTimerHandle		FireTimerHandle;
	FTimerHandle		BurstTimerHandle;


	void Fire();
	void StartBurst();
	void EndFire();
};
