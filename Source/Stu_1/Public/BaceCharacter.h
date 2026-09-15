// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/HitInterface.h"
#include "BaceCharacter.generated.h"

class AWeapons;
class UAttributeComponent;
class UAnimMontage;
class UNiagaraSystem;

UCLASS()
class STU_1_API ABaceCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaceCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

protected:
	virtual void BeginPlay() override;
	virtual void Attack();
	virtual void Die();
	/**
	* Animation montages
	*/
	virtual void PlayAttackMontage();
	void PlayHitReactMontage(const FName& SectionName);
	void DirectionalHitReact(const FVector& ImpactPoint);

	virtual bool CanAttack();

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	UPROPERTY(VisibleAnywhere, Category = Weapon);
	AWeapons* EquippedWeapon;
	
	/**
	* Animation montages
	*/

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* DeathMontage;

	/*
	* Components
	*/

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = VisualEffects)
	UNiagaraSystem* HitParticles;
};
