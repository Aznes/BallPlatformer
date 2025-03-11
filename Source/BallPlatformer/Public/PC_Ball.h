// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "WindController.h"
class AWindController;
#include "PC_Ball.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;

UCLASS()
class BALLPLATFORMER_API APC_Ball : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem *Subsystem;
	
private:

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMappingContext;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	
	void Move(const FInputActionValue& Value);
	void Jump();
	
	// UPROPERTY(EditAnywhere)
	// class UInputAction* IA_Move;
	// class UInputAction* IA_Jump;

	UPROPERTY()
	class ABall* PlayerCharacter;

	/** Capture les inputs de la souris */
	void HandleWindDirection(const FInputActionValue& Value);

	/** Capture l'input pour la force du vent */
	void HandleWindControl(const FInputActionValue& Value);

	/** Met à jour la direction du vent avec l'input */
	void UpdateWindDirection(const FInputActionValue& Value);

	/** Modifie la force du vent (clic gauche pour augmenter, clic droit pour diminuer) */
	void UpdateWindForce(const FInputActionValue& Value);
	
	AWindController* WindController;
};