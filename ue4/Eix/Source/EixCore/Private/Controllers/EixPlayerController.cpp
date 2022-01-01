#include "Controllers/EixPlayerController.h"
#include "Characters/EixPlayerCharacter.h"

void AEixPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	EixPlayerCharacter = Cast<AEixPlayerCharacter>(InPawn);
}

void AEixPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis(TEXT("MoveForward"), this, &AEixPlayerController::MoveForward);
	InputComponent->BindAxis(TEXT("MoveRight"), this, &AEixPlayerController::MoveRight);
	InputComponent->BindAxis(TEXT("Turn"), this, &AEixPlayerController::Turn);
	InputComponent->BindAxis(TEXT("LookUp"), this, &AEixPlayerController::LookUp);
	InputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AEixPlayerController::StartSprint);
	InputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AEixPlayerController::StopSprint);
	InputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &AEixPlayerController::StartWalk);
	InputComponent->BindAction(TEXT("Walk"), IE_Released, this, &AEixPlayerController::StopWalk);
}

void AEixPlayerController::MoveForward(float Value)
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->MoveForward(Value);
	}
}

void AEixPlayerController::MoveRight(float Value)
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->MoveRight(Value);
	}
}

void AEixPlayerController::Turn(float Value)
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->Turn(Value);
	}
}

void AEixPlayerController::LookUp(float Value)
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->LookUp(Value);
	}
}

void AEixPlayerController::StartSprint()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->StartSprint();
	}
}

void AEixPlayerController::StopSprint()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->StopSprint();
	}
}

void AEixPlayerController::StartWalk()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->StartWalk();
	}
}

void AEixPlayerController::StopWalk()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->StopWalk();
	}
}
