#include "WindController.h"
#include "Components/PrimitiveComponent.h"

AWindController::AWindController()
{
    PrimaryActorTick.bCanEverTick = true;

    WindMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WindMesh"));
    WindMesh->SetupAttachment(RootComponent);
    WindMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWindController::BeginPlay()
{
    Super::BeginPlay();

    if (TargetBall)
    {
        UE_LOG(LogTemp, Warning, TEXT("WindController attached to: %s"), *TargetBall->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("WindController NOT attached to any ball!"));
    }
}

void AWindController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (TargetBall)
    {

    }
}

void AWindController::SetWindControl(bool bIncrease)
{
    bIsIncreasingWind = bIncrease;
}

void AWindController::SetWindDirection(FVector2D WindInput)
{
    if (TargetBall)
    {
        FVector BallLocation = TargetBall->GetActorLocation();
        
        FVector InputDirection = FVector(WindInput.X, WindInput.Y, 0.0f).GetSafeNormal();
        
        WindDirection = InputDirection.RotateAngleAxis(90.0f, FVector(0, 0, 1));
    }
}