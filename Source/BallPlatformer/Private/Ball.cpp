// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "WindController.h"
#include "EnhancedInputSubsystems.h"

#include "KismetTraceUtils.h"
#include "MovieSceneSequenceID.h"
#include "Components/SphereComponent.h"
#include "Misc/MapErrors.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

ABall::ABall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABall::BeginPlay()
{
	Super::BeginPlay();

	
	if (WindController)
	{
		UE_LOG(LogTemp, Warning, TEXT("WindController successfully referenced in Ball: %s"), *WindController->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WindController NOT assigned in Blueprint!"));
	}
}

void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABall::PerformMove(FVector2D direction)
{
	FVector MoveVector = FVector(direction.X, direction.Y, 0);

	if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Mesh->AddForce(MoveVector  * MoveSpeed, NAME_None, true);
	}
}

void ABall::PerformJump()
{
	if (JumpCount > 0 || CheckIfGrounded())
	{
		if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
		{
			Mesh->AddImpulse(FVector(0, 0, JumpStrength), NAME_None, true);
			JumpCount--;
			IsGrounded = false;
		}
	}
}

bool ABall::CheckIfGrounded()
{
	if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		FVector Start = GetActorLocation();
		FVector End = Start + FVector(0, 0, -DoubleJumpRange);

		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		
		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
		{
			JumpCount = 2;
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}