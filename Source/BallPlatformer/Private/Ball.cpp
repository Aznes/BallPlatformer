// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "EnhancedInputSubsystems.h"

#include "KismetTraceUtils.h"
#include "MovieSceneSequenceID.h"
#include "Components/SphereComponent.h"
#include "Misc/MapErrors.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"

// Sets default values
ABall::ABall()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABall::PerformMove(FVector2D direction)
{
	FVector MoveVector = FVector(direction.X, direction.Y, 0);

	if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Mesh->AddForce(MoveVector  * MoveSpeed, NAME_None, true); // Appliquer une force pour le déplacement
	}
}

void ABall::PerformJump()
{
	if (UStaticMeshComponent* Mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		Mesh->AddImpulse(FVector(0, 0, JumpStrength), NAME_None, true); // Appliquer une impulsion verticale
	}
}
