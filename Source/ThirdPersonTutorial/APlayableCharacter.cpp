// Fill out your copyright notice in the Description page of Project Settings.


#include "APlayableCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "MinMaxSpan.h"
#include "DebugFunctions.h"
#include "string"

// Sets default values
AAPlayableCharacter::AAPlayableCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetupCamera();
	
	

	SetMesh();
}

// Called when the game starts or when spawned
void AAPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindInputs();

	SetStartPositionForCamera();
}

// Called every frame
void AAPlayableCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCameraRotation();
	RotateCharacter();

	UpdateCharacterPositionByMovement();

	

	
}



// Called to bind functionality to input
void AAPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void AAPlayableCharacter::SetupCamera() {

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 250.0f;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;




	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;



	CameraBoom->SetRelativeRotation(FRotator(330.0f, 0.0f, 0.0f));

	
	

	
}


void AAPlayableCharacter::PitchCamera(float inputValue) {
	_cameraXYRotation->SetX(_cameraXYRotation->GetX() + inputValue);
}

void AAPlayableCharacter::YawCamera(float inputValue) {
	_cameraXYRotation->SetY(_cameraXYRotation->GetY() + inputValue);

	_cameraArmXYRotation->SetY((_cameraArmXYRotation->GetY() + (inputValue * 1.5f)));
}

void AAPlayableCharacter::UpdateCameraRotation() {
	
	FRotator cameraRotation = FollowCamera->GetRelativeRotation();

	FRotator cameraBoomRotation = CameraBoom->GetRelativeRotation();

	FollowCamera->SetRelativeRotation(FRotator(_cameraXYRotation->GetY(), cameraRotation.Yaw, cameraRotation.Roll));

	cameraRotation = FollowCamera->GetComponentRotation();

	FollowCamera->SetWorldRotation(FRotator(cameraRotation.Pitch, cameraRotation.Yaw, 0.0f));

	CameraBoom->SetRelativeRotation(FRotator(_cameraArmXYRotation->GetY(), _cameraXYRotation->GetX(), cameraBoomRotation.Roll));
	 
	
}

void AAPlayableCharacter::BindInputs() {
	InputComponent->BindAxis("CameraPitch", this, &AAPlayableCharacter::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &AAPlayableCharacter::YawCamera);

	InputComponent->BindAxis("MoveForward", this, &AAPlayableCharacter::MoveForwardInput);
	InputComponent->BindAxis("MoveBackwards", this, &AAPlayableCharacter::MoveBackwardsInput);
	InputComponent->BindAxis("MoveRight", this, &AAPlayableCharacter::MoveRightInput);
	InputComponent->BindAxis("MoveLeft", this, &AAPlayableCharacter::MoveLeftInput);
}

void AAPlayableCharacter::MoveForwardInput(float inputValue) {

	if (inputValue == 0.0f) {
		return;
	}

	MovementDirection* input = new MovementDirection(MovementDirection::MoveForward);

	_movementQueue.Enqueue(input);

}

void AAPlayableCharacter::MoveBackwardsInput(float inputValue) {

	if (inputValue == 0.0f) {
		return;
	}

	MovementDirection* input = new MovementDirection(MovementDirection::MoveBackwards);

	_movementQueue.Enqueue(input);

}

void AAPlayableCharacter::MoveRightInput(float inputValue) {

	if (inputValue == 0.0f) {
		return;
	}

	MovementDirection* input = new MovementDirection(MovementDirection::MoveRight);

	_movementQueue.Enqueue(input);

}

void AAPlayableCharacter::MoveLeftInput(float inputValue) {

	if (inputValue == 0.0f) {
		return;
	}

	MovementDirection* input = new MovementDirection(MovementDirection::MoveLeft);

	_movementQueue.Enqueue(input);

}

void AAPlayableCharacter::UpdateCharacterPositionByMovement() {

	
	if (_movementQueue.Count() <= 0) {
		return;
	}

	FVector facingDirection = GetActorForwardVector();


	DebugFunctions::PrintMessage(10, FColor::Red, "facingDirection Y : " + std::to_string(facingDirection.Y));


	DebugFunctions::PrintMessage(12, FColor::Red, "facingDirection X : " + std::to_string(facingDirection.X));

	MovementDirection* movementAction = _movementQueue.Dequeue();
		
	switch (*movementAction) {

	case MovementDirection::MoveForward: // Do nothing, already facing forwards.
		break;

	case MovementDirection::MoveBackwards:
		facingDirection.X -= 0.90f;
		//facingDirection.Y -= 0.90f;
		break;

	case MovementDirection::MoveLeft:

		break;

	case MovementDirection::MoveRight:

		break;

	default:
		DebugFunctions::PrintMessage(11, FColor::Red, "Unrecognised movement input handled");
		break;
	}


	AddActorWorldOffset(FVector(facingDirection.X * _forwardMovScale,
		facingDirection.Y * _forwardMovScale, 0.0f)); 
		
	delete movementAction;
	
	movementAction = nullptr;
	
}

void AAPlayableCharacter::RotateCharacter() {
	SetActorRotation(FRotator(0.0f, _cameraXYRotation->GetX() + 15, 0.0f));
}

void AAPlayableCharacter::SetMesh() {

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> characterMeshAsset(TEXT("/Game/Assets/GenericMale.GenericMale"));

	if (characterMeshAsset.Succeeded()) {
		USkeletalMeshComponent* characterMesh = GetMesh();
		characterMesh->SetupAttachment(RootComponent);
		characterMesh->SetSkeletalMesh(characterMeshAsset.Object);

		characterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	}
	else {
		DebugFunctions::PrintMessage(1, FColor::Red, "Failed to find character mesh");
	}

	
}

void AAPlayableCharacter::SetStartPositionForCamera() {

	FRotator cameraRelativeRotation = FollowCamera->GetRelativeRotation();

	//DebugFunctions::PrintMessage(10, FColor::Red, "pitch before change :" + std::to_string(wat.Pitch));


	FollowCamera->SetRelativeRotation(FRotator(20.0f, 15.0f, 0.0f));

	cameraRelativeRotation = FollowCamera->GetRelativeRotation();

	//DebugFunctions::PrintMessage(11, FColor::Red, "pitch after change :" + std::to_string(wat.Pitch));

	FRotator cameraRotation = FollowCamera->GetComponentRotation();

	FollowCamera->SetWorldRotation(FRotator(cameraRotation.Pitch, cameraRotation.Yaw, 0.0f));





	FRotator currentCameraRotation = FollowCamera->GetRelativeRotation();
	FRotator currentCameraBoomRotation = CameraBoom->GetRelativeRotation();

	std::vector<MinMaxSpan> cameraYawLimits = { MinMaxSpan(0.0f, 20.0f) };
	std::vector<MinMaxSpan> cameraPitchLimits = std::vector<MinMaxSpan>();

	std::vector<MinMaxSpan> cameraArmLimits = { MinMaxSpan(-40.0f, 30.0f) };


	_cameraXYRotation = new Vector2DWithMinMax(currentCameraRotation.Yaw, currentCameraRotation.Pitch, cameraPitchLimits, cameraYawLimits);

	_cameraArmXYRotation = new Vector2DWithMinMax(currentCameraBoomRotation.Yaw, currentCameraBoomRotation.Pitch,
		std::vector<MinMaxSpan>(), cameraArmLimits);

}

void AAPlayableCharacter::MoveRight() {

}

void AAPlayableCharacter::MoveLeft() {

}

void AAPlayableCharacter::MoveForward() {

}

void AAPlayableCharacter::MoveBackwards() {

}

AAPlayableCharacter::~AAPlayableCharacter() noexcept{
	delete _cameraXYRotation;
	delete _cameraArmXYRotation;
}