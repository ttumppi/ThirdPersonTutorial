// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vector2DWithMinMax.h"
#include "APlayableCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;

UCLASS()
class THIRDPERSONTUTORIAL_API AAPlayableCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAPlayableCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	USpringArmComponent* CameraBoom;

	
	UCameraComponent* FollowCamera;

private:

	Vector2DWithMinMax _cameraXYRotation;

	Vector2DWithMinMax _cameraArmXYRotation;

	FVector2D _characterXYMovement;

	void SetupCamera();

	void PitchCamera(float inputValue);

	void YawCamera(float inputValue);

	void UpdateCameraRotation();

	void BindInputs();

	void MoveForwardInput(float inputValue);

	void MoveBackwardsInput(float inputValue);

	void MoveLeftInput(float inputValue);

	void MoveRightInput(float inputValue);

	void UpdateCharacterPositionByMovement();

	void RotateCharacter();

	void SetMesh();

	void SetStartPositionForCamera();

};
