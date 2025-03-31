#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeManager.generated.h"

class APin;
class ABeam;
class ABall;

UCLASS()
class BALLPLATFORMER_API ABridgeManager : public AActor
{
    GENERATED_BODY()

public:
    ABridgeManager();

    virtual void Tick(float DeltaTime) override;
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
    TArray<APin*> Pins;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
    TArray<ABeam*> Beams;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bridge")
    ABall* PlayerBall;

    UFUNCTION(CallInEditor, Category = "Bridge")
    void ClearBridge();
};