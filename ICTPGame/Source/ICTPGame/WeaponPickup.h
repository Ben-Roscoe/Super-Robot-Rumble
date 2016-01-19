// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AWeaponActor;

#include "GameFramework/Actor.h"
#include "WeaponPickup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FWeaponPickupPickedUpEvent, AActor*, PickupActor );

UCLASS()
class ICTPGAME_API AWeaponPickup : public AActor
{
	GENERATED_BODY()
	
public:	


	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FWeaponPickupPickedUpEvent OnPickup;

	AWeaponPickup( const FObjectInitializer& ObjectInitializer );
	virtual void BeginPlay();


protected:


	UFUNCTION( BlueprintNativeEvent, Category = "Weapon Pickup" )
	void Pickup( AActor* PickupActor );
	virtual void Pickup_Implementation( AActor* PickupActor );
	

private:


	UPROPERTY( EditAnywhere, Category = "Weapon Pickup" )
	TSubclassOf<AWeaponActor> WeaponActorClass;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon Pickup" )
	UBoxComponent* BoxComponent;

	UPROPERTY( EditDefaultsOnly, Category = "Weapon Pickup" )
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void BeginBoxComponentOverlap( AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );
};
