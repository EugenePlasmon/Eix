#include "Characters/EixCharacter.h"
#include "Components/Movement/EixCharacterMovComp.h"

AEixCharacter::AEixCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UEixCharacterMovComp>(ACharacter::CharacterMovementComponentName)
	)
{
	EixCharacterMovement = StaticCast<UEixCharacterMovComp*>(GetCharacterMovement());
}
