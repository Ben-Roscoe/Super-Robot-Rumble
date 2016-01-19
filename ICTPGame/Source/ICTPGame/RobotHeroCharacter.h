// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UHealthComponent;
class UMultipleWeaponUserComponent;

#include "GameFramework/Character.h"
#include "Components/ActorComponent.h"
#include "RobotHeroCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API ARobotHeroCharacter : public ACharacter
{
	GENERATED_BODY()
	
	
public:


	UPROPERTY( BlueprintReadWrite, VisibleAnywhere, Category = "Health" )
	UHealthComponent* HealthComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Weapons" )
	UMultipleWeaponUserComponent* MultipleWeaponUserComponent;

	UPROPERTY( EditDefaultsOnly )
	UArrowComponent*				WeaponFireArrowComponent;

	ARobotHeroCharacter( const FObjectInitializer& ObjectInitializer );

	void PressWeaponTrigger();
	void ReleaseWeaponTrigger();


	UFUNCTION( BlueprintPure, Category = "Spawn" )
	bool IsInBadSpawn() const;


private:

};
