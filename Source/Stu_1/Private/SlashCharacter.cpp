// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Items/Item.h"
#include "Items/Weapons/Weapons.h"
#include "Animation/AnimationAsset.h"
#include "Components/BoxComponent.h"
ASlashCharacter::ASlashCharacter()
{
	
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
}

void ASlashCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(FName("SlashCharacter"));
}

void ASlashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ASlashCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ASlashCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ASlashCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ASlashCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("pick up items"), IE_Pressed, this, &ASlashCharacter::EKeyPressed);
	PlayerInputComponent->BindAction(FName("Attack" ),IE_Pressed, this, &ASlashCharacter::Attack);
	PlayerInputComponent->BindAction(FName("FnsAttack"), IE_Pressed, this, &ASlashCharacter::FnsAttack);
}

void ASlashCharacter::MoveForward(float Value) {
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller != nullptr) && (Value != 0.f)) {
		 // 找到哪个方向向前
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void ASlashCharacter::MoveRight(float Value) {
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if ((Controller != nullptr) && (Value != 0.f)) {
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}
void ASlashCharacter::Turn(float Value) {
	AddControllerYawInput(Value);
}

void ASlashCharacter::LookUp(float Value) {
	AddControllerPitchInput(Value);
}

void ASlashCharacter::EKeyPressed()
{
	AWeapons* OverlappingWeapon = Cast<AWeapons>(OverlappingItem);
	if (OverlappingWeapon) {
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"), this, this);
		

		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else 
	{
		FnsComboIndex = 0; // 装备/卸下武器时清空拳击连击计数
		if (CanDisarm()) {
			PlayEquipMontage(FName("Unequip"));
			CharacterState = ECharacterState::ECS_Unequipped;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
		else if (CanArm()) {
			PlayEquipMontage(FName("Equip"));
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
			ActionState = EActionState::EAS_EquippingWeapon;
		}
	}
}
void ASlashCharacter::Attack() 
{
	const bool bCanAttack =
		ActionState == EActionState::EAS_Unoccupied && 
		CharacterState != ECharacterState::ECS_Unequipped;
	if (bCanAttack)
	{
		FnsComboIndex = 0; // 切换为用剑攻击时清空拳击连击计数
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
	
	

}

void ASlashCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage) {
		AnimInstance->Montage_Play(AttackMontage );
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection) {
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void ASlashCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage) {
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);

	}
}

void ASlashCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}

bool ASlashCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState != ECharacterState::ECS_Unequipped ;
}

bool ASlashCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

void ASlashCharacter::Disarm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket"));
	}
}

void ASlashCharacter::Arm()
{
	if (EquippedWeapon) {
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocket"));
	}
}

void ASlashCharacter::FnsAttack()
{
	// 仅在未装备武器（含已卸下/收起）时触发拳击；并复用 EAS_Attacking 锁定移动，与用剑攻击一致
	if (ActionState == EActionState::EAS_Unoccupied && CharacterState == ECharacterState::ECS_Unequipped)
	{
		// 取消上一轮残留的连击重置计时，避免这一拳刚打出就被计时器清零
		GetWorldTimerManager().ClearTimer(FnsComboResetTimer);
		PlayFnsAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void ASlashCharacter::PlayFnsAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FnsAttackMontage)
	{
		AnimInstance->Montage_Play(FnsAttackMontage);
		// 按顺序依次播放三段拳击：FnsAttack1 -> FnsAttack2 -> FnsAttack3 -> 循环
		FName SectionName;
		switch (FnsComboIndex)
		{
		case 0: SectionName = FName("FnsAttack1"); break;
		case 1: SectionName = FName("FnsAttack2"); break;
		case 2: SectionName = FName("FnsAttack3"); break;
		default: SectionName = FName("FnsAttack1"); break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, FnsAttackMontage);
		FnsComboIndex = (FnsComboIndex + 1) % 3;
	}
}

void ASlashCharacter::FnsAttackEnd()
{
	// 恢复移动：重置行动状态（与用剑攻击的 AttackEnd 一致）
	ActionState = EActionState::EAS_Unoccupied;
	// 开启连击窗口：窗口内再按左键接下一拳；超时未接则 ResetFnsCombo 把连击归零，下一拳从第一招开始
	GetWorldTimerManager().SetTimer(FnsComboResetTimer, this, &ASlashCharacter::ResetFnsCombo, FnsComboWindow, false);
}

void ASlashCharacter::ResetFnsCombo()
{
	FnsComboIndex = 0;
}

void ASlashCharacter::AnimNotify_FnsAttackEnd()
{

	FnsAttackEnd();
}

void ASlashCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) {
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}



void ASlashCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
