// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Items\CharacterType.h"
#include "SlashAnimInstance.generated.h"

/**

 * 
 */
UCLASS()

class STU_1_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	void NativeInitializeAnimation();
	void NativeUpdateAnimation(float DeltaTime);
public:
	

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class ASlashCharacter* SlashCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UCharacterMovementComponent* SlashCharacterMovement;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character State")
	ECharacterState CharacterState;
};
