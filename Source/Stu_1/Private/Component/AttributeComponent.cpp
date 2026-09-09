// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// CDO 默认值：蓝图细节面板里改过的值会覆盖这里
	Health = 100.f;
	MaxHealth = 100.f;
}


void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

}

float UAttributeComponent::GetHealthPercent()
{
	return Health / MaxHealth;
}


void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

bool UAttributeComponent::IsAlive() {
	return Health > 0.f;
}

