#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuicksandZone.generated.h"

UCLASS()
class BALLPLATFORMER_API AQuicksandZone : public AActor
{
    GENERATED_BODY()
    
public:    
    AQuicksandZone();

protected:
    virtual void BeginPlay() override;

public:    
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quicksand", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* QuicksandMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quicksand", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* QuicksandTrigger;

    UPROPERTY(EditAnywhere, Category = "Quicksand Settings", meta = (ClampMin = "0.0"))
    float SinkingForce = 100.0f;

    UPROPERTY(EditAnywhere, Category = "Quicksand Settings", meta = (ClampMin = "0.0"))
    float AntiGravityForce = 900.0f;

    UPROPERTY(EditAnywhere, Category = "Quicksand Settings", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float MovementResistance = 0.5f;

    UPROPERTY(EditAnywhere, Category = "Quicksand Settings", meta = (ClampMin = "0.0", ClampMax = "10.0"))
    float JumpResistance = 0.4f;

    TSet<AActor*> AffectedPawns;

    UFUNCTION()
    void OnQuicksandEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnQuicksandExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    void ApplyQuicksandPhysics(float DeltaTime);
};