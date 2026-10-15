#include "DawsGameMode.h"
#include "DawsPlayerCharacter.h"
#include "DawsPlayerController.h"
#include "DawsWorldBuilder.h"
#include "DawsMissionTrigger.h"
#include "DawsHUD.h"
#include "DawsEnemyCharacter.h"
#include "DawsPickup.h"
#include "DawsMissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
ADawsGameMode::ADawsGameMode()
{
    DefaultPawnClass = ADawsPlayerCharacter::StaticClass();
    PlayerControllerClass = ADawsPlayerController::StaticClass();
    HUDClass = ADawsHUD::StaticClass();
}
void ADawsGameMode::StartPlay()
{
    Super::StartPlay();
    if (!GetWorld()) return;
    GetWorld()->SpawnActor<ADawsWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator);
    if (GetGameInstance()) {
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            TArray<FDawsMissionObjective> InitialObjectives;
            FDawsMissionObjective InitialObjective;
            InitialObjective.Id = TEXT("chapter01_plaza_reach");
            InitialObjective.Description = FText::FromString(TEXT("Reach the plaza and secure the area"));
            InitialObjectives.Add(InitialObjective);
            Missions->StartMission(TEXT("chapter01_plaza"), FText::FromString(TEXT("The First Signal")), InitialObjectives);
        }
    }
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
    const FVector EnemyPositions[6] = { FVector(-450,450,100), FVector(450,-450,100), FVector(-650,-350,100), FVector(650,350,100), FVector(0,-750,100), FVector(750,750,100) };
    for (const FVector& EnemyPosition : EnemyPositions)
        GetWorld()->SpawnActor<ADawsEnemyCharacter>(EnemyPosition, FRotator::ZeroRotator);
    const FVector HealthLocations[3] = { FVector(-900,300,75), FVector(300,900,75), FVector(1200,-200,75) };
    for (const FVector& Location : HealthLocations) {
        ADawsPickup* Pickup = GetWorld()->SpawnActor<ADawsPickup>(Location, FRotator::ZeroRotator);
        if (Pickup) { Pickup->PickupType = EDawsPickupType::Health; Pickup->PickupAmount = 35.0f; }
    }
    const FVector AmmoLocations[3] = { FVector(600,600,75), FVector(-600,-600,75), FVector(1200,600,75) };
    for (const FVector& Location : AmmoLocations) {
        ADawsPickup* Pickup = GetWorld()->SpawnActor<ADawsPickup>(Location, FRotator::ZeroRotator);
        if (Pickup) { Pickup->PickupType = EDawsPickupType::Ammo; Pickup->PickupAmount = 24.0f; }
    }
}
