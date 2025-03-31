#include "BridgeManager.h"
#include "Pin.h"
#include "Beam.h"
#include "Ball.h"

ABridgeManager::ABridgeManager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABridgeManager::BeginPlay()
{
    Super::BeginPlay();
}

void ABridgeManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // for (APin* Pin : Pins)
    // {
    //     if (Pin)
    //     {
    //         Pin->CalculateForces(Beams, PlayerBall);
    //     }
    // }

    // for (APin* Pin : Pins)
    // {
    //     if (Pin)
    //     {
    //         Pin->SimulateStep(DeltaTime);
    //     }
    // }

    for (int32 i = Beams.Num() - 1; i >= 0; --i)
    {
        if (Beams[i] && Beams[i]->IsBroken())
        {
            Beams[i]->Destroy();
            Beams.RemoveAt(i);
        }
    }
}

void ABridgeManager::ClearBridge()
{
    for (ABeam* Beam : Beams)
    {
        if (Beam)
        {
            Beam->Destroy();
        }
    }
    Beams.Empty();

    for (APin* Pin : Pins)
    {
        if (Pin)
        {
            Pin->Destroy();
        }
    }
    Pins.Empty();
}