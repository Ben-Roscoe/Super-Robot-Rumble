// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "EnemySpawnerBase.generated.h"

class AEnemySpawnerBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FEnemySpawnerClearedEvent, AEnemySpawnerBase*, EnemySpawner );

UCLASS( abstract )
class ICTPGAME_API AEnemySpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FEnemySpawnerClearedEvent OnCleared;

	UPROPERTY( EditAnywhere, Category = "Enemy Spawner" )
	TSubclassOf<AActor> EnemyClass;

	UPROPERTY( EditAnywhere, Category = "Enemy Spawner" )
	int32 Groups;

	UPROPERTY( EditAnywhere, Category = "Enemy Spawner" )
	int32 EnemiesPerGroup;

	UPROPERTY( EditAnywhere, Category = "Enemy Spawner" )
	float SecondsBetweenGroups;

	UPROPERTY( EditAnywhere, Category = "Enemy Spawner" )
	bool  bShouldWaitForGroupDeath;

	AEnemySpawnerBase( const FObjectInitializer& ObjectInitializer );

	UFUNCTION( BlueprintNativeEvent, BlueprintCallable, Category = "Enemy Spawner" )
	void SpawnEnemies();
	virtual void SpawnEnemies_Implementation();

	UFUNCTION( BlueprintPure, Category = "Enemy Spawner" )
	bool IsCleared() const { return bIsCleared; }


protected:


	UPROPERTY( VisibleAnywhere, Category = "Enemy Spawner" )
	TArray<AActor*> SpawnedEnemies;

	int32 DestroyedEnemiesCount = 0;

	AActor* SpawnEnemy( const FVector& Location, const FRotator& Rotation );
	void ClearSpawnedEnemies();
	
	UFUNCTION( BlueprintNativeEvent, Category = "Enemy Spawner" )
	void AllEnemiesDestroyed();
	virtual void AllEnemiesDestroyed_Implementation();

	UFUNCTION( BlueprintNativeEvent, Category = "Enemy Spawner" )
	void SpawnGroup();
	virtual void SpawnGroup_Implementation(){}

	UFUNCTION( BlueprintCallable, Category = "Enemy Spawner" )
	void SetCleared();


private:


	int32				SpawnedGroupsCount	= 0;
	bool				bHasTriggered		= false;
	FTimerHandle		GroupSpawnTimer;

	UPROPERTY( VisibleAnywhere, Category = "Enemy Spawner" )
	bool bIsCleared = false;

	UFUNCTION()
	void OnSpawnedEnemyDestroyed();

	UFUNCTION()
	void SpawnNextGroup();

};
