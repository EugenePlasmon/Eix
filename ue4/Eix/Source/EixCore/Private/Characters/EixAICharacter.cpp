#include "Characters/EixAICharacter.h"
#include "Components/EixCharacterHealthComp.h"

AEixAICharacter::AEixAICharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HealthComp = CreateDefaultSubobject<UEixCharacterHealthComp>(TEXT("HealthComponent"));
}