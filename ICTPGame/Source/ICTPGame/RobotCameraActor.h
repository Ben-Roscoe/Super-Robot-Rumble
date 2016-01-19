// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "RobotCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class ICTPGAME_API ARobotCameraActor : public AActor
{
	GENERATED_BODY()
	
	

public:


	UPROPERTY( VisibleAnywhere, Category = "Robot Camera" )
	USceneComponent*	 LookAtSceneComponent;

	UPROPERTY( VisibleAnywhere, BlueprintReadWrite, Category = "Robot Camera" )
	UCameraComponent*	 CameraComponent;

	UPROPERTY( VisibleAnywhere, Category = "Robot Camera" )
	USpringArmComponent* SpringArmComponent;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Robot Camera" )
	float MinArmLength;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Robot Camera" )
	float MaxArmLength;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Robot Camera" )
	float BoundsScale;

	UPROPERTY( BlueprintReadWrite, EditAnywhere, Category = "Robot Camera" )
	float ExtrapolateSpeed;

	ARobotCameraActor( const FObjectInitializer& ObjectInitializer );
	virtual void Tick( float DeltaSeconds ) override;
	

private:


	FVector		NextPosition;
	float		NextArmLength;
};
