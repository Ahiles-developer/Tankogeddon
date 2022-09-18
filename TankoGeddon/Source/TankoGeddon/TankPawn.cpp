// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// MoveForward;
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime +
		RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	SetActorLocation(movePosition);
}

void ATankPawn::MoveForward(float Value) {
	ForwardMoveAxisValue = Value;
}

void ATankPawn::MoveRight(float Value) {
	RightMoveAxisValue = Value;
}


