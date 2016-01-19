// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnemySpawnerBase.h"
#include "RandomEnemySpawnVolume.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API ARandomEnemySpawnVolume : public AEnemySpawnerBase
{
	GENERATED_BODY()
	
	
public:


	ARandomEnemySpawnVolume( const FObjectInitializer& ObjectInitializer );
	

protected:
	
	virtual void SpawnGroup_Implementation() override;


private:


	UPROPERTY( EditDefaultsOnly, Category = "Random Enemy Spawn Volume" )
	UBoxComponent*		SpawnAreaBoxComponent;
};
