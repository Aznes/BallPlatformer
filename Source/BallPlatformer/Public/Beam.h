#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beam.generated.h"

class APin;

UCLASS()
class BALLPLATFORMER_API ABeam : public AActor
{
	GENERATED_BODY()

public:
	ABeam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float Thickness = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
	float KStiffness;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MaxStretchScaleX = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	float MinStretchScaleX = 2.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meta")
	bool bIsRoad = false;

	UFUNCTION(CallInEditor, Category = "Bridge")
	void UpdateVisual();

	UFUNCTION(BlueprintCallable, Category = "Bridge")
	void SetPins(APin* InPin1, APin* InPin2);

	bool IsBroken() const;
	bool IsConnectedTo(APin* Pin) const;
	bool CheckOverlapWithUnit(const FVector& BallPosition, float Radius) const;
	FVector GetForceAtPin(APin* pin);
	float InitialLength;
	float Mass;
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BeamMesh;

	UPROPERTY(EditAnywhere, Category = "Pins")
	APin* PinConnection1;

	UPROPERTY(EditAnywhere, Category = "Pins")
	APin* PinConnection2;

	bool bIsBroken = false;
	UMaterialInstanceDynamic* DynamicMaterial;

	float ColorForceFactor;
};