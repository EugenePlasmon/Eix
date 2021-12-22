#include "Characters/EixPlayerCharacter.h"
#include "Components/Movement/EixPlayerCharacterMovComp.h"

AEixPlayerCharacter::AEixPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UEixPlayerCharacterMovComp>(ACharacter::CharacterMovementComponentName)
	)
{
	EixPlayerCharacterMovement = StaticCast<UEixPlayerCharacterMovComp*>(GetCharacterMovement());
}


void AEixPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEixPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}