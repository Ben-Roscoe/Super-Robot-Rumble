// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "EnemySpawnerBase.h"


//
// AEnemySpawnerBase::AEnemySpawnerBase
//
AEnemySpawnerBase::AEnemySpawnerBase( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	bIsCleared			  = false;
	bHasTriggered		  = false;
}


//
// AEnemySpawnerBase::SpawnEnemies_Implementation
//
void AEnemySpawnerBase::SpawnEnemies_Implementation()
{
	if( IsCleared() )
	{
		return;
	}

	if( bHasTriggered )
	{
		return;
	}
	bHasTriggered = true;

	if( !bShouldWaitForGroupDeath )
	{
		// We're spawning off a timer.
		auto World = GetWorld();
		check( World );
		World->GetTimerManager().SetTimer( GroupSpawnTimer, this, &AEnemySpawnerBase::SpawnNextGroup, SecondsBetweenGroups, true, 0.0f );
	}
	else
	{
		// We're spawning off group deaths.
		SpawnNextGroup();
	}
}


//
// AEnemySpawnerBase::SpawnEnemy
//
AActor* AEnemySpawnerBase::SpawnEnemy( const FVector& Location, const FRotator& Rotation )
{
	auto World	  = GetWorld();
	check( World );

	if( EnemyClass == nullptr )
	{
		return nullptr;
	}

	// Make sure spawning doesn't fail due to collision.
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	auto NewEnemy = World->SpawnActor<AActor>( EnemyClass, Location, GetActorRotation(), SpawnParams );
	check( NewEnemy );

	NewEnemy->OnDestroyed.AddDynamic( this, &AEnemySpawnerBase::OnSpawnedEnemyDestroyed );
	SpawnedEnemies.Add( NewEnemy );

	return NewEnemy;
}


//
// AEnemySpawnerBase::ClearSpawnedEnemies
//
void AEnemySpawnerBase::ClearSpawnedEnemies()
{
	for( int i = 0; i < SpawnedEnemies.Num(); ++i )
	{
		if( SpawnedEnemies[i] == nullptr || !SpawnedEnemies[i]->IsValidLowLevel() )
		{
			continue;
		}

		// Remove our destroy callbacks.
		SpawnedEnemies[i]->OnDestroyed.RemoveDynamic( this, &AEnemySpawnerBase::OnSpawnedEnemyDestroyed );
	}

	DestroyedEnemiesCount = 0;
	SpawnedEnemies.Empty();
}


//
// AEnemySpawnerBase::AllEnemiesDestroyed_Implementation
//
void AEnemySpawnerBase::AllEnemiesDestroyed_Implementation()
{
	if( !IsCleared() )
	{
		ClearSpawnedEnemies();
		if( SpawnedGroupsCount >= Groups )
		{
			SetCleared();
		}
		else if( bShouldWaitForGroupDeath )
		{
			SpawnNextGroup();
		}
	}
}


//
// AEnemySpawnerBase::SetCleared
//
void AEnemySpawnerBase::SetCleared()
{
	bIsCleared = true;
	OnCleared.Broadcast( this );
}


//
// AEnemySpawnerBase::OnSpawnedEnemyDestroyed
//
void AEnemySpawnerBase::OnSpawnedEnemyDestroyed()
{
	++DestroyedEnemiesCount;
	if( DestroyedEnemiesCount >= SpawnedEnemies.Num() )
	{
		AllEnemiesDestroyed();
	}
}


//
// AEnemySpawnerBase::SpawnNextGroup
//
void AEnemySpawnerBase::SpawnNextGroup()
{
	auto World = GetWorld();
	check( World );

	++SpawnedGroupsCount;
	if( SpawnedGroupsCount >= Groups && !bShouldWaitForGroupDeath )
	{
		World->GetTimerManager().ClearTimer( GroupSpawnTimer );
	}

	SpawnGroup();
}