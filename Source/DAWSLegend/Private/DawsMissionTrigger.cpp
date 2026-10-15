#include "DawsMissionTrigger.h"
#include "Components/BoxComponent.h"
#include "DawsMissionSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Character.h"
ADawsMissionTrigger::ADawsMissionTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MissionTrigger"));
    SetRootComponent(TriggerBox);
    TriggerBox->SetBoxExtent(FVector(220,220,150));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
}
void ADawsMissionTrigger::BeginPlay()
{
    Super::BeginPlay();
    TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADawsMissionTrigger::OnOverlap);
}
void ADawsMissionTrigger::OnOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    if (!Cast<ACharacter>(OtherActor) || !GetGameInstance()) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        TArray<FDawsMissionObjective> Objectives;
        FDawsMissionObjective Objective;
        Objective.Id = MissionId.ToString() + TEXT("_reach");
        Objective.Description = ObjectiveText;
        Objectives.Add(Objective);
        Missions->StartMission(MissionId.ToString(), ObjectiveText, Objectives);
        SetActorEnableCollision(false);
    }
}