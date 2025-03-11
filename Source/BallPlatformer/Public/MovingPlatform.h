#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class BALLPLATFORMER_API AMovingPlatform : public AActor
{
    GENERATED_BODY()

public:
    AMovingPlatform();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    /** Static Mesh component for the platform */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* PlatformMesh;

    /** Initial location of the platform */
    FVector StartLocation;

    /** Target location for the platform movement */
    UPROPERTY(EditAnywhere, Category = "Platform Movement")
    FVector TargetLocation;

    /** Movement speed of the platform */
    UPROPERTY(EditAnywhere, Category = "Platform Movement", meta = (ClampMin = "0.0"))
    float Speed = 200.0f;

    /** Factor for smooth velocity transition when moving the player */
    UPROPERTY(EditAnywhere, Category = "Platform Physics", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float VelocitySmoothingFactor = 0.15f;

    /** Vertical threshold for detecting when the player leaves the platform */
    UPROPERTY(EditAnywhere, Category = "Platform Physics", meta = (ClampMin = "0.0"))
    float VerticalExitThreshold = 100.0f;

    /** Horizontal threshold for detecting when the player leaves the platform */
    UPROPERTY(EditAnywhere, Category = "Platform Physics", meta = (ClampMin = "0.0"))
    float HorizontalExitThreshold = 150.0f;

    /** Direction of movement */
    FVector Direction;

    /** Boolean to track movement direction */
    bool bIsMovingForward = true;

    /** Keeps track of objects currently on the platform */
    TSet<AActor*> ObjectsOnPlatform;

    /** Handles collision detection when an object lands on the platform */
    UFUNCTION()
    void OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** Handles objects leaving the platform */
    void CheckObjectsOnPlatform();

    /** Moves the platform using physics */
    void MovePlatform(float DeltaTime);

    /** Applies platform velocity to objects standing on it */
    void TransportObjects();
};
