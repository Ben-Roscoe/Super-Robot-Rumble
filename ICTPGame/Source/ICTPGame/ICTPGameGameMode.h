// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ICTPGameModeBase.h"
#include "GameFramework/DamageType.h"
#include "RobotPlayerController.h"
#include "RobotCameraActor.h"
#include "RobotHeroCharacter.h"
#include "Runtime/UMG/Public/UMG.h"
#include "ICTPGameGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams( FGameModeActorDeathEvent, float, DamageAmount, AActor*, DamagedActor, AActor*, DamageCauser );

UENUM( BlueprintType )
enum class EPlayerID : uint8
{
	One			= 0,
	Two			= 1,
	Three		= 2,
	Four		= 3,
};

UCLASS()
class ICTPGAME_API AICTPGameGameMode : public AICTPGameModeBase
{
	GENERATED_BODY()
	
	
public:


	UPROPERTY( EditDefaultsOnly, Category = "Game Rules" )
	TSubclassOf<ARobotPlayerController> RobotPlayerControllerClass;

	UPROPERTY( EditDefaultsOnly, Category = "Camera" )
	TSubclassOf<ARobotCameraActor> RobotCameraActorClass;

	UPROPERTY( EditDefaultsOnly, Category = "Game Rules" )
	TSubclassOf<ARobotHeroCharacter> RobotHeroCharacterClass;

	UPROPERTY( BlueprintReadOnly, Category = "Players" )
	TArray<ARobotPlayerController*>		Players;

	UPROPERTY( BlueprintAssignable, Category = "Events" )
	FGameModeActorDeathEvent			OnActorDeath;

	UPROPERTY( BlueprintReadWrite, Category = "Game Rules" )
	float RespawnTime;

	UPROPERTY( BlueprintReadWrite, Category = "Game Rules" )
	int32 DeathScorePenalty;

	AICTPGameGameMode();
	virtual void BeginPlay() override;
	void RegisterActorDeath( float DamageAmount, AActor* DamagedActor, AActor* DamageCauser );
	void OnRobotCharacterDeath( ARobotPlayerController* RobotOwner );

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void TriggerLevelEnd( FName NextLevelName );

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void GoToNextLevel();

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void TryDamage( AActor* Damager, AActor* Damagee, float DamageAmount, TSubclassOf<UDamageType> DamageTypeClass ) const;

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void TryRadialDamage( AActor* Damager, float DamageAmount, float DamageRadius, FVector Origin, TSubclassOf<UDamageType> DamageTypeClass ) const;

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	bool CanDamage( AActor* Damager, AActor* Damagee ) const;

	UFUNCTION( BlueprintPure, Category = "Game Rules" )
	bool GetCanFriendlyFire() const { return bCanFriendlyFire; }

	UFUNCTION( BlueprintPure, Category = "Game Rules" )
	ARobotPlayerController* GetWinner() const;

	UFUNCTION( BlueprintPure, Category = "Game Rules" )
	bool IsAnyonePlaying() const;

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void SetCanFriendlyFire( bool Value ){ bCanFriendlyFire = Value; }

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	float GetRespawnSecondsForPlayer( const ARobotPlayerController* RobotPlayerController ) const;

	UFUNCTION( BlueprintCallable, Category = "Players" )
	ARobotPlayerController* GetRobotPlayer( EPlayerID PlayerID ) const;

	UFUNCTION( BlueprintPure, Category = "Game Rules" )
	ARobotCameraActor* GetCameraActor() const { return RobotCameraActor; }

	UFUNCTION( BlueprintCallable, Category = "Players" )
	FColor GetColourFromPlayerID( EPlayerID PlayerID );

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void SetGamePaused( bool bPaused ) const;

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void EnableInGamePause();

	UFUNCTION( BlueprintCallable, Category = "Game Rules" )
	void DisableInGamePause();

	UFUNCTION( BlueprintPure, Category = "Game Rules" )
	bool IsInGamePaused() const { return bIsInGamePaused; }


protected:


	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual void HandleLeavingMap() override;
	virtual bool ReadyToEndMatch_Implementation() override;


private:


	UPROPERTY( EditDefaultsOnly, Category = "Player Colours" )
	FColor								PlayerOneColour;

	UPROPERTY( EditDefaultsOnly, Category = "Player Colours" )
	FColor								PlayerTwoColour;

	UPROPERTY( EditDefaultsOnly, Category = "Player Colours" )
	FColor								PlayerThreeColour;

	UPROPERTY( EditDefaultsOnly, Category = "Player Colours" )
	FColor								PlayerFourColour;

	UPROPERTY( EditDefaultsOnly, Category = "User Interface" )
	TSubclassOf<UUserWidget>			InGameWidgetClass;

	UPROPERTY()
	UUserWidget*						InGameWidget;

	UPROPERTY( EditDefaultsOnly, Category = "User Interface" )
	TSubclassOf<UUserWidget>			MatchEndResultsWidgetClass;

	UPROPERTY()
	UUserWidget*						MatchEndResultsWidget;

	UPROPERTY( EditDefaultsOnly, Category = "User Interface" )
	TSubclassOf<UUserWidget>			InGamePauseWidgetClass;

	UPROPERTY()
	UUserWidget*						InGamePauseWidget;

	UPROPERTY()
	ARobotCameraActor*					RobotCameraActor;

	FName								NextLevel;
	bool								bHasLevelEndTriggered;
	bool								bIsInGamePaused		= false;
	bool								bCanFriendlyFire;


	ARobotPlayerController* InitialisePlayerController( EPlayerID PlayerID );

};
