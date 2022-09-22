// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStructs.h"
#include "Cannon.generated.h"

UCLASS()
class TANKOGEDDON_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	void Fire();
	void Reload();
	bool IsReadyToFire();

	FTimerHandle ReloadTimer;
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	ECannonType CannonType = ECannonType::FireProjectile;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire params")
	float FireRate = 1.0f;
		
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire params")
	float FireRange = 1000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Fire params")
	float FireDamage = 1.0f;

private:
	bool bReadyToFire = false;
};
