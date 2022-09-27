// Fill out your copyright notice in the Description page of Project Settings.


#include "CartridgesBox.h"
#include "TankPawn.h"

ACartridgesBox::ACartridgesBox()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CartridgesMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CartridgesMesh"));
	CartridgesMesh->SetupAttachment(SceneComp);
	CartridgesMesh->OnComponentBeginOverlap.AddDynamic(this, &ACartridgesBox::OnMeshOverlapBegin);
	CartridgesMesh->SetCollisionProfileName("OverlapAll");
	CartridgesMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CartridgesMesh->SetGenerateOverlapEvents(true);
}

void ACartridgesBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn) {
		Ammo += 10;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("You pick up 10 cartridges!")));
	}
	Destroy();
}

