// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ICTPGAME_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	

	UScoreComponent();

	UFUNCTION( BlueprintPure, Category = "Score" )
	int32 GetScore() const { return Score; }

	UFUNCTION( BlueprintCallable, Category = "Score" )
	void SetScore( int32 NewScore ) { Score = NewScore; }
	
private:


	UPROPERTY( EditAnywhere, Category = "Score" )
	int32 Score;


};
