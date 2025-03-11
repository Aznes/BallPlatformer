#include "QuicksandZone.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AQuicksandZone::AQuicksandZone()
{
    PrimaryActorTick.bCanEverTick = true;

    QuicksandMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("QuicksandMesh"));
    RootComponent = QuicksandMesh;
    QuicksandMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    QuicksandMesh->SetGenerateOverlapEvents(true);

    QuicksandTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("QuicksandTrigger"));
    QuicksandTrigger->SetupAttachment(RootComponent);
    QuicksandTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    QuicksandTrigger->SetGenerateOverlapEvents(true);
    QuicksandTrigger->OnComponentBeginOverlap.AddDynamic(this, &AQuicksandZone::OnQuicksandEnter);
    QuicksandTrigger->OnComponentEndOverlap.AddDynamic(this, &AQuicksandZone::OnQuicksandExit);
}

void AQuicksandZone::BeginPlay()
{
    Super::BeginPlay();
}

void AQuicksandZone::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ApplyQuicksandPhysics(DeltaTime);
}

void AQuicksandZone::OnQuicksandEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        AffectedPawns.Add(OtherActor);

        ACharacter* Character = Cast<ACharacter>(OtherActor);
        if (Character)
        {
            UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
            if (MovementComp)
            {
                MovementComp->JumpZVelocity *= JumpResistance;
            }
        }
    }
}

void AQuicksandZone::OnQuicksandExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor)
    {
        AffectedPawns.Remove(OtherActor);

        ACharacter* Character = Cast<ACharacter>(OtherActor);
        if (Character)
        {
            UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
            if (MovementComp)
            {
                MovementComp->JumpZVelocity /= JumpResistance;
            }
        }
    }
}

void AQuicksandZone::ApplyQuicksandPhysics(float DeltaTime)
{
    for (AActor* Actor : AffectedPawns)
    {
        if (Actor && Actor->IsA(APawn::StaticClass()))
        {
            UPrimitiveComponent* PawnMesh = Cast<UPrimitiveComponent>(Actor->GetComponentByClass(UPrimitiveComponent::StaticClass()));

            if (PawnMesh && PawnMesh->IsSimulatingPhysics())
            {
                FVector CurrentVelocity = PawnMesh->GetPhysicsLinearVelocity();

                FVector AntiGravityVector = FVector(0, 0, AntiGravityForce * DeltaTime * PawnMesh->GetMass());

                FVector SinkingForceVector = FVector(0, 0, -SinkingForce * DeltaTime * PawnMesh->GetMass());

                FVector NewVelocity = FVector(CurrentVelocity.X * MovementResistance, CurrentVelocity.Y * MovementResistance, CurrentVelocity.Z);
                
                if (CurrentVelocity.Z > 0)
                {
                    FVector JumpDampening = FVector(0, 0, -CurrentVelocity.Z * JumpResistance);
                    PawnMesh->AddForce(JumpDampening * PawnMesh->GetMass());
                }

                PawnMesh->AddForce(AntiGravityVector + SinkingForceVector);
                PawnMesh->SetPhysicsLinearVelocity(NewVelocity, false);
            }
        }
    }
}