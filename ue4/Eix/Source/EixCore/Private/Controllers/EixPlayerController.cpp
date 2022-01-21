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
	InputComponent->BindAction(TEXT("Roll"), IE_Pressed, this, &AEixPlayerController::Roll);
	InputComponent->BindAction(TEXT("PrimaryAttack"), IE_Pressed, this, &AEixPlayerController::PrimaryAttack_Pressed);
	InputComponent->BindAction(TEXT("SecondaryAttack"), IE_Pressed, this, &AEixPlayerController::SecondaryAttack_Pressed);
	InputComponent->BindAction(TEXT("SecondaryAttack"), IE_Released, this, &AEixPlayerController::SecondaryAttack_Released);
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

void AEixPlayerController::Roll()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->Roll();
	}
}

void AEixPlayerController::PrimaryAttack_Pressed()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->PrimaryAttack(IE_Pressed);
	}
}

void AEixPlayerController::SecondaryAttack_Pressed()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->SecondaryAttack(IE_Pressed);
	}
}

void AEixPlayerController::SecondaryAttack_Released()
{
	if (EixPlayerCharacter.IsValid())
	{
		EixPlayerCharacter->SecondaryAttack(IE_Released);
	}
}
