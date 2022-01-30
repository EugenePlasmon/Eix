#include "Components/EixCharacterHealthComp.h"

UEixCharacterHealthComp::UEixCharacterHealthComp()
{
}

void UEixCharacterHealthComp::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
}

void UEixCharacterHealthComp::TakeDamage(float DamageAmount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.f, MaxHealth);
	if (FMath::IsNearlyZero(CurrentHealth))
	{
		
	}
}
