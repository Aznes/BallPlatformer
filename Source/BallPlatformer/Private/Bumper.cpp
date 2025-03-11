#include "Bumper.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

ABumper::ABumper()
{
	PrimaryActorTick.bCanEverTick = false;

	BumperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BumperMesh"));
	RootComponent = BumperMesh;

	BumperMesh->SetSimulatePhysics(false);
	BumperMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BumperMesh->SetNotifyRigidBodyCollision(true);
}

void ABumper::BeginPlay()
{
	Super::BeginPlay();
    
	if (BumperMesh)
	{
		BumperMesh->OnComponentHit.AddDynamic(this, &ABumper::OnBumperHit);
	}
}

void ABumper::OnBumperHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
	{
		UPrimitiveComponent* BallMesh = Cast<UPrimitiveComponent>(OtherActor->GetComponentByClass(UPrimitiveComponent::StaticClass()));

		if (BallMesh && BallMesh->IsSimulatingPhysics())
		{
			FVector BounceDirection = -Hit.ImpactNormal.GetSafeNormal();

			FVector BounceImpulse = BounceDirection * BounceForce * BallMesh->GetMass();

			BallMesh->AddImpulse(BounceImpulse, NAME_None, true);
		}
	}
}
