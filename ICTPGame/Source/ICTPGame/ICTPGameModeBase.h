// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "ICTPGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API AICTPGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
	

public:


	UFUNCTION( BlueprintCallable, Category = "Sound" )
	void ChangeSoundClassVolume( FString SoundClassName, float Value );

	UFUNCTION( BlueprintPure, Category = "Sound" )
	float GetSoundClassVolume( FString SoundClassName );
	
	
};
