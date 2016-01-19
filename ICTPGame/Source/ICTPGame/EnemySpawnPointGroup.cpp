// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "EnemySpawnPointGroup.h"


//
// AEnemySpawnPointGroup::SpawnGroup_Implementation
//
void AEnemySpawnPointGroup::SpawnGroup_Implementation()
{
	// Spawn an enemy at each of our spawn points.
	for( int i = 0; i < SpawnPoints.Num(); ++i )
	{
		SpawnEnemy( SpawnPoints[i] + GetActorLocation(), FRotator::ZeroRotator );
	}
}