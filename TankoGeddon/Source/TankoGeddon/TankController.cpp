// Fill out your copyright notice in the Description page of Project Settings.


#include "TankController.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"

ATankController::ATankController() {
	bShowMouseCursor = true;
}

void ATankController::SetPawn(APawn* InPawn) {
	Super::SetPawn(InPawn);
	TankPawn = Cast<ATankPawn>(InPawn);
}

void ATankController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward", this, &ATankController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankController::RotateRight);
}

void ATankController::Tick(float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	FVector MouseDirection;
	DeprojectMousePositionToWorld(MousePos, MouseDirection);
	FVector tanklPosotion = TankPawn->GetActorLocation();
	MousePos.Z = tanklPosotion.Z;
	FVector dir = MousePos - tanklPosotion;
	dir.Normalize();
	MousePos = tanklPosotion + dir * 1000.0f;
	DrawDebugLine(GetWorld(), tanklPosotion, MousePos, FColor::Green, false, 0.0f, 0, 5);
}

void ATankController::MoveForward(float Value) {
	if (TankPawn) {
		TankPawn->MoveForward(Value);
	}
}

void ATankController::MoveRight(float Value) {
	if (TankPawn) {
		TankPawn->MoveRight(Value);
	}
}

void ATankController::RotateRight(float Value) {
	if (TankPawn) {
		TankPawn->RotateRight(Value);
	}
}

