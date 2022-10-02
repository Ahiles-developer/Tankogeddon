// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameStructs.h"
#include "HealthComponent.generated.h"

DECLARE_EVENT(UHealthComponent, FOnDie);
DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKOGEDDON_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	FOnDie OnDie;
	FOnHealthChanged OnHealthChanged;

	void TakeDamage(FDamageData DamageData);
	float GetHealth() const;
	void AddHealth(float newHealth);

	UFUNCTION(BlueprintCallable)
	float GetHealthState();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health value");
	float MaxHealth = 10.0f;

	float CurrentHealth;
};
