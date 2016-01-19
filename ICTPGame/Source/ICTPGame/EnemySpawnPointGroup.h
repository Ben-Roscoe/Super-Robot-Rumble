// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemySpawnerBase.h"
#include "EnemySpawnPointGroup.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API AEnemySpawnPointGroup : public AEnemySpawnerBase
{
	GENERATED_BODY()
	

public:
	

	UPROPERTY( EditAnywhere, Category = "Enemy Spawn Point Group", Meta = ( MakeEditWidget = true ) )
	TArray<FVector> SpawnPoints;


protected:


	virtual void SpawnGroup_Implementation() override;
	
};
