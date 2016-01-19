// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UTeamComponent;

#include "GameFramework/PlayerController.h"
#include "RobotPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE( FRobotPlayerControllerCharacterDieEvent );
DECLARE_DYNAMIC_MULTICAST_DELEGATE( FRobotPlayerControllerCharacterSpawnedEvent );

/**
 * 
 */
UCLASS()
class ICTPGAME_API ARobotPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:


	UPROPERTY( BlueprintReadWrite, VisibleAnywhere, Category = "Team" )
	UTeamComponent*	  TeamComponent;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FRobotPlayerControllerCharacterDieEvent OnCharacterDied;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FRobotPlayerControllerCharacterSpawnedEvent OnCharacterSpawned;
	
	ARobotPlayerController( const FObjectInitializer& ObjectInitializer );
	virtual void SetupInputComponent() override;
	virtual void GetAudioListenerPosition( FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir ) override;

	UFUNCTION( BlueprintCallable, Category = "Robot Player" )
	void DropIn();

	UFUNCTION( BlueprintCallable, Category = "Robot Player" )
	void DropOut();

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	bool GetIsIn() const { return bIsIn; }

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	bool IsRespawning() const;

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	bool IsCurrentlyPlaying() const { return GetIsIn() && !IsRespawning(); }

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	float GetRemainingRespawnSeconds() const;
	
	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	int32 GetScore() const { return Score; }

	UFUNCTION( BlueprintCallable, Category = "Robot Player" )
	void SetScore( int32 NewScore ) { Score = NewScore; if( Score < 0 ) { Score = 0; } }

	UFUNCTION( BlueprintCallable, Category = "Robot Player" )
	void AddScore( int32 DeltaScore ) { Score += DeltaScore; if( Score < 0 ) { Score = 0; } }

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	FColor GetColour() const { return Colour; }

	UFUNCTION( BlueprintCallable, Category = "Robot Player" )
	void SetColour( FColor NewColour ){ Colour = NewColour; }

	UFUNCTION( BlueprintPure, Category = "Robot Player" )
	int32 GetDeaths() const { return Deaths; }


private:


	UPROPERTY( EditAnywhere, Category = "Robot Player" )
	float RotateInputAxisTolerance;

	UPROPERTY( VisibleAnywhere, Category = "Robot Player" )
	FColor				Colour;

	int32				Score;
	int32				Deaths;

	bool				bIsIn;
	FTimerHandle		RespawnTimerHandle;

	float				LastXInputValue;
	float				LastYInputValue;

	void MoveRight( float Value );
	void MoveUp( float Value );
				    
	void FireRight( float Value );
	void FireUp( float Value );

	void ToggleInGamePause();

	void StartFireCharacterWeapon();
	void EndFireCharacterWeapon();

	void StartRespawn();
	void CancelRespawn();
	void SpawnCharacter();

	bool ShouldRotateWithLastValues() const;

	UFUNCTION()
	void RobotCharacterDeathCallback( float DamageAmount, AController* DamageInstigator, AActor* DamageCauser );
	
};
