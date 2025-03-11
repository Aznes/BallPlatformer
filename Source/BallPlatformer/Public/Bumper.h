#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bumper.generated.h"

UCLASS()
class BALLPLATFORMER_API ABumper : public AActor
{
	GENERATED_BODY()
    
public:    
	ABumper();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bumper", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BumperMesh;

	UPROPERTY(EditAnywhere, Category = "Bumper Settings", meta = (ClampMin = "0.0"))
	float BounceForce = 2000.0f;

	UFUNCTION()
	void OnBumperHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
