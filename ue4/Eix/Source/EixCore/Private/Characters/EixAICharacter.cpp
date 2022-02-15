#include "Characters/EixAICharacter.h"
#include "Components/EixCharacterHealthComp.h"
#include "Components/EixCharacterHitReactionComp.h"
#include "Components/EixEnemyHealthWidgetComp.h"

AEixAICharacter::AEixAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealthComp = CreateDefaultSubobject<UEixCharacterHealthComp>(TEXT("HealthComponent"));
	HitReactionComp = CreateDefaultSubobject<UEixCharacterHitReactionComp>(TEXT("HitReactionComponent"));
	EnemyHealthWidgetComp = CreateDefaultSubobject<UEixEnemyHealthWidgetComp>(TEXT("EnemyHealthWidgetComponent"));
	EnemyHealthWidgetComp->SetupAttachment(RootComponent);
}

float AEixAICharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
                                  AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	HitReactionComp->HandleHit(DamageEvent);
	HealthComp->TakeDamage(ActualDamage);
	
	return ActualDamage;
}

void AEixAICharacter::BeginPlay()
{
	Super::BeginPlay();
	HealthComp->OnHealthChanged().AddLambda([this]()
	{
		EnemyHealthWidgetComp->SetHealthPercentage(HealthComp->GetCurrentHealthPercentage());
	});
}
