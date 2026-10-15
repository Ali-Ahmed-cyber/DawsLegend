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
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->StartCampaign();
    }
    const TCHAR* MissionNames[15] = {
        TEXT("chapter01_plaza"), TEXT("chapter01_supply"), TEXT("chapter01_rescue"),
        TEXT("chapter02_market"), TEXT("chapter02_clinic"), TEXT("chapter02_courier"),
        TEXT("chapter03_relay"), TEXT("chapter03_escort"), TEXT("chapter03_broadcast"),
        TEXT("chapter04_yard"), TEXT("chapter04_workers"), TEXT("chapter04_escape"),
        TEXT("chapter05_evidence"), TEXT("chapter05_depot"), TEXT("chapter05_finale")
    };
    const float MissionY[15] = { -4900,-4200,-3500,-2800,-2100,-1400,-700,0,700,1400,2100,2800,3500,4200,4900 };
    for (int32 Index = 0; Index < 15; ++Index) {
        ADawsMissionTrigger* Trigger = GetWorld()->SpawnActor<ADawsMissionTrigger>(FVector(0, MissionY[Index], 90), FRotator::ZeroRotator);
        if (Trigger) {
            Trigger->MissionId = FName(MissionNames[Index]);
            Trigger->ObjectiveText = FText::FromString(FString::Printf(TEXT("Continue campaign mission %d of 15"), Index+1));
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
