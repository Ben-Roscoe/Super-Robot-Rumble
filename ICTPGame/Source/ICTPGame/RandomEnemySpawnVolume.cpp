// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "RandomEnemySpawnVolume.h"



//
// ARandomEnemySpawnVolume::ARandomEnemySpawnVolume
//
ARandomEnemySpawnVolume::ARandomEnemySpawnVolume( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	SpawnAreaBoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this, TEXT( "SpawnAreaBoxComponent" ) );
	RootComponent		  = SpawnAreaBoxComponent;
}


//
// ARandomEnemySpawnVolume::SpawnGroup_Implementation
//
void ARandomEnemySpawnVolume::SpawnGroup_Implementation()
{
	// Spawn the group in random locations within the spawn volume.
	for( int i = 0; i < EnemiesPerGroup; ++i )
	{
		FVector HalfExtents{ SpawnAreaBoxComponent->GetScaledBoxExtent() / 2.0f };
		FBox	Box{ GetActorLocation() - HalfExtents, GetActorLocation() + HalfExtents };
		FVector Location = FMath::RandPointInBox( Box );
		
		SpawnEnemy( Location, FRotator::ZeroRotator );
	}
}