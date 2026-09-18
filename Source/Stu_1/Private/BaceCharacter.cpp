// Fill out your copyright notice in the Description page of Project Settings.


#include "BaceCharacter.h"
#include "Components/BoxComponent.h"
#include "Items/Weapons/Weapons.h"
#include "Kismet/GameplayStatics.h"
#include "Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"

ABaceCharacter::ABaceCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABaceCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaceCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaceCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaceCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox()) 
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

void ABaceCharacter::Attack()
{
}

int32 ABaceCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaceCharacter::PlayDeathMontage()
{
	return PlayRandomMontageSection(DeathMontage, DeathMontageSections);
}

void ABaceCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool ABaceCharacter::CanAttack()
{
	return true;
}

void ABaceCharacter::AttackEnd()
{

}

void ABaceCharacter::Die()
{

}

void ABaceCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage) {
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
	else
	{
		// 受击蒙太奇静默不播的常见原因：基类上提该 UPROPERTY 后，蓝图里原本挂好的资产引用被孤儿化变 nullptr。
		// 直接在 BP_Enemy / BP_SlashCharacter 的 Class Defaults 里重新赋值 HitReactMontage 即可。
		UE_LOG(LogTemp, Warning, TEXT("PlayHitReactMontage: 受击蒙太奇未播放 — AnimInstance=%s, HitReactMontage=%s"),
			AnimInstance ? TEXT("OK") : TEXT("NULL"),
			HitReactMontage ? TEXT("OK") : TEXT("NULL"));
	}
}

void ABaceCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	PlayHitReactMontage(Section);

}

void ABaceCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaceCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			HitParticles,
			ImpactPoint
		);
	}
}

void ABaceCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

void ABaceCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);

	}
}

int32 ABaceCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionName)
{
	
	if (SectionName.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionName.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionName[Selection]);
	return Selection;
	
}

bool ABaceCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}