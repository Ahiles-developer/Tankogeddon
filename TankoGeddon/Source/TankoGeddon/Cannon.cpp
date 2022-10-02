// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Projectile.h"


ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(sceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}

void ACannon::Fire() {
	if (!IsReadyToFire() || Shells <= 0) {
		return;
	}
	bReadyToFire = false;
	Shells--;

	if(CannonType == ECannonType::FireProjectile) {
		FireProjectile();
	} else {
		FireTrace();
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial() {
	if (!IsReadyToFire() || Shells <= 0) {
		return;
	}
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::Burst, BurstInterval, true, 0.0f);
}

void ACannon::Reload() {
	bReadyToFire = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shells: %d"), Shells));
}

bool ACannon::IsReadyToFire() {
	return bReadyToFire;
}

void ACannon::FireProjectile() {
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());

	if (projectile) {
		projectile->Start();
	}
}

void ACannon::FireTrace() {
	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, traceParams)) {
		DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Purple, false, 1.0f, 0, 5.0f);
		if (hitResult.GetActor()) {
			UE_LOG(LogTemp, Warning, TEXT("trace overlap : %s"), *hitResult.GetActor()->GetName());
			hitResult.GetActor()->Destroy();
		}
	} else {
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
	}
}

void ACannon::Burst() {
	if (CurrentBurts == BurstSize) {
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		bReadyToFire = true;
		CurrentBurts = 0;
		Shells--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shells is: %d"), Shells));
		return;
	}

	CurrentBurts++;

	if (CannonType == ECannonType::FireProjectile) {
		FireProjectile();
	} else {
		FireTrace();
	}
}

