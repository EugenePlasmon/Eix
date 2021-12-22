#include "Characters/EixPlayerCharacter.h"

AEixPlayerCharacter::AEixPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEixPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEixPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}