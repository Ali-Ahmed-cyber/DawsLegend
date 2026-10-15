#include "DawsMissionTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DawsMissionSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "DawsPlayerCharacter.h"
#include "DawsSaveSubsystem.h"
#include "DawsHealthComponent.h"
#include "DawsStoryNPC.h"
ADawsMissionTrigger::ADawsMissionTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MissionTrigger"));
    SetRootComponent(TriggerBox);
    TriggerBox->SetBoxExtent(FVector(220,220,150));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    TriggerBox->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
    MarkerVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissionMarker"));
    MarkerVisual->SetupAttachment(TriggerBox);
    MarkerVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MarkerVisual->SetRelativeLocation(FVector(0,0,20));
    MarkerVisual->SetRelativeScale3D(FVector(0.35f,0.35f,1.8f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Cylinder.Succeeded()) MarkerVisual->SetStaticMesh(Cylinder.Object);
    if (UMaterialInstanceDynamic* MarkerMat = UMaterialInstanceDynamic::Create(MarkerVisual->GetMaterial(0), this)) {
        MarkerMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.04f,0.85f,0.70f));
        MarkerVisual->SetMaterial(0, MarkerMat);
    }
}
void ADawsMissionTrigger::BeginPlay()
{
    Super::BeginPlay();
}
void ADawsMissionTrigger::RefreshMissionMarker()
{
    FString CurrentId;
    if (GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>())
            CurrentId = Missions->GetCurrentMissionId();
    }
    static const TCHAR* MissionOrder[] = {
        TEXT("chapter01_radio"), TEXT("chapter01_plaza"), TEXT("chapter01_workshop"), TEXT("chapter01_supplies"), TEXT("chapter01_driver"),
        TEXT("chapter02_market"), TEXT("chapter02_nurse"), TEXT("chapter02_minibus"), TEXT("chapter02_convoy"), TEXT("chapter02_sami"),
        TEXT("chapter03_tawfiq"), TEXT("chapter03_amjad"), TEXT("chapter03_clinic"), TEXT("chapter03_needle"), TEXT("chapter03_bashir"),
        TEXT("chapter04_relay"), TEXT("chapter04_escort"), TEXT("chapter04_broadcast"), TEXT("chapter04_ferry"), TEXT("chapter04_charity"),
        TEXT("chapter05_roadblocks"), TEXT("chapter05_workers"), TEXT("chapter05_tawfiq_debt"), TEXT("chapter05_ledger"), TEXT("chapter05_amjad_truth"),
        TEXT("chapter06_river_road"), TEXT("chapter06_witness_run"), TEXT("chapter06_depot"), TEXT("chapter06_muntasir"), TEXT("chapter06_dawn")
    };
    int32 MyIndex = INDEX_NONE, ActiveIndex = INDEX_NONE;
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(MissionOrder); ++Index) {
        if (MissionId.ToString() == MissionOrder[Index]) MyIndex = Index;
        if (CurrentId == MissionOrder[Index]) ActiveIndex = Index;
    }
    if (CurrentId == TEXT("campaign_complete") || (MyIndex != INDEX_NONE && ActiveIndex != INDEX_NONE && MyIndex < ActiveIndex)) {
        bMissionCompleted = true;
        MarkerVisual->SetVisibility(false);
        SetActorEnableCollision(false);
        return;
    }
    bMissionCompleted = false;
    SetActorEnableCollision(true);
    const bool bActive = CurrentId == MissionId.ToString();
    MarkerVisual->SetVisibility(true);
    if (UMaterialInstanceDynamic* MarkerMat = UMaterialInstanceDynamic::Create(MarkerVisual->GetMaterial(0), this)) {
        MarkerMat->SetVectorParameterValue(TEXT("Color"), bActive ? FLinearColor(0.04f,0.95f,0.70f) : FLinearColor(0.18f,0.22f,0.23f));
        MarkerVisual->SetMaterial(0, MarkerMat);
    }
}
bool ADawsMissionTrigger::IsEscortReady(const AActor* Player) const
{
    if (MissionId != FName(TEXT("chapter03_escort"))) return true;
    if (!Player || !GetWorld()) return false;
    for (TActorIterator<ADawsStoryNPC> It(GetWorld()); It; ++It) {
        if (It->GetNPCId() == FName(TEXT("technician"))) return FVector::Dist2D(Player->GetActorLocation(), It->GetActorLocation()) <= 650.0f;
    }
    return false;
}
FText ADawsMissionTrigger::GetInteractionPrompt_Implementation() const
{
    if (!GetGameInstance()) return FText::GetEmpty();
    if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (Missions->GetCurrentMissionId() != MissionId.ToString()) return FText::GetEmpty();
        if (!Missions->CanCompleteCurrentMission()) return FText::Format(NSLOCTEXT("DAWS", "TaskIncompletePrompt", "Task progress: {0} / {1}"), FText::AsNumber(Missions->GetTaskProgress()), FText::AsNumber(Missions->GetRequiredTaskProgress()));
        if (!IsEscortReady(UGameplayStatics::GetPlayerPawn(this, 0))) return FText::FromString(TEXT("Wait for the technician to catch up"));
        const TArray<FDawsMissionObjective> Objectives = Missions->GetObjectives();
        const FText PromptText = Objectives.Num() > 0 ? Objectives[0].Description : ObjectiveText;
        return FText::Format(NSLOCTEXT("DAWS", "MissionPrompt", "Press E: {0}"), PromptText);
    }
    return FText::GetEmpty();
}
void ADawsMissionTrigger::Interact_Implementation(AActor* InteractingActor)
{
    if (!Cast<ADawsPlayerCharacter>(InteractingActor) || !GetGameInstance()) return;
    if (!IsEscortReady(InteractingActor)) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (Missions->GetCurrentMissionId() != MissionId.ToString()) return;
        if (!Missions->CanCompleteCurrentMission()) return;
        if (!Missions->CompleteObjective(MissionId.ToString() + TEXT("_reach"))) return;
        bool bCheckpointSaved = false;
        bMissionCompleted = true;
        SetActorEnableCollision(false);
        MarkerVisual->SetVisibility(false);
        for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->RefreshMissionMarker();
        if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
            const FString ActiveMission = Missions->GetCurrentMissionId();
            const int32 Chapter = ActiveMission.StartsWith(TEXT("chapter")) ? FCString::Atoi(*ActiveMission.Mid(7,2)) : 5;
            ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(InteractingActor);
            const float Health = (Player && Player->HealthComponent) ? Player->HealthComponent->CurrentHealth : 100.0f;
            FVector NextCheckpointLocation = InteractingActor->GetActorLocation();
            bool bHasNextCheckpoint = false;
            if (ActiveMission != TEXT("campaign_complete")) {
                for (TActorIterator<ADawsMissionTrigger> NextIt(GetWorld()); NextIt; ++NextIt) {
                    if (NextIt->MissionId.ToString() == ActiveMission) {
                        NextCheckpointLocation = NextIt->GetActorLocation();
                        bHasNextCheckpoint = true;
                        break;
                    }
                }
            }
            bCheckpointSaved = Saves->SaveCheckpoint(Chapter, ActiveMission, InteractingActor, Health, NextCheckpointLocation, bHasNextCheckpoint);
        }
        if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(InteractingActor)) {
            if (Missions->GetCurrentMissionId() == TEXT("campaign_complete"))
                Player->SetStatusMessage(bCheckpointSaved ? TEXT("CAMPAIGN COMPLETE") : TEXT("CAMPAIGN COMPLETE — SAVE FAILED"), 3.5f);
            else
                Player->SetStatusMessage(bCheckpointSaved ? TEXT("MISSION COMPLETE — CHECKPOINT SAVED") : TEXT("MISSION COMPLETE — SAVE FAILED"), 3.5f);
        }
    }
}
