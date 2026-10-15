#include "DawsMissionTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DawsMissionSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "DawsPlayerCharacter.h"
ADawsMissionTrigger::ADawsMissionTrigger()
{
    PrimaryActorTick.bCanEverTick = false;
    TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("MissionTrigger"));
    SetRootComponent(TriggerBox);
    TriggerBox->SetBoxExtent(FVector(220,220,150));
    TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    TriggerBox->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    MarkerVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MissionMarker"));
    MarkerVisual->SetupAttachment(TriggerBox);
    MarkerVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MarkerVisual->SetRelativeLocation(FVector(0,0,20));
    MarkerVisual->SetRelativeScale3D(FVector(0.35f,0.35f,1.8f));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    if (Cylinder.Succeeded()) MarkerVisual->SetStaticMesh(Cylinder.Object);
}
void ADawsMissionTrigger::BeginPlay()
{
    Super::BeginPlay();
}
FText ADawsMissionTrigger::GetInteractionPrompt_Implementation() const
{
    return FText::Format(NSLOCTEXT("DAWS", "MissionPrompt", "Press E: {0}"), ObjectiveText);
}
void ADawsMissionTrigger::Interact_Implementation(AActor* InteractingActor)
{
    if (!Cast<ADawsPlayerCharacter>(InteractingActor) || !GetGameInstance()) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (Missions->GetCurrentMissionId() != MissionId.ToString()) return;
        Missions->CompleteObjective(MissionId.ToString() + TEXT("_reach"));
        SetActorEnableCollision(false);
        MarkerVisual->SetVisibility(false);
    }
}
