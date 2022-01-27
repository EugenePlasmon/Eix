#include "Characters/EixAICharacter.h"
#include "Components/EixCharacterHealthComp.h"
#include "Components/EixCharacterHitReactionComp.h"

AEixAICharacter::AEixAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealthComp = CreateDefaultSubobject<UEixCharacterHealthComp>(TEXT("HealthComponent"));
	HitReactionComp = CreateDefaultSubobject<UEixCharacterHitReactionComp>(TEXT("HitReactionComponent"));
}

float AEixAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                  AController* EventInstigator, AActor* DamageCauser)
{
	HitReactionComp->HandleHit(DamageEvent);
	// TODO: Cause damage to health component
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void AEixAICharacter::BeginPlay()
{
	Super::BeginPlay();
}
