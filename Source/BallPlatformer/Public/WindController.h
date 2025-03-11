#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindController.generated.h"

UCLASS()
class BALLPLATFORMER_API AWindController : public AActor
{
	GENERATED_BODY()
    
public:    
	AWindController();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;
	
	void AttachToBall(AActor* TargetBall);
	
	void SetWindControl(bool bIncrease);
	
	void SetWindDirection(FVector2D WindInput);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind Settings", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float WindScaleFactor = 0.5f;

private:
	AActor* TargetBall;
	
	UPROPERTY(EditAnywhere, Category = "Wind Settings", meta = (ClampMin = "0.0"))
	float CurrentWindForce = 0.0f;
	
	UPROPERTY(EditAnywhere, Category = "Wind Settings", meta = (ClampMin = "0.0"))
	float MaxWindForce = 2000.0f;
	
	FVector WindDirection;
	
	bool bIsIncreasingWind = false;
	
	UPROPERTY(EditAnywhere, Category = "Wind Settings", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* WindMesh;
};
