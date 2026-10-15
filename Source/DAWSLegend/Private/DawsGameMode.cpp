#include "DawsGameMode.h"
#include "DawsPlayerCharacter.h"
#include "DawsPlayerController.h"
#include "DawsWorldBuilder.h"
#include "DawsMissionTrigger.h"
#include "DawsHUD.h"
#include "DawsEnemyCharacter.h"
#include "DawsPickup.h"
#include "DawsStoryNPC.h"
#include "DawsVehiclePawn.h"
#include "DawsMissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "EngineUtils.h"
ADawsGameMode::ADawsGameMode()
{
    DefaultPawnClass = ADawsPlayerCharacter::StaticClass();
    PlayerControllerClass = ADawsPlayerController::StaticClass();
    HUDClass = ADawsHUD::StaticClass();
}
void ADawsGameMode::StartPlay()
{
    Super::StartPlay();
    SpawnCampaignWorldActors();
}
void ADawsGameMode::SpawnCampaignWorldActors()
{if (!GetWorld()) return;
    GetWorld()->SpawnActor<ADawsWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator);
    if (GetGameInstance()) {
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->StartCampaign();
    }
    const TCHAR* MissionNames[30] = {
        TEXT("chapter01_radio"),
        TEXT("chapter01_plaza"),
        TEXT("chapter01_workshop"),
        TEXT("chapter01_supplies"),
        TEXT("chapter01_driver"),
        TEXT("chapter02_market"),
        TEXT("chapter02_nurse"),
        TEXT("chapter02_minibus"),
        TEXT("chapter02_convoy"),
        TEXT("chapter02_sami"),
        TEXT("chapter03_tawfiq"),
        TEXT("chapter03_amjad"),
        TEXT("chapter03_clinic"),
        TEXT("chapter03_needle"),
        TEXT("chapter03_bashir"),
        TEXT("chapter04_relay"),
        TEXT("chapter04_escort"),
        TEXT("chapter04_broadcast"),
        TEXT("chapter04_ferry"),
        TEXT("chapter04_charity"),
        TEXT("chapter05_roadblocks"),
        TEXT("chapter05_workers"),
        TEXT("chapter05_tawfiq_debt"),
        TEXT("chapter05_ledger"),
        TEXT("chapter05_amjad_truth"),
        TEXT("chapter06_river_road"),
        TEXT("chapter06_witness_run"),
        TEXT("chapter06_depot"),
        TEXT("chapter06_muntasir"),
        TEXT("chapter06_dawn")
    };
    for (int32 Index = 0; Index < 30; ++Index) {
        const float MissionY = 500.0f + Index * 410.0f;
        const FVector MarkerLocation = Index == 0 ? FVector(500,0,90) : FVector(0, MissionY, 90);
        ADawsMissionTrigger* Trigger = GetWorld()->SpawnActor<ADawsMissionTrigger>(MarkerLocation, FRotator::ZeroRotator);
        if (Trigger) {
            Trigger->MissionId = FName(MissionNames[Index]);
            Trigger->ObjectiveText = FText::FromString(FString::Printf(TEXT("Continue campaign mission %d of 30"), Index+1));
            Trigger->RefreshMissionMarker();
        }
    }
    const FVector EnemyPositions[28] = {
        FVector(-450,450,100), FVector(450,-450,100), FVector(-650,-350,100), FVector(650,350,100),
        FVector(450,1800,100), FVector(-450,2450,100), FVector(500,3150,100), FVector(-500,3850,100),
        FVector(450,4550,100), FVector(-450,5250,100), FVector(500,5950,100), FVector(-500,6650,100),
        FVector(450,7350,100), FVector(-450,8050,100), FVector(500,8750,100), FVector(-500,9450,100),
        FVector(0,1050,100), FVector(0,3850,100), FVector(0,6650,100), FVector(0,8750,100),
        FVector(350,3000,100), FVector(-350,3400,100), FVector(350,7900,100), FVector(-350,8300,100),
        FVector(350,9200,100), FVector(-350,9600,100), FVector(350,10000,100), FVector(-350,9800,100)
    };
    for (int32 EnemyIndex = 0; EnemyIndex < UE_ARRAY_COUNT(EnemyPositions); ++EnemyIndex) {
        ADawsEnemyCharacter* Enemy = GetWorld()->SpawnActor<ADawsEnemyCharacter>(EnemyPositions[EnemyIndex], FRotator::ZeroRotator);
        if (Enemy) {
            const TCHAR* EnemyTypes[] = { TEXT("scout"), TEXT("guard"), TEXT("marksman"), TEXT("enforcer") };
            Enemy->ConfigureEnemy(FName(EnemyTypes[EnemyIndex % 4]), EnemyIndex == UE_ARRAY_COUNT(EnemyPositions)-1);
        }
    }
    const FVector HealthLocations[8] = { FVector(-900,300,75), FVector(300,900,75), FVector(1200,-200,75), FVector(-900,2600,75), FVector(900,4000,75), FVector(-900,5400,75), FVector(900,6900,75), FVector(-900,8300,75) };
    for (const FVector& Location : HealthLocations) {
        ADawsPickup* Pickup = GetWorld()->SpawnActor<ADawsPickup>(Location, FRotator::ZeroRotator);
        if (Pickup) Pickup->ConfigurePickup(EDawsPickupType::Health, 35.0f);
    }
    const FVector AmmoLocations[8] = { FVector(600,600,75), FVector(-600,-600,75), FVector(1200,600,75), FVector(600,2100,75), FVector(-600,3500,75), FVector(600,4900,75), FVector(-600,6300,75), FVector(600,9100,75) };
    for (const FVector& Location : AmmoLocations) {
        ADawsPickup* Pickup = GetWorld()->SpawnActor<ADawsPickup>(Location, FRotator::ZeroRotator);
        if (Pickup) Pickup->ConfigurePickup(EDawsPickupType::Ammo, 24.0f);
    }

    const FVector TaskPickupLocations[9] = {
        FVector(-70,900,75), FVector(70,1150,75), FVector(-70,1400,75),
        FVector(70,2500,75), FVector(-70,2750,75),
        FVector(70,8050,75), FVector(-70,8350,75),
        FVector(70,8750,75), FVector(-70,9000,75)
    };
    const EDawsPickupType TaskPickupTypes[9] = {
        EDawsPickupType::Supplies, EDawsPickupType::Supplies, EDawsPickupType::Supplies,
        EDawsPickupType::Evidence, EDawsPickupType::Evidence,
        EDawsPickupType::Evidence, EDawsPickupType::Evidence,
        EDawsPickupType::Evidence, EDawsPickupType::Evidence
    };
    for (int32 Index = 0; Index < 9; ++Index) {
        ADawsPickup* Pickup = GetWorld()->SpawnActor<ADawsPickup>(TaskPickupLocations[Index], FRotator::ZeroRotator);
        if (Pickup) Pickup->ConfigurePickup(TaskPickupTypes[Index], 1.0f, FString::Printf(TEXT("task_pickup_%02d"), Index + 1));
    }


    struct FStoryNPCSetup { const TCHAR* Id; const TCHAR* Name; const TCHAR* Dialogue; FVector Location; FLinearColor Color; };
    const FStoryNPCSetup StoryNPCs[13] = {
        { TEXT("hiba"), TEXT("Hiba"), TEXT("The radio reports missing medical cargo. Secure the plaza, then follow the supply trail."), FVector(-650,300,0), FLinearColor(0.06f,0.28f,0.46f) },
        { TEXT("bashir"), TEXT("Bashir"), TEXT("These crates were emptied before the convoy arrived. Someone knew the route in advance."), FVector(650,1900,0), FLinearColor(0.45f,0.24f,0.08f) },
        { TEXT("fawzi"), TEXT("Fawzi"), TEXT("The clinic needs these supplies. I will move the families while you keep the road clear."), FVector(-650,3300,0), FLinearColor(0.12f,0.40f,0.20f) },
        { TEXT("captain_nadir"), TEXT("Captain Nadir"), TEXT("Keep the evidence intact. Rumours will not hold up when the Broker denies everything."), FVector(650,5400,0), FLinearColor(0.22f,0.24f,0.28f) },
        { TEXT("the_broker"), TEXT("The Broker"), TEXT("You think one ledger can stop me? The whole route belongs to people you have never met."), FVector(-650,10100,0), FLinearColor(0.35f,0.08f,0.12f) },
        { TEXT("survivor"), TEXT("The Survivor"), TEXT("I heard the convoy leader say the next shipment goes through the market after dark."), FVector(-650,1900,0), FLinearColor(0.40f,0.30f,0.18f) },
        { TEXT("technician"), TEXT("The Technician"), TEXT("The relay is jammed. Hold this street while I reconnect the transmitter."), FVector(-650,4700,0), FLinearColor(0.18f,0.32f,0.34f) },
        { TEXT("worker"), TEXT("The Worker"), TEXT("They locked us in after we refused to move the medical cargo."), FVector(650,7500,0), FLinearColor(0.30f,0.26f,0.18f) },
        { TEXT("tawfiq"), TEXT("Tawfiq"), TEXT("I am with you, brother. We will find who is stealing the medical supplies, but we do this together."), FVector(-220,-240,0), FLinearColor(0.92f,0.66f,0.12f) },
        { TEXT("amjad"), TEXT("Amjad"), TEXT("I checked the side streets. Someone has been watching the convoy since before dawn. Keep your head clear."), FVector(-220,240,0), FLinearColor(0.045f,0.05f,0.06f) },
        { TEXT("salwa"), TEXT("Salwa"), TEXT("The clinic seal is fake. I can prove the medicine was relabeled after leaving the depot."), FVector(650,3000,0), FLinearColor(0.18f,0.48f,0.42f) },
        { TEXT("sami"), TEXT("Sami the Courier"), TEXT("I drove because they threatened my family. I will tell you what I know if you keep them safe."), FVector(-650,3900,0), FLinearColor(0.30f,0.34f,0.42f) },
        { TEXT("muntasir"), TEXT("Muntasir"), TEXT("You have routes and rumors. I have the records. Without the missing page, you cannot prove a thing."), FVector(650,9800,0), FLinearColor(0.28f,0.22f,0.32f) }
    };
    for (const FStoryNPCSetup& Setup : StoryNPCs) {
        ADawsStoryNPC* NPC = GetWorld()->SpawnActor<ADawsStoryNPC>(Setup.Location, FRotator::ZeroRotator);
        if (NPC) NPC->ConfigureNPC(FName(Setup.Id), FText::FromString(Setup.Name), FText::FromString(Setup.Dialogue), Setup.Color);
    }


    const FVector VehicleLocations[2] = { FVector(650,1700,55), FVector(-650,6500,55) };
    for (const FVector& VehicleLocation : VehicleLocations)
        GetWorld()->SpawnActor<ADawsVehiclePawn>(VehicleLocation, FRotator::ZeroRotator);


}
void ADawsGameMode::RestartCampaignWorld()
{
    if (!GetWorld()) return;
    for (TActorIterator<ADawsWorldBuilder> It(GetWorld()); It; ++It) It->Destroy();
    for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->Destroy();
    for (TActorIterator<ADawsEnemyCharacter> It(GetWorld()); It; ++It) It->Destroy();
    for (TActorIterator<ADawsPickup> It(GetWorld()); It; ++It) It->Destroy();
    for (TActorIterator<ADawsStoryNPC> It(GetWorld()); It; ++It) It->Destroy();
    for (TActorIterator<ADawsVehiclePawn> It(GetWorld()); It; ++It) It->Destroy();
    SpawnCampaignWorldActors();
}
