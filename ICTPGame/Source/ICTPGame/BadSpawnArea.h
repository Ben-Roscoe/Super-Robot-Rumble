// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UBoxComponent;

#include "GameFramework/Actor.h"
#include "BadSpawnArea.generated.h"

UCLASS()
class ICTPGAME_API ABadSpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	


	ABadSpawnArea( const FObjectInitializer& ObjectInitializer );

	
private:


	UPROPERTY( EditDefaultsOnly, Category = "Bad Spawn Area" )
	UBoxComponent*			BoxComponent;
	
};
