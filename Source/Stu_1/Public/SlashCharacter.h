// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaceCharacter.h"
#include "Items\CharacterType.h"
#include "SlashCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMonatge;


UCLASS()
class STU_1_API ASlashCharacter : public ABaceCharacter
{
	GENERATED_BODY()

public:
	ASlashCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float Value);
	void LookUp(float Value);
	void EKeyPressed();
	virtual void Attack() override;
	void FnsAttack();
	virtual bool CanAttack() override;

	/*
	* Callbacks for input
	*/
	void PlayFnsAttackMontage();
	void ResetFnsCombo();
	void PlayEquipMontage(FName SectionName);
	virtual void AttackEnd() override;

	bool CanDisarm();
	bool CanArm();

	UFUNCTION(BlueprintCallable)
	void Disarm();

	UFUNCTION(BlueprintCallable)
	void Arm();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();

	// 拳击攻击结束：C++ 负责恢复移动（重置行动状态，与用剑攻击一致）
	UFUNCTION(BlueprintCallable)
	void FnsAttackEnd();


	// 蒙太奇通知桥接：在 FnsAttackMontage 中放置名为 "FnsAttackEnd" 的 Notify 即可触发
	UFUNCTION()
	void AnimNotify_FnsAttackEnd();

private:

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem;



	UPROPERTY(EditDefaultsOnly, Category = Montages);
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* FnsAttackMontage;

	int32 FnsComboIndex = 0;

	// 连击窗口（秒）：上一拳结束后超过该时间未接下一拳，则下一拳从第一招重新开始。可在角色蓝图细节面板调
	UPROPERTY(EditDefaultsOnly, Category = Combat)
	float FnsComboWindow = 1.f;

	FTimerHandle FnsComboResetTimer;



public:
	FORCEINLINE  void SetOverlappingItem(AItem* Item) { OverlappingItem = Item; }
	FORCEINLINE  ECharacterState GetCharacterState() const { return CharacterState; }



};
