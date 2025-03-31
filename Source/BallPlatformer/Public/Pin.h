#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pin.generated.h"

class ABeam;
class ABall;

UCLASS()
class BALLPLATFORMER_API APin : public AActor
{
    GENERATED_BODY()

public:
    APin();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    float Mass = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
    bool bFixed = false;

    UFUNCTION(CallInEditor, Category = "Bridge")
    void UpdateVisual();

    void CalculateForces(const TArray<ABeam*>& Beams, ABall* Ball);
    void SimulateStep(float DeltaTime);

    FVector AccumulatedForces;
    
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    FVector Velocity;

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    void ResetForces();
    void ApplyForces(float DeltaTime);
};