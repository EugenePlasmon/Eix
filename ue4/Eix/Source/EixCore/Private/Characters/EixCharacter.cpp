#include "Characters/EixCharacter.h"
#include "Components/EixCharacterIKComp.h"
#include "Components/Movement/EixCharacterMovComp.h"

AEixCharacter::AEixCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer
		.SetDefaultSubobjectClass<UEixCharacterMovComp>(ACharacter::CharacterMovementComponentName)
	)
{
	EixCharacterMovement = StaticCast<UEixCharacterMovComp*>(GetCharacterMovement());
	IKComp = CreateDefaultSubobject<UEixCharacterIKComp>(TEXT("IKComponent"));
}

void AEixCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	IKComp->UpdateIK(DeltaSeconds);
}

void AEixCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEixCharacter::SetDesiredGait(EEixGait In_DesiredGait)
{
	DesiredGait = In_DesiredGait;
	EixCharacterMovement->SetCurrentGait(GetCurrentAllowedGait());
}

EEixGait AEixCharacter::GetCurrentAllowedGait() const
{
	return DesiredGait;
}
