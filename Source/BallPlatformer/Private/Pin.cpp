#include "Pin.h"
#include "Beam.h"
#include "Ball.h"
#include "Components/StaticMeshComponent.h"

APin::APin()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PinMesh"));
    RootComponent = Mesh;

    Mesh->SetSimulatePhysics(false);
    Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APin::BeginPlay()
{
    Super::BeginPlay();
}

void APin::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ApplyForces(DeltaTime);
}

void APin::ResetForces()
{
    AccumulatedForces = FVector::ZeroVector;
}

void APin::CalculateForces(const TArray<ABeam*>& Beams, ABall* Ball)
{
    ResetForces();

    // for (ABeam* Beam : Beams)
    // {
    //     if (Beam && Beam->IsConnectedTo(this))
    //     {
    //         FVector Direction = Beam->IsConnectedTo(this) ? Beam->GetActorLocation() - GetActorLocation() : FVector::ZeroVector;
    //         AccumulatedForces += Direction; // Simplified force placeholder
    //     }
    // }
}

void APin::ApplyForces(float DeltaTime)
{
    if (bFixed) return;

    ResetForces();
    FVector Acceleration = AccumulatedForces / Mass;
    Velocity += Acceleration * DeltaTime;

    Mesh->AddForce(Velocity);
}

void APin::SimulateStep(float DeltaTime)
{
    ApplyForces(DeltaTime);
}

void APin::UpdateVisual()
{
    // Optionally override to visually update pin in editor
}
