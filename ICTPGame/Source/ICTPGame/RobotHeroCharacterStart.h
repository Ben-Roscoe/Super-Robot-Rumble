// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerStart.h"
#include "ICTPGameGameMode.h"
#include "RobotHeroCharacterStart.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API ARobotHeroCharacterStart : public APlayerStart
{
	GENERATED_BODY()
	
	
public:


	UPROPERTY( BlueprintReadOnly, EditAnywhere, Category = "Robot Hero" )
	EPlayerID FollowPlayerID;

	UPROPERTY( BlueprintReadOnly, EditDefaultsOnly, Category = "Robot Hero" )
	float	  SecondsBetweenTeleport;

	UPROPERTY( BlueprintReadOnly, EditDefaultsOnly, Category = "Robot Hero" )
	float	  TeleportHeight;

	ARobotHeroCharacterStart( const FObjectInitializer& ObjectInitializer );
	virtual void BeginPlay() override;
	

private:


	FTimerHandle TeleportTimerHandle;

	void TeleportToRobotHero();
	bool TryTeleportToRobotControllerPawn( ARobotPlayerController* RobotPlayerController );
};
