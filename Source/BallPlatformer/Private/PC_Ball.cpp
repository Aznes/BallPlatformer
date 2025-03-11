 // Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Ball.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Ball.h"
#include "WindController.h"

void APC_Ball::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerCharacter=Cast<ABall>(GetPawn());
	// Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	// if (Subsystem)
	// {
	// 	Subsystem->AddMappingContext(InputMappingContext, 0);
	// 	UE_LOG(LogTemp, Warning, TEXT("subsystem loaded"));
	// }

	// Vérifier si le joueur est local
	if (IsLocalController())
	{
		// Récupérer le sous-système Enhanced Input
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

		if (InputSubsystem)
		{
			UInputMappingContext* IMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/IMC_Default.IMC_Default"));
			if (IMC)
			{
				InputSubsystem->AddMappingContext(IMC, 0);
			}
		}
	}

	// Récupérer la balle contrôlée
	if (PlayerCharacter)
	{
		WindController = PlayerCharacter->WindController;
	}
}

void APC_Ball::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(InputComponent))
	{
		UInputAction* IA_Move = LoadObject<UInputAction>(nullptr, TEXT("/Game/IA_Move.IA_Move"));
		if (IA_Move)
		{
			EnhancedInputComp->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APC_Ball::Move);
		}

		UInputAction* IA_Jump = LoadObject<UInputAction>(nullptr, TEXT("/Game/IA_Jump.IA_Jump"));
		if (IA_Jump)
		{
			EnhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Started, this, &APC_Ball::Jump);
		}

		UInputAction* IA_WindDirection = LoadObject<UInputAction>(nullptr, TEXT("/Game/IA_WindDirection.IA_WindDirection"));
		if (IA_WindDirection)
		{
			EnhancedInputComp->BindAction(IA_WindDirection, ETriggerEvent::Triggered, this, &APC_Ball::UpdateWindDirection);
		}
		
		UInputAction* IA_WindControl = LoadObject<UInputAction>(nullptr, TEXT("/Game/IA_WindControl.IA_WindControl"));
		if (IA_WindControl)
		{
			EnhancedInputComp->BindAction(IA_WindControl, ETriggerEvent::Triggered, this, &APC_Ball::UpdateWindForce);
		}
	}
}

void APC_Ball::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ABall* Ball = Cast<ABall>(ControlledPawn))
		{
			Ball->PerformMove(MovementVector);
		}
	}
}

void APC_Ball::Jump()
{
	if (APawn* ControlledPawn = GetPawn())
	{
		if (ABall* Ball = Cast<ABall>(ControlledPawn))
		{
			Ball->PerformJump();
		}
	}
}

void APC_Ball::HandleWindDirection(const FInputActionValue& Value)
{
	FVector2D WindInput = Value.Get<FVector2D>();

	if (WindController)
	{
		WindController->SetWindDirection(WindInput);
	}
}

void APC_Ball::HandleWindControl(const FInputActionValue& Value)
{
	bool bIncrease = Value.Get<bool>();

	if (WindController)
	{
		WindController->SetWindControl(bIncrease);
	}
}

void APC_Ball::UpdateWindDirection(const FInputActionValue& Value)
{
	FVector2D WindInput = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Warning, TEXT("Wind Direction Input: %s"), *WindInput.ToString());

	if (WindController)
	{
		WindController->SetWindDirection(WindInput);
	}
}

void APC_Ball::UpdateWindForce(const FInputActionValue& Value)
{
	float WindInput = Value.Get<float>();  // Enhanced Input gère le negate en float

	bool bIncrease = WindInput > 0.0f;
	bool bDecrease = WindInput < 0.0f;

	UE_LOG(LogTemp, Warning, TEXT("Wind Control Input: %s"), bIncrease ? TEXT("Increase") : TEXT("Decrease"));

	if (WindController)
	{
		if (bIncrease)
		{
			WindController->SetWindControl(true);
		}
		else if (bDecrease)
		{
			WindController->SetWindControl(false);
		}
	}
}
