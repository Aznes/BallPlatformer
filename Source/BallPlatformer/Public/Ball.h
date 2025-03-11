// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputMappingContext.h"
class AWindController;
#include "Ball.generated.h"

UCLASS()
class BALLPLATFORMER_API ABall : public APawn
{
	GENERATED_BODY()

public:
	ABall();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind Settings")
	AWindController* WindController;

protected:
	virtual void BeginPlay() override;
	
public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void PerformMove(FVector2D direction);
	void PerformJump();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
	float MoveSpeed = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
	float JumpStrength = 300.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
	int JumpCount = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movements")
	float DoubleJumpRange = 105.0f;
	
	bool IsGrounded;

	bool CheckIfGrounded();
};