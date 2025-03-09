 // Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Ball.h"

#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Ball.h"

void APC_Ball::BeginPlay()
{
	Super::BeginPlay();
	
	// PlayerCharacter=Cast<ABall>(GetPawn());
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
			UE_LOG(LogTemp, Warning, TEXT("Subsystem loaded correctly."));
			
			// Charger dynamiquement le Mapping Context
			UInputMappingContext* IMC = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/IMC_Default.IMC_Default"));
			if (IMC)
			{
				UE_LOG(LogTemp, Warning, TEXT("IMC_Default Loaded Successfully! Adding to Mapping Context..."));
				InputSubsystem->AddMappingContext(IMC, 0);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to load IMC_Default! Check the path."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load Enhanced Input Subsystem!"));
		}
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
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load IA_Move. Check the asset path!"));
		}

		UInputAction* IA_Jump = LoadObject<UInputAction>(nullptr, TEXT("/Game/IA_Jump.IA_Jump"));
		if (IA_Jump)
		{
			EnhancedInputComp->BindAction(IA_Jump, ETriggerEvent::Triggered, this, &APC_Ball::Jump);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to load IA_Jump. Check the asset path!"));
		}
	}
	
	// if (PlayerCharacter)
	// {
	// 	if (UEnhancedInputComponent *EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	// 	{
	// 		EnhancedInputComponent->BindAction(IA_Move,ETriggerEvent::Triggered,PlayerCharacter, &ABall::Move);
	// 		EnhancedInputComponent->BindAction(IA_Jump,ETriggerEvent::Triggered,PlayerCharacter, &ABall::Jump);
	// 	}
	// }
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