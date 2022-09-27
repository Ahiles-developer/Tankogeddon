// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CartridgesBox.generated.h"

UCLASS()
class TANKOGEDDON_API ACartridgesBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACartridgesBox();

protected:
	UFUNCTION()
		void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* CartridgesMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int Ammo = 10;
};
