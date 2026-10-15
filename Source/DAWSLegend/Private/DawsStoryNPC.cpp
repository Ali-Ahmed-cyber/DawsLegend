#include "DawsStoryNPC.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "DawsPlayerCharacter.h"
#include "DawsEnemyCharacter.h"
#include "EngineUtils.h"
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
    bool bShouldFollow = false;
    bool bIsMainCompanion = false;
    APawn* PlayerPawn = nullptr;
    if (GetGameInstance()) {
        const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
        const FString ActiveMission = Missions ? Missions->GetCurrentMissionId() : FString();
        if (NPCId == FName(TEXT("the_broker"))) {
            const FVector EventLocation = ActiveMission == TEXT("chapter04_charity") ? FVector(-650,8290,0) : FVector(-650,12450,0);
            if (FVector::DistSquared2D(GetActorLocation(), EventLocation) > 2500.0f)
                SetActorLocation(EventLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (NPCId == FName(TEXT("hiba"))) {
            const FVector StudioLocation = ActiveMission == TEXT("chapter04_broadcast") ? FVector(-650,7470,0) : FVector(-650,300,0);
            if (FVector::DistSquared2D(GetActorLocation(), StudioLocation) > 2500.0f)
                SetActorLocation(StudioLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (NPCId == FName(TEXT("muntasir"))) {
            const FVector RecordOfficeLocation = ActiveMission == TEXT("chapter04_charity") ? FVector(650,8090,0) : FVector(650,11900,0);
            if (FVector::DistSquared2D(GetActorLocation(), RecordOfficeLocation) > 2500.0f)
                SetActorLocation(RecordOfficeLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (NPCId == FName(TEXT("bashir"))) {
            const FVector WorkshopLocation = ActiveMission == TEXT("chapter03_bashir") ? FVector(650,6240,0) : FVector(650,1900,0);
            if (FVector::DistSquared2D(GetActorLocation(), WorkshopLocation) > 2500.0f)
                SetActorLocation(WorkshopLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (NPCId == FName(TEXT("worker"))) {
            const FVector WorkerLocation = ActiveMission == TEXT("chapter05_workers") ? FVector(650,9110,0) : FVector(650,7500,0);
            if (FVector::DistSquared2D(GetActorLocation(), WorkerLocation) > 2500.0f)
                SetActorLocation(WorkerLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (NPCId == FName(TEXT("fawzi"))) {
            const FVector ClinicSupportLocation = ActiveMission == TEXT("chapter06_depot") ? FVector(-650,11570,0) : FVector(-650,3300,0);
            if (FVector::DistSquared2D(GetActorLocation(), ClinicSupportLocation) > 2500.0f)
                SetActorLocation(ClinicSupportLocation, false, nullptr, ETeleportType::TeleportPhysics);
        }
        if (ActiveMission == TEXT("chapter06_witness_run")) {
            if (NPCId == FName(TEXT("sami")) && FVector::DistSquared2D(GetActorLocation(), FVector(-650,11050,0)) > 2500.0f) SetActorLocation(FVector(-650,11050,0), false, nullptr, ETeleportType::TeleportPhysics);
            else if (NPCId == FName(TEXT("survivor")) && FVector::DistSquared2D(GetActorLocation(), FVector(-650,11250,0)) > 2500.0f) SetActorLocation(FVector(-650,11250,0), false, nullptr, ETeleportType::TeleportPhysics);
            else if (NPCId == FName(TEXT("captain_nadir")) && FVector::DistSquared2D(GetActorLocation(), FVector(650,11160,0)) > 2500.0f) SetActorLocation(FVector(650,11160,0), false, nullptr, ETeleportType::TeleportPhysics);
        } else {
            if (NPCId == FName(TEXT("sami")) && FVector::DistSquared2D(GetActorLocation(), FVector(-650,3900,0)) > 2500.0f) SetActorLocation(FVector(-650,3900,0), false, nullptr, ETeleportType::TeleportPhysics);
            else if (NPCId == FName(TEXT("survivor")) && FVector::DistSquared2D(GetActorLocation(), FVector(-650,1900,0)) > 2500.0f) SetActorLocation(FVector(-650,1900,0), false, nullptr, ETeleportType::TeleportPhysics);
            else if (NPCId == FName(TEXT("captain_nadir")) && FVector::DistSquared2D(GetActorLocation(), FVector(650,5400,0)) > 2500.0f) SetActorLocation(FVector(650,5400,0), false, nullptr, ETeleportType::TeleportPhysics);
        }
        bIsMainCompanion = NPCId == FName(TEXT("tawfiq")) || NPCId == FName(TEXT("amjad"));
        if (bIsMainCompanion && Missions && ActiveMission != TEXT("campaign_complete")) {
            PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
            bShouldFollow = PlayerPawn != nullptr;
        } else if (NPCId == FName(TEXT("technician")) && ActiveMission == TEXT("chapter04_escort")) {
            PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
            bShouldFollow = PlayerPawn != nullptr;
        }
    }
    float Swing = FMath::Sin(VisualAnimationTime * 1.8f) * 2.0f;
    if (bShouldFollow && PlayerPawn) {
        FVector DesiredLocation;
        if (bIsMainCompanion) {
            const float SideOffset = NPCId == FName(TEXT("tawfiq")) ? -115.0f : 115.0f;
            DesiredLocation = PlayerPawn->GetActorLocation() - PlayerPawn->GetActorForwardVector() * 210.0f
                + PlayerPawn->GetActorRightVector() * SideOffset;
        } else {
            DesiredLocation = PlayerPawn->GetActorLocation() - PlayerPawn->GetActorForwardVector() * 190.0f;
        }
        const float FollowDistance = FVector::Dist2D(GetActorLocation(), DesiredLocation);
        if (FollowDistance > 95.0f) Swing = FMath::Sin(VisualAnimationTime * 8.5f) * 17.0f;
        if (FollowDistance > 1600.0f) {
            SetActorLocation(DesiredLocation, false, nullptr, ETeleportType::TeleportPhysics);
        } else {
            const FVector SmoothedLocation = FMath::VInterpTo(GetActorLocation(), DesiredLocation, DeltaSeconds, bIsMainCompanion ? 3.4f : 2.8f);
            SetActorLocation(SmoothedLocation, true);
        }
        const FRotator DesiredRotation = bIsMainCompanion ? PlayerPawn->GetActorRotation() : (PlayerPawn->GetActorLocation() - GetActorLocation()).Rotation();
        SetActorRotation(FMath::RInterpTo(GetActorRotation(), DesiredRotation, DeltaSeconds, 4.0f));
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
    if (NPCId == FName(TEXT("the_broker")) && GetGameInstance()) {
        const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
        if (Missions && Missions->GetCurrentMissionId() == TEXT("chapter06_dawn")) {
            for (TActorIterator<ADawsEnemyCharacter> It(GetWorld()); It; ++It)
                if (It->IsSpecialTarget() && It->GetHealthRatio() > 0.0f)
                    return FText::FromString(TEXT("Defeat the Broker enforcer before confronting him"));
        }
    }
    return FText::Format(NSLOCTEXT("DAWS", "TalkPrompt", "Press E to talk to {0}"), DisplayName);
}
void ADawsStoryNPC::Interact_Implementation(AActor* InteractingActor)
{
    if (!Cast<ADawsPlayerCharacter>(InteractingActor) || !GetGameInstance()) return;
    if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
        if (NPCId == FName(TEXT("the_broker")) && Missions->GetCurrentMissionId() == TEXT("chapter06_dawn")) {
            for (TActorIterator<ADawsEnemyCharacter> It(GetWorld()); It; ++It) {
                if (It->IsSpecialTarget() && It->GetHealthRatio() > 0.0f) {
                    Missions->SetCurrentDialogue(FText::FromString(TEXT("The Broker: You cannot reach me while my enforcer stands. Take him down first.")));
                    return;
                }
            }
        }
        FText SpokenLine = DialogueLine;
        const FString ActiveMission = Missions->GetCurrentMissionId();
        if (NPCId == FName(TEXT("the_broker")) && ActiveMission == TEXT("chapter04_charity")) {
            SpokenLine = FText::FromString(TEXT("We feed this city when others only talk. Strange how every accusation comes before proof."));
        } else if (NPCId == FName(TEXT("technician")) && ActiveMission == TEXT("chapter04_escort")) {
            SpokenLine = FText::FromString(TEXT("Stay close. We have to reach the broadcast station before the signal window closes."));
        } else if (NPCId == FName(TEXT("tawfiq"))) {
            if (ActiveMission == TEXT("chapter01_plaza")) SpokenLine = FText::FromString(TEXT("I'm right here with you. Let's secure the plaza first, then follow the supply trail."));
            else if (ActiveMission == TEXT("chapter01_supplies")) SpokenLine = FText::FromString(TEXT("Check the ground around the convoy route. Three supply bundles are missing."));
            else if (ActiveMission == TEXT("chapter03_clinic")) SpokenLine = FText::FromString(TEXT("I'll watch the side street. Don't let the guards push you away from the clinic."));
            else if (ActiveMission == TEXT("chapter04_escort")) SpokenLine = FText::FromString(TEXT("I'll cover the rear. Keep the technician close and move toward the broadcast marker."));
            else if (ActiveMission == TEXT("chapter06_river_road")) SpokenLine = FText::FromString(TEXT("The river road is exposed. Use the truck if the route gets too hot."));
            else if (ActiveMission == TEXT("chapter06_depot")) SpokenLine = FText::FromString(TEXT("This is the last stronghold. Stay mobile and use the cover around the depot."));
            else if (ActiveMission == TEXT("chapter01_driver")) SpokenLine = FText::FromString(TEXT("That pickup is flying, ya zalameh! Watch the carts and don't let the guards box us in."));
            else if (ActiveMission == TEXT("chapter02_sami")) SpokenLine = FText::FromString(TEXT("Sami looks scared, not proud. Give him a chance to explain himself."));
            else if (ActiveMission == TEXT("chapter03_tawfiq")) SpokenLine = FText::FromString(TEXT("Okay, the shortcut was my idea. If we get out of this, I owe Bashir a proper apology."));
            else if (ActiveMission == TEXT("chapter03_bashir")) SpokenLine = FText::FromString(TEXT("Nobody leaves Bashir behind. He'd charge us for the rescue if he wasn't stuck in there."));
            else if (ActiveMission == TEXT("chapter04_charity")) SpokenLine = FText::FromString(TEXT("Look at all these cameras. The Broker wants the city to see his good side."));
            else if (ActiveMission == TEXT("chapter05_roadblocks")) SpokenLine = FText::FromString(TEXT("Fake roadblocks, real trouble. Idris, call us the safe turn!"));
            else if (ActiveMission == TEXT("chapter05_tawfiq_debt")) SpokenLine = FText::FromString(TEXT("I messed up, brother. I won't hide behind a joke this time."));
            else if (ActiveMission == TEXT("chapter06_witness_run")) SpokenLine = FText::FromString(TEXT("Getting the witnesses home matters more than looking brave."));
            else SpokenLine = FText::FromString(TEXT("We started this together. Keep checking the evidence and don't trust the Broker's promises."));
        } else if (NPCId == FName(TEXT("amjad"))) {
            if (ActiveMission == TEXT("chapter01_plaza")) SpokenLine = FText::FromString(TEXT("I checked the side streets. The convoy trail leads away from the main road."));
            else if (ActiveMission == TEXT("chapter01_supplies")) SpokenLine = FText::FromString(TEXT("Look for the supply bundles beside the marked route. We need all three."));
            else if (ActiveMission == TEXT("chapter02_market")) SpokenLine = FText::FromString(TEXT("The burned store is our best lead. Search for evidence before it disappears."));
            else if (ActiveMission == TEXT("chapter04_relay")) SpokenLine = FText::FromString(TEXT("The relay is quiet. Find the technician and get the signal moving again."));
            else if (ActiveMission == TEXT("chapter05_workers")) SpokenLine = FText::FromString(TEXT("Listen to the workers. Their account could connect the yard to the missing cargo."));
            else if (ActiveMission == TEXT("chapter06_dawn")) SpokenLine = FText::FromString(TEXT("Make the Broker answer for this. Then get the verified evidence to Hiba."));
            else if (ActiveMission == TEXT("chapter01_radio")) SpokenLine = FText::FromString(TEXT("Hiba's voice is steady, but that recording shook her. Let's not waste the lead."));
            else if (ActiveMission == TEXT("chapter01_driver")) SpokenLine = FText::FromString(TEXT("The token has a depot code. This driver didn't pick it up from the street."));
            else if (ActiveMission == TEXT("chapter02_convoy")) SpokenLine = FText::FromString(TEXT("The recorder time stamp proves the convoy changed direction before the blackout."));
            else if (ActiveMission == TEXT("chapter03_clinic")) SpokenLine = FText::FromString(TEXT("The attack began when the generator failed. The timing was planned."));
            else if (ActiveMission == TEXT("chapter03_bashir")) SpokenLine = FText::FromString(TEXT("The ledger's torn edge is clean. They removed a page carefully, not in a panic."));
            else if (ActiveMission == TEXT("chapter04_ferry")) SpokenLine = FText::FromString(TEXT("These parcel numbers match the depot ledger. The land deal and missing cargo are connected."));
            else if (ActiveMission == TEXT("chapter04_charity")) SpokenLine = FText::FromString(TEXT("That handoff looked rehearsed. Watch Muntasir's hands, not the cameras."));
            else if (ActiveMission == TEXT("chapter05_ledger")) SpokenLine = FText::FromString(TEXT("The carbon copy can prove who changed the order. Keep it flat and dry."));
            else if (ActiveMission == TEXT("chapter06_witness_run")) SpokenLine = FText::FromString(TEXT("The recordings are copies. The witnesses are people. We protect the people first."));
            else if (ActiveMission == TEXT("chapter06_depot")) SpokenLine = FText::FromString(TEXT("The neighborhood is bringing water and bandages. We're holding this depot together."));
            else SpokenLine = FText::FromString(TEXT("Keep your eyes open. The route changes, but the clues are all connected."));
        }
        Missions->SetCurrentDialogue(SpokenLine);
        Missions->RegisterNPCInteraction(NPCId.ToString());
        bHasTalked = true;
    }
}
