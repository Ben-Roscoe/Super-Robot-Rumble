// Fill out your copyright notice in the Description page of Project Settings.

#include "ICTPGame.h"
#include "BadSpawnArea.h"
#include "Components/BoxComponent.h"


//
// ABadSpawnArea::ABadSpawnArea
//
ABadSpawnArea::ABadSpawnArea( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	BoxComponent = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>( this, TEXT( "BoxComponent" ) );
	RootComponent = BoxComponent;
}


