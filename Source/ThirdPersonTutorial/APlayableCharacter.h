// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vector2DWithMinMax.h"
#include "TSQueue.h"
#include "MovementDirections.h"
#include "vector"
#include "TSVector.h"

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
	virtual ~AAPlayableCharacter() noexcept;
	

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

	//Field for camera rotation, updated from inputs and read by character tick
	Vector2DWithMinMax* _cameraXYRotation;

	//Field for camera arm rotation, updated from inputs and read by character tick
	Vector2DWithMinMax* _cameraArmXYRotation;

	

	const float _forwardMovScale = 3.0f;
	const float _sidewaysMovScale = 2.0f;

	//Holds all movement inputs for the character
	//TSQueue<MovementDirection> _movementQueue; This one limits movement input reading to one per tick. as is.

	TSVector<MovementDirection> _movementActions;
	



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

	void MoveRight();

	void MoveLeft();

	void MoveForward();

	void MoveBackwards();

	

};
