#include "MovingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"

AMovingPlatform::AMovingPlatform()
{
    PrimaryActorTick.bCanEverTick = true;

    PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlatformMesh"));
    RootComponent = PlatformMesh;

    PlatformMesh->SetSimulatePhysics(true);
    PlatformMesh->SetMassOverrideInKg(NAME_None, 1000.0f, true);
    PlatformMesh->SetLinearDamping(1.0f);
    PlatformMesh->SetAngularDamping(1.0f);

    PlatformMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    PlatformMesh->SetCollisionObjectType(ECC_PhysicsBody);
    PlatformMesh->SetNotifyRigidBodyCollision(true); // Enables OnHit events
}

void AMovingPlatform::BeginPlay()
{
    Super::BeginPlay();

    if (PlatformMesh)
    {
        PlatformMesh->OnComponentHit.AddDynamic(this, &AMovingPlatform::OnPlatformHit);
    }

    StartLocation = GetActorLocation();
    Direction = (TargetLocation - StartLocation).GetSafeNormal();
}

void AMovingPlatform::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    MovePlatform(DeltaTime);
    TransportObjects();
    CheckObjectsOnPlatform();
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
    if (!PlatformMesh) return;

    FVector CurrentLocation = GetActorLocation();
    FVector Target = bIsMovingForward ? TargetLocation : StartLocation;

    FVector MoveDirection = (Target - CurrentLocation).GetSafeNormal();
    PlatformMesh->AddForce(MoveDirection * Speed * PlatformMesh->GetMass());

    if (FVector::Dist(CurrentLocation, Target) < 5.0f) // If close to target, change direction
    {
        bIsMovingForward = !bIsMovingForward;
    }
}

void AMovingPlatform::OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        FVector ActorLocation = OtherActor->GetActorLocation();
        FVector PlatformLocation = GetActorLocation();
        float HeightThreshold = PlatformLocation.Z + 50.0f; // Ensures the object is ON the platform

        if (ActorLocation.Z > HeightThreshold)
        {
            ObjectsOnPlatform.Add(OtherActor);
        }
    }
}

void AMovingPlatform::TransportObjects()
{
    if (!PlatformMesh) return;

    FVector PlatformVelocity = PlatformMesh->GetPhysicsLinearVelocity();
    TSet<AActor*> ObjectsToRemove;

    for (AActor* Actor : ObjectsOnPlatform)
    {
        if (Actor && Actor->IsA(APawn::StaticClass()))
        {
            UPrimitiveComponent* PawnMesh = Cast<UPrimitiveComponent>(Actor->GetComponentByClass(UPrimitiveComponent::StaticClass()));

            if (PawnMesh && PawnMesh->IsSimulatingPhysics())
            {
                FVector CurrentVelocity = PawnMesh->GetPhysicsLinearVelocity();
                
                FVector VelocityDifference = (PlatformVelocity - FVector(CurrentVelocity.X, CurrentVelocity.Y, 0)) * VelocitySmoothingFactor;
                
                PawnMesh->SetPhysicsLinearVelocity(CurrentVelocity + VelocityDifference, false);
                
                FVector ActorLocation = Actor->GetActorLocation();
                FVector PlatformLocation = GetActorLocation();
                float HeightThreshold = PlatformLocation.Z + 50.0f;

                if (ActorLocation.Z > HeightThreshold + VerticalExitThreshold || 
                    FVector::Dist2D(ActorLocation, PlatformLocation) > HorizontalExitThreshold)
                {
                    ObjectsToRemove.Add(Actor);
                }
            }
        }
    }
    
    for (AActor* Actor : ObjectsToRemove)
    {
        ObjectsOnPlatform.Remove(Actor);
    }
}


void AMovingPlatform::CheckObjectsOnPlatform()
{
    TSet<AActor*> ObjectsToRemove;

    for (AActor* Actor : ObjectsOnPlatform)
    {
        FVector ActorLocation = Actor->GetActorLocation();
        FVector PlatformLocation = GetActorLocation();
        float HeightThreshold = PlatformLocation.Z + 50.0f;

        if (ActorLocation.Z > HeightThreshold + 100.0f)
        {
            ObjectsToRemove.Add(Actor);
        }
    }

    for (AActor* Actor : ObjectsToRemove)
    {
        ObjectsOnPlatform.Remove(Actor);
    }
}