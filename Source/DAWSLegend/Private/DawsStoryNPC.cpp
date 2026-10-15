#include "DawsStoryNPC.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "DawsPlayerCharacter.h"
#include "DawsMissionSubsystem.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
ADawsStoryNPC::ADawsStoryNPC()
{
    PrimaryActorTick.bCanEverTick = true;
    InteractionBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBounds"));
    SetRootComponent(InteractionBounds);
    InteractionBounds->SetBoxExtent(FVector(65,65,105));
    InteractionBounds->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    InteractionBounds->SetCollisionResponseToAllChannels(ECR_Ignore);
    InteractionBounds->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    InteractionBounds->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    BodyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyVisual"));
    BodyVisual->SetupAttachment(InteractionBounds);
    if (Cube.Succeeded()) BodyVisual->SetStaticMesh(Cube.Object);
    BodyVisual->SetRelativeLocation(FVector(0,0,48));
    BodyVisual->SetRelativeScale3D(FVector(0.34f,0.28f,0.52f));
    BodyVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HeadVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadVisual"));
    HeadVisual->SetupAttachment(InteractionBounds);
    if (Sphere.Succeeded()) HeadVisual->SetStaticMesh(Sphere.Object);
    HeadVisual->SetRelativeLocation(FVector(0,0,91));
    HeadVisual->SetRelativeScale3D(FVector(0.25f,0.25f,0.27f));
    HeadVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AccessoryVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AccessoryVisual"));
    AccessoryVisual->SetupAttachment(InteractionBounds);
    if (Cylinder.Succeeded()) AccessoryVisual->SetStaticMesh(Cylinder.Object);
    AccessoryVisual->SetRelativeLocation(FVector(0,0,68));
    AccessoryVisual->SetRelativeScale3D(FVector(0.36f,0.36f,0.08f));
    AccessoryVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void ADawsStoryNPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bHasTalked || NPCId != FName(TEXT("technician")) || !GetGameInstance()) return;
    const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    if (!Missions || Missions->GetCurrentMissionId() != TEXT("chapter03_escort")) return;
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!PlayerPawn) return;
    const FVector DesiredLocation = PlayerPawn->GetActorLocation() - PlayerPawn->GetActorForwardVector() * 190.0f;
    const FVector SmoothedLocation = FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaSeconds, 2.8f);
    SetActorLocation(SmoothedLocation, true);
    const FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
    if (!ToPlayer.IsNearlyZero()) SetActorRotation(FMath::RInterpTo(GetActorRotation(), ToPlayer.Rotation(), DeltaSeconds, 4.0f));
}
void ADawsStoryNPC::ConfigureNPC(FName NewId, const FText& NewName, const FText& NewDialogue, FLinearColor NewClothingColor)
{
    NPCId = NewId;
    DisplayName = NewName;
    DialogueLine = NewDialogue;
    ClothingColor = NewClothingColor;
    ApplyClothingColor();
}
void ADawsStoryNPC::ApplyClothingColor()
{
    if (UMaterialInstanceDynamic* BodyMat = UMaterialInstanceDynamic::Create(BodyVisual->GetMaterial(0), this)) {
        BodyMat->SetVectorParameterValue(TEXT("Color"), ClothingColor);
        BodyVisual->SetMaterial(0, BodyMat);
    }
    if (UMaterialInstanceDynamic* AccessoryMat = UMaterialInstanceDynamic::Create(AccessoryVisual->GetMaterial(0), this)) {
        AccessoryMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.08f,0.09f,0.10f));
        AccessoryVisual->SetMaterial(0, AccessoryMat);
    }
}
FText ADawsStoryNPC::GetInteractionPrompt_Implementation() const
{
    return FText::Format(NSLOCTEXT("DAWS", "TalkPrompt", "Press E to talk to {0}"), DisplayName);
}
void ADawsStoryNPC::Interact_Implementation(AActor* InteractingActor)
{
    if (!Cast<ADawsPlayerCharacter>(InteractingActor) || !GetGameInstance()) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        FText SpokenLine = DialogueLine;
        if (NPCId == FName(TEXT("technician")) && Missions->GetCurrentMissionId() == TEXT("chapter03_escort"))
            SpokenLine = FText::FromString(TEXT("Stay close. We have to reach the broadcast station before the signal window closes."));
        Missions->SetCurrentDialogue(SpokenLine);
        Missions->RegisterNPCInteraction(NPCId.ToString());
        bHasTalked = true;
    }
}
