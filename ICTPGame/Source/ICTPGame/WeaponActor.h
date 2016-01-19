// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Components/SceneComponent.h"
#include "WeaponComponent.h"
#include "WeaponActor.generated.h"

UCLASS()
class ICTPGAME_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	

	UPROPERTY( BlueprintReadWrite, VisibleAnywhere, Category = "Weapon Actor" )
	UWeaponComponent*		WeaponComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon Actor" )
	USceneComponent*		WeaponRoot;

	AWeaponActor( const FObjectInitializer& ObjectInitializer );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Weapon Actor" )
	void FireWeapon();
	virtual void FireWeapon_Implementation(){}

	UFUNCTION( BlueprintPure, Category = "Weapon Actor" )
	FVector WorldLocationToFireOriginLocation( FVector WorldLocation ) const;

	UFUNCTION( BlueprintPure, Category = "Weapon Actor" )
	FRotator WorldRotationToFireOriginRotation( FRotator WorldRotation ) const;

	UFUNCTION( BlueprintPure, Category = "Weapon Actor" )
	UArrowComponent* GetWeaponFireArrowComponent() const { return WeaponFireArrowComponent; }

	UFUNCTION( BlueprintCallable, Category = "Weapon Actor" )
	void SetWeaponFireArrowComponent( UArrowComponent* NewWeaponFireArrowComponent ) { WeaponFireArrowComponent = NewWeaponFireArrowComponent; }


private:


	// The origin from where the weapon should fire.
	UArrowComponent*		WeaponFireArrowComponent;

	

};
