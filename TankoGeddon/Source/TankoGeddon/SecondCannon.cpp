// Fill out your copyright notice in the Description page of Project Settings.


#include "SecondCannon.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "TimerManager.h"
#include "Projectile.h"


ASecondCannon::ASecondCannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(sceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::BeginPlay() {
	Super::BeginPlay();
	Reload();
}

void ACannon::Fire() {
	if (!IsReadyToFire()) {
		return;
	}
	bReadyToFire = false;

	if (CannonType == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile"))); // можно добавить учет патронов

		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
			ProjectileSpawnPoint->GetComponentLocation(),
			ProjectileSpawnPoint->GetComponentRotation());

		if (projectile) {
			projectile->Start();
		}
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace")));
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

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::FireSpecial() {
	if (!IsReadyToFire()) {
		return;
	}
	bReadyToFire = false;

	if (Ammo <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Need ammo!")));
		return;
	}

	if (CannonType == ECannonType::FireProjectile) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Ammo: %d"), Ammo));
		for (int i = 0; i <= 2; i++) {
			if (CannonType == ECannonType::FireProjectile) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire Special")));
			}
		}
	}
	Ammo--;

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);
}

void ACannon::Reload() {
	bReadyToFire = true;
	//GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Fire, 1 / FireRate, true, 1 / FireRate); бесконечная стрельба
}

bool ACannon::IsReadyToFire() {
	return bReadyToFire;
}



