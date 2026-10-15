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
#include "DawsPlayerCharacter.h"
#include "DawsSaveSubsystem.h"
#include "DawsHealthComponent.h"
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
    if (bMissionCompleted) { MarkerVisual->SetVisibility(false); SetActorEnableCollision(false); return; }
    bool bActive = false;
    if (GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            const FString CurrentId = Missions->GetCurrentMissionId();
            static const TCHAR* MissionOrder[] = {
                TEXT("chapter01_plaza"), TEXT("chapter01_supply"), TEXT("chapter01_rescue"),
                TEXT("chapter02_market"), TEXT("chapter02_clinic"), TEXT("chapter02_courier"),
                TEXT("chapter03_relay"), TEXT("chapter03_escort"), TEXT("chapter03_broadcast"),
                TEXT("chapter04_yard"), TEXT("chapter04_workers"), TEXT("chapter04_escape"),
                TEXT("chapter05_evidence"), TEXT("chapter05_depot"), TEXT("chapter05_finale")
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
            bActive = CurrentId == MissionId.ToString();
        }
    }
    MarkerVisual->SetVisibility(true);
    if (UMaterialInstanceDynamic* MarkerMat = UMaterialInstanceDynamic::Create(MarkerVisual->GetMaterial(0), this)) {
        MarkerMat->SetVectorParameterValue(TEXT("Color"), bActive ? FLinearColor(0.04f,0.95f,0.70f) : FLinearColor(0.18f,0.22f,0.23f));
        MarkerVisual->SetMaterial(0, MarkerMat);
    }
}
FText ADawsMissionTrigger::GetInteractionPrompt_Implementation() const
{
    if (!GetGameInstance()) return FText::GetEmpty();
    if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (Missions->GetCurrentMissionId() != MissionId.ToString()) return FText::GetEmpty();
        if (!Missions->CanCompleteCurrentMission()) return FText::Format(NSLOCTEXT("DAWS", "TaskIncompletePrompt", "Task progress: {0} / {1}"), FText::AsNumber(Missions->GetTaskProgress()), FText::AsNumber(Missions->GetRequiredTaskProgress()));
        const TArray<FDawsMissionObjective> Objectives = Missions->GetObjectives();
        const FText PromptText = Objectives.Num() > 0 ? Objectives[0].Description : ObjectiveText;
        return FText::Format(NSLOCTEXT("DAWS", "MissionPrompt", "Press E: {0}"), PromptText);
    }
    return FText::GetEmpty();
}
void ADawsMissionTrigger::Interact_Implementation(AActor* InteractingActor)
{
    if (!Cast<ADawsPlayerCharacter>(InteractingActor) || !GetGameInstance()) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (Missions->GetCurrentMissionId() != MissionId.ToString()) return;
        if (!Missions->CanCompleteCurrentMission()) return;
        Missions->CompleteObjective(MissionId.ToString() + TEXT("_reach"));
        bMissionCompleted = true;
        SetActorEnableCollision(false);
        MarkerVisual->SetVisibility(false);
        for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->RefreshMissionMarker();
        if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
            const FString ActiveMission = Missions->GetCurrentMissionId();
            const int32 Chapter = ActiveMission.StartsWith(TEXT("chapter")) ? FCString::Atoi(*ActiveMission.Mid(7,2)) : 5;
            ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(InteractingActor);
            const float Health = (Player && Player->HealthComponent) ? Player->HealthComponent->CurrentHealth : 100.0f;
            Saves->SaveCheckpoint(Chapter, ActiveMission, InteractingActor, Health);
        }
    }
}
