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
	
	_characterXYMovement = FVector2D(0, 0);

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
	_cameraXYRotation.SetX(_cameraXYRotation.GetX() + inputValue);
}

void AAPlayableCharacter::YawCamera(float inputValue) {
	_cameraXYRotation.SetY(_cameraXYRotation.GetY() + inputValue);

	_cameraArmXYRotation.SetY((_cameraArmXYRotation.GetY() + (inputValue * 1.5f)));
}

void AAPlayableCharacter::UpdateCameraRotation() {
	
	FRotator cameraRotation = FollowCamera->GetRelativeRotation();

	FRotator cameraBoomRotation = CameraBoom->GetRelativeRotation();

	FollowCamera->SetRelativeRotation(FRotator(_cameraXYRotation.GetY(), cameraRotation.Yaw, cameraRotation.Roll));

	cameraRotation = FollowCamera->GetComponentRotation();

	FollowCamera->SetWorldRotation(FRotator(cameraRotation.Pitch, cameraRotation.Yaw, 0.0f));

	CameraBoom->SetRelativeRotation(FRotator(_cameraArmXYRotation.GetY(), _cameraXYRotation.GetX(), cameraBoomRotation.Roll));
	 
	/*DebugFunctions::PrintMessage(3, FColor::Red, "Camera rotation : " + 
	std::string(TCHAR_TO_UTF8(*FollowCamera->GetRelativeRotation().ToString())));
	
	DebugFunctions::PrintMessage(4, FColor::Yellow, "camera input y : " + std::to_string(_cameraXYRotation.GetY()));*/

	DebugFunctions::PrintMessage(3, FColor::Red, "Camera arm rotation : " + 
		std::string(TCHAR_TO_UTF8(*CameraBoom->GetRelativeRotation().ToString())));

	DebugFunctions::PrintMessage(4, FColor::Red, "camera arm y value: " + std::to_string(_cameraArmXYRotation.GetY()));
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
	_characterXYMovement.X -= inputValue;
}

void AAPlayableCharacter::MoveBackwardsInput(float inputValue) {
	_characterXYMovement.X += inputValue;
}

void AAPlayableCharacter::MoveRightInput(float inputValue) {
	_characterXYMovement.Y -= inputValue;
}

void AAPlayableCharacter::MoveLeftInput(float inputValue) {
	_characterXYMovement.Y += inputValue;
}

void AAPlayableCharacter::UpdateCharacterPositionByMovement() {

	FVector currentPosition = GetActorLocation();

	currentPosition.Y += _characterXYMovement.Y;
	_characterXYMovement.Y = 0;

	currentPosition.X += _characterXYMovement.X;
	_characterXYMovement.X = 0;

	SetActorLocation(currentPosition);
}

void AAPlayableCharacter::RotateCharacter() {
	SetActorRotation(FRotator(0.0f, _cameraXYRotation.GetX() + 15, 0.0f));
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
	FRotator wat = FollowCamera->GetRelativeRotation();

	DebugFunctions::PrintMessage(10, FColor::Red, "pitch before change :" + std::to_string(wat.Pitch));


	FollowCamera->SetRelativeRotation(FRotator(20.0f, 15.0f, 0.0f));

	wat = FollowCamera->GetRelativeRotation();

	DebugFunctions::PrintMessage(11, FColor::Red, "pitch after change :" + std::to_string(wat.Pitch));

	FRotator cameraRotation = FollowCamera->GetComponentRotation();

	FollowCamera->SetWorldRotation(FRotator(cameraRotation.Pitch, cameraRotation.Yaw, 0.0f));





	FRotator currentCameraRotation = FollowCamera->GetRelativeRotation();
	FRotator currentCameraBoomRotation = CameraBoom->GetRelativeRotation();

	std::vector<MinMaxSpan> cameraYawLimits = { MinMaxSpan(0.0f, 20.0f) };
	std::vector<MinMaxSpan> cameraPitchLimits = std::vector<MinMaxSpan>();

	std::vector<MinMaxSpan> cameraArmLimits = { MinMaxSpan(-40.0f, 30.0f) };

	_cameraXYRotation = Vector2DWithMinMax(currentCameraRotation.Yaw, currentCameraRotation.Pitch, cameraPitchLimits, cameraYawLimits);

	_cameraArmXYRotation = Vector2DWithMinMax(currentCameraBoomRotation.Yaw, currentCameraBoomRotation.Pitch,
		std::vector<MinMaxSpan>(), cameraArmLimits);

}