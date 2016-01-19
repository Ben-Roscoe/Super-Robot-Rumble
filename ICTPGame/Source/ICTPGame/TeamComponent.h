// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TeamComponent.generated.h"

UENUM( BlueprintType )
enum class ETeamName : uint8
{
	Friendly	= 0,
	Enemy		= 1,
	Any			= 2,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ICTPGAME_API UTeamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	


	static bool AreEnemies( const UTeamComponent* t1, const UTeamComponent* t2 );

	UTeamComponent();

	UFUNCTION( BlueprintPure, Category = "Team" )
	ETeamName GetTeamName() const { return TeamName; }

	void SetTeamName( ETeamName NewTeamName ) { TeamName = NewTeamName; }

private:


	UPROPERTY( EditDefaultsOnly, Category = "Team" )
	ETeamName		TeamName;

		
	
};
