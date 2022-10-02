// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"

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

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankController>(GetController());

	SetupCannon(EquippedCannonClass);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Move Forward;
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime +
		RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	SetActorLocation(movePosition);

	// Body Rotation
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, RotateRightAxisValue, RotateInterpolationKey);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentRightAxisValue %f, RotateRightAxisValue %f"), CurrentRightAxisValue, RotateRightAxisValue);
	float yawRotation = CurrentRightAxisValue * RotationSpeed * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);

	// Turret Rotation
	if (TankController) {
		FVector MousePos = TankController->GetMousePos();
		FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), MousePos);
		FRotator turretRotation = TurretMesh->GetComponentRotation();
		targetRotation.Pitch = turretRotation.Pitch;
		targetRotation.Roll = turretRotation.Roll;
		TurretMesh->SetWorldRotation(FMath::Lerp(targetRotation, turretRotation, TurretInterpolationKey));
	}
}

void ATankPawn::MoveForward(float Value) {
	ForwardMoveAxisValue = Value;
}

void ATankPawn::MoveRight(float Value) {
	RightMoveAxisValue = Value;
}

void ATankPawn::RotateRight(float Value) {
	RotateRightAxisValue = Value;
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannon) {
	if (!newCannon) {
		return;
	}

	if (Cannon) {
		Cannon->Destroy();
	}

	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannon, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::ChangeCannon() {
	TSubclassOf<ACannon> CachedCannon;
	CachedCannon = EquippedCannonClass;
	EquippedCannonClass = SecondCannonClass;
	SecondCannonClass = CachedCannon;
	SetupCannon(EquippedCannonClass);
}

void ATankPawn::Fire() {
	if (Cannon) {
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial() {
	if (Cannon) {
		Cannon->FireSpecial();
	}
}

void ATankPawn::TakeDamage(FDamageData DamageData) {
	HealthComponent->TakeDamage(DamageData);
}

void ATankPawn::DamageTaked(float Value) {
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage: %f, health %f"), *GetName(), Value, HealthComponent->GetHealth());
}

void ATankPawn::Die() {
	if (Cannon) {
		Cannon->Destroy();
	}
	Destroy();
}

