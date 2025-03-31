#include "Beam.h"
#include "Pin.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ABeam::ABeam()
{
    PrimaryActorTick.bCanEverTick = true;

    BeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BeamMesh"));
    RootComponent = BeamMesh;

    BeamMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    BeamMesh->SetSimulatePhysics(true);
    BeamMesh->SetEnableGravity(false);
    BeamMesh->SetMobility(EComponentMobility::Movable);
    BeamMesh->SetNotifyRigidBodyCollision(true);
}

void ABeam::BeginPlay()
{
    Super::BeginPlay();
    InitialLength = (PinConnection1->GetActorLocation() - PinConnection2->GetActorLocation()).Size();
    Mass = PinConnection1->Mass * InitialLength;

    if (BeamMesh && BeamMesh->GetMaterial(0))
    {
        DynamicMaterial = UMaterialInstanceDynamic::Create(BeamMesh->GetMaterial(0), this);
        BeamMesh->SetMaterial(0, DynamicMaterial);
    }
}

void ABeam::SetPins(APin* InPin1, APin* InPin2)
{
    PinConnection1 = InPin1;
    PinConnection2 = InPin2;
    UpdateVisual();
}

void ABeam::UpdateVisual()
{
    if (!PinConnection1 || !PinConnection2 || bIsBroken) return;

    FVector Pos1 = PinConnection1->GetActorLocation();
    FVector Pos2 = PinConnection2->GetActorLocation();
    FVector Delta = Pos2 - Pos1;
    float Length = Delta.Size();
    FVector Center = (Pos1 + Pos2) * 0.5f;

    SetActorLocation(Center);
    FRotator Rotation = FRotationMatrix::MakeFromX(Delta).Rotator();
    SetActorRotation(Rotation);

    float ScaleX = Length / 100.0f;
    FVector MeshScale = FVector(ScaleX, Thickness / 10.0f, Thickness / 10.0f);
    BeamMesh->SetWorldScale3D(MeshScale);

    if (ScaleX > MaxStretchScaleX || ScaleX < MinStretchScaleX)
    {
        bIsBroken = true;
        Destroy();
    }

    if (DynamicMaterial)
    {
        float ColorIntensity = FMath::Clamp(1.0f - ColorForceFactor, 0.0f, 1.0f);
        DynamicMaterial->SetVectorParameterValue("BaseColor", FLinearColor(1.0f, ColorIntensity, ColorIntensity));
    }
}

FVector ABeam::GetForceAtPin(APin* pin) {
    //Determine the current length and displacement of the beam.
    FVector lengthCurrent = PinConnection1->GetActorLocation() - PinConnection2->GetActorLocation();
    FVector displacement = FVector(lengthCurrent).GetSafeNormal() * (lengthCurrent.Size() - InitialLength);

    //Determine the forces.
    FVector forceBeam = displacement * KStiffness;
    FVector forceGravity = FVector(0.0f, 0.0f, Mass * 9.8067f); //gravity

    ColorForceFactor = forceBeam.Size() / MaxStretchScaleX;
    
    if (ColorForceFactor >= 1.0f)
    {
        ColorForceFactor = 1.0f;
    }
    else if (ColorForceFactor <= 0.0f)
    {
        ColorForceFactor = 0.0f;
    }
    
    //Output the forces.
    if (PinConnection1 == pin)
        return (forceBeam * -1.0f) + (forceGravity / 2.0f);
    else if (PinConnection2 == pin)
        return forceBeam + (forceGravity / 2.0f);

    return FVector();
}

void ABeam::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateVisual();
    PinConnection1->AccumulatedForces += GetForceAtPin(PinConnection1);
    PinConnection2->AccumulatedForces += GetForceAtPin(PinConnection2);
}

bool ABeam::IsBroken() const
{
    return bIsBroken;
}

bool ABeam::IsConnectedTo(APin* Pin) const
{
    return (Pin == PinConnection1 || Pin == PinConnection2);
}

bool ABeam::CheckOverlapWithUnit(const FVector& BallPosition, float Radius) const
{
    if (!PinConnection1 || !PinConnection2) return false;

    FVector A = PinConnection1->GetActorLocation();
    FVector B = PinConnection2->GetActorLocation();

    FVector AB = B - A;
    FVector AP = BallPosition - A;

    float T = FVector::DotProduct(AP, AB) / AB.SizeSquared();
    T = FMath::Clamp(T, 0.0f, 1.0f);

    FVector Closest = A + T * AB;
    float Distance = FVector::Dist(BallPosition, Closest);

    return Distance < Radius + Thickness;
}