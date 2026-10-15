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
    LeftArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmVisual"));
    LeftArmVisual->SetupAttachment(InteractionBounds);
    if (Cylinder.Succeeded()) LeftArmVisual->SetStaticMesh(Cylinder.Object);
    LeftArmVisual->SetRelativeLocation(FVector(0,-24,45));
    LeftArmVisual->SetRelativeScale3D(FVector(0.10f,0.10f,0.34f));
    LeftArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmVisual"));
    RightArmVisual->SetupAttachment(InteractionBounds);
    if (Cylinder.Succeeded()) RightArmVisual->SetStaticMesh(Cylinder.Object);
    RightArmVisual->SetRelativeLocation(FVector(0,24,45));
    RightArmVisual->SetRelativeScale3D(FVector(0.10f,0.10f,0.34f));
    RightArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegVisual"));
    LeftLegVisual->SetupAttachment(InteractionBounds);
    if (Cylinder.Succeeded()) LeftLegVisual->SetStaticMesh(Cylinder.Object);
    LeftLegVisual->SetRelativeLocation(FVector(0,-10,9));
    LeftLegVisual->SetRelativeScale3D(FVector(0.12f,0.12f,0.34f));
    LeftLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegVisual"));
    RightLegVisual->SetupAttachment(InteractionBounds);
    if (Cylinder.Succeeded()) RightLegVisual->SetStaticMesh(Cylinder.Object);
    RightLegVisual->SetRelativeLocation(FVector(0,10,9));
    RightLegVisual->SetRelativeScale3D(FVector(0.12f,0.12f,0.34f));
    RightLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HairVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HairVisual"));
    HairVisual->SetupAttachment(HeadVisual);
    if (Sphere.Succeeded()) HairVisual->SetStaticMesh(Sphere.Object);
    HairVisual->SetRelativeLocation(FVector(-0.1f,0,36.0f));
    HairVisual->SetRelativeScale3D(FVector(0.95f,0.95f,0.32f));
    HairVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftEyeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftEyeVisual"));
    LeftEyeVisual->SetupAttachment(InteractionBounds);
    if (Sphere.Succeeded()) LeftEyeVisual->SetStaticMesh(Sphere.Object);
    LeftEyeVisual->SetRelativeLocation(FVector(12,-4,95));
    LeftEyeVisual->SetRelativeScale3D(FVector(0.026f,0.026f,0.026f));
    LeftEyeVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightEyeVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightEyeVisual"));
    RightEyeVisual->SetupAttachment(InteractionBounds);
    if (Sphere.Succeeded()) RightEyeVisual->SetStaticMesh(Sphere.Object);
    RightEyeVisual->SetRelativeLocation(FVector(12,4,95));
    RightEyeVisual->SetRelativeScale3D(FVector(0.026f,0.026f,0.026f));
    RightEyeVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    NoseVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoseVisual"));
    NoseVisual->SetupAttachment(InteractionBounds);
    if (Sphere.Succeeded()) NoseVisual->SetStaticMesh(Sphere.Object);
    NoseVisual->SetRelativeLocation(FVector(14,0,90));
    NoseVisual->SetRelativeScale3D(FVector(0.032f,0.032f,0.04f));
    NoseVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftEyeVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    RightEyeVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    NoseVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
}
void ADawsStoryNPC::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    VisualAnimationTime += DeltaSeconds;
    const float IdleBob = FMath::Abs(FMath::Sin(VisualAnimationTime * 2.0f)) * 1.4f;
    if (BodyVisual) BodyVisual->SetRelativeLocation(FVector(0,0,48.0f + IdleBob));
    if (HeadVisual) HeadVisual->SetRelativeLocation(FVector(0,0,91.0f + IdleBob));
    bool bEscorting = false;
    APawn* PlayerPawn = nullptr;
    if (NPCId == FName(TEXT("technician")) && GetGameInstance()) {
        const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
        if (Missions && Missions->GetCurrentMissionId() == TEXT("chapter03_escort")) {
            PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
            bEscorting = PlayerPawn != nullptr;
        }
    }
    float Swing = FMath::Sin(VisualAnimationTime * 1.8f) * 2.0f;
    if (bEscorting && PlayerPawn) {
        const FVector DesiredLocation = PlayerPawn->GetActorLocation() - PlayerPawn->GetActorForwardVector() * 190.0f;
        const float FollowDistance = FVector::Dist2D(GetActorLocation(), DesiredLocation);
        if (FollowDistance > 95.0f) Swing = FMath::Sin(VisualAnimationTime * 8.5f) * 17.0f;
        const FVector SmoothedLocation = FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaSeconds, 2.8f);
        SetActorLocation(SmoothedLocation, true);
        const FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
        if (!ToPlayer.IsNearlyZero()) SetActorRotation(FMath::RInterpTo(GetActorRotation(), ToPlayer.Rotation(), DeltaSeconds, 4.0f));
    }
    if (LeftArmVisual) LeftArmVisual->SetRelativeRotation(FRotator(Swing,0,0));
    if (RightArmVisual) RightArmVisual->SetRelativeRotation(FRotator(-Swing,0,0));
    if (LeftLegVisual) LeftLegVisual->SetRelativeRotation(FRotator(-Swing*0.8f,0,0));
    if (RightLegVisual) RightLegVisual->SetRelativeRotation(FRotator(Swing*0.8f,0,0));
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
    if (UMaterialInstanceDynamic* LeftArmMat = UMaterialInstanceDynamic::Create(LeftArmVisual->GetMaterial(0), this)) { LeftArmMat->SetVectorParameterValue(TEXT("Color"), ClothingColor); LeftArmVisual->SetMaterial(0, LeftArmMat); }
    if (UMaterialInstanceDynamic* RightArmMat = UMaterialInstanceDynamic::Create(RightArmVisual->GetMaterial(0), this)) { RightArmMat->SetVectorParameterValue(TEXT("Color"), ClothingColor); RightArmVisual->SetMaterial(0, RightArmMat); }
    if (UMaterialInstanceDynamic* LeftLegMat = UMaterialInstanceDynamic::Create(LeftLegVisual->GetMaterial(0), this)) { LeftLegMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.08f,0.09f,0.10f)); LeftLegVisual->SetMaterial(0, LeftLegMat); }
    if (UMaterialInstanceDynamic* RightLegMat = UMaterialInstanceDynamic::Create(RightLegVisual->GetMaterial(0), this)) { RightLegMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.08f,0.09f,0.10f)); RightLegVisual->SetMaterial(0, RightLegMat); }
    if (UMaterialInstanceDynamic* HairMat = UMaterialInstanceDynamic::Create(HairVisual->GetMaterial(0), this)) { HairMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.035f,0.028f,0.025f)); HairVisual->SetMaterial(0, HairMat); }
    if (UMaterialInstanceDynamic* EyeMat = UMaterialInstanceDynamic::Create(LeftEyeVisual->GetMaterial(0), this)) { EyeMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.025f,0.02f,0.018f)); LeftEyeVisual->SetMaterial(0, EyeMat); }
    if (UMaterialInstanceDynamic* EyeMat = UMaterialInstanceDynamic::Create(RightEyeVisual->GetMaterial(0), this)) { EyeMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.025f,0.02f,0.018f)); RightEyeVisual->SetMaterial(0, EyeMat); }
    if (UMaterialInstanceDynamic* NoseMat = UMaterialInstanceDynamic::Create(NoseVisual->GetMaterial(0), this)) { NoseMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.45f,0.29f,0.21f)); NoseVisual->SetMaterial(0, NoseMat); }
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
