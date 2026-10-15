#include "DawsGameMode.h"
#include "DawsPlayerCharacter.h"
#include "DawsWorldBuilder.h"
#include "DawsMissionTrigger.h"
#include "DawsHUD.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
ADawsGameMode::ADawsGameMode()
{
    DefaultPawnClass = ADawsPlayerCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
    HUDClass = ADawsHUD::StaticClass();
}
void ADawsGameMode::StartPlay()
{
    Super::StartPlay();
    if (!GetWorld()) return;
    GetWorld()->SpawnActor<ADawsWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator);
    const FVector MissionPositions[3] = { FVector(0,850,90), FVector(850,0,90), FVector(-850,0,90) };
    const TCHAR* MissionNames[3] = { TEXT("chapter01_plaza"), TEXT("chapter01_supply"), TEXT("chapter01_rescue") };
    const TCHAR* MissionDescriptions[3] = {
        TEXT("Reach the plaza and secure the area"),
        TEXT("Recover the missing supplies"),
        TEXT("Find the survivor near the west street")
    };
    for (int32 Index = 0; Index < 3; ++Index)
    {
        ADawsMissionTrigger* Trigger = GetWorld()->SpawnActor<ADawsMissionTrigger>(MissionPositions[Index], FRotator::ZeroRotator);
        if (Trigger)
        {
            Trigger->MissionId = FName(MissionNames[Index]);
            Trigger->ObjectiveText = FText::FromString(MissionDescriptions[Index]);
        }
    }
}
