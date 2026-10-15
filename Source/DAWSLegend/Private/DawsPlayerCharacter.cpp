#include "DawsPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
#include "DawsSaveSubsystem.h"
#include "DawsMissionSubsystem.h"
#include "DawsInteractable.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/PlayerController.h"
#include "DawsPlayerController.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "DawsMissionTrigger.h"
#include "DawsEnemyCharacter.h"
ADawsPlayerCharacter::ADawsPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 620.0f);
    GetCharacterMovement()->JumpZVelocity = 520.0f;
    GetCharacterMovement()->AirControl = 0.32f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 360.0f;
    CameraBoom->bUsePawnControlRotation = true;
    CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 68.0f));
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
    HealthComponent = CreateDefaultSubobject<UDawsHealthComponent>(TEXT("Health"));
    CombatComponent = CreateDefaultSubobject<UDawsCombatComponent>(TEXT("Combat"));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> CylinderMeshAsset(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    auto MakePart = [this](const TCHAR* Name, UStaticMesh* Mesh, FVector Location, FVector Scale, FLinearColor Tint) {
        UStaticMeshComponent* Part = CreateDefaultSubobject<UStaticMeshComponent>(Name);
        Part->SetupAttachment(RootComponent);
        if (Mesh) Part->SetStaticMesh(Mesh);
        Part->SetRelativeLocation(Location);
        Part->SetRelativeScale3D(Scale);
        Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        if (UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(Part->GetMaterial(0), this)) {
            Material->SetVectorParameterValue(TEXT("Color"), Tint);
            Part->SetMaterial(0, Material);
        }
        return Part;
    };
    BodyVisual = MakePart(TEXT("BodyVisual"), CubeMeshAsset.Object, FVector(0,0,48), FVector(0.42f,0.28f,0.55f), FLinearColor(0.88f,0.88f,0.82f));
    HeadVisual = MakePart(TEXT("HeadVisual"), SphereMeshAsset.Object, FVector(0,0,91), FVector(0.28f,0.28f,0.30f), FLinearColor(0.52f,0.34f,0.24f));
    LeftArmVisual = MakePart(TEXT("LeftArmVisual"), CylinderMeshAsset.Object, FVector(0,-28,48), FVector(0.13f,0.13f,0.42f), FLinearColor(0.86f,0.87f,0.82f));
    RightArmVisual = MakePart(TEXT("RightArmVisual"), CylinderMeshAsset.Object, FVector(0,28,48), FVector(0.13f,0.13f,0.42f), FLinearColor(0.86f,0.87f,0.82f));
    LeftLegVisual = MakePart(TEXT("LeftLegVisual"), CylinderMeshAsset.Object, FVector(0,-13,7), FVector(0.14f,0.14f,0.42f), FLinearColor(0.08f,0.09f,0.10f));
    RightLegVisual = MakePart(TEXT("RightLegVisual"), CylinderMeshAsset.Object, FVector(0,13,7), FVector(0.14f,0.14f,0.42f), FLinearColor(0.08f,0.09f,0.10f));
    WeaponVisual = MakePart(TEXT("WeaponVisual"), CubeMeshAsset.Object, FVector(34,25,49), FVector(0.42f,0.08f,0.09f), FLinearColor(0.05f,0.06f,0.07f));
    NeckVisual = MakePart(TEXT("NeckVisual"), CylinderMeshAsset.Object, FVector(0,0,76), FVector(0.12f,0.12f,0.12f), FLinearColor(0.48f,0.31f,0.22f));
    HairVisual = MakePart(TEXT("HairVisual"), SphereMeshAsset.Object, FVector(-1,0,102), FVector(0.28f,0.28f,0.12f), FLinearColor(0.035f,0.028f,0.025f));
    LeftEyeVisual = MakePart(TEXT("LeftEyeVisual"), SphereMeshAsset.Object, FVector(13,-5.0f,95), FVector(0.035f,0.035f,0.035f), FLinearColor(0.025f,0.02f,0.018f));
    RightEyeVisual = MakePart(TEXT("RightEyeVisual"), SphereMeshAsset.Object, FVector(13,5.0f,95), FVector(0.035f,0.035f,0.035f), FLinearColor(0.025f,0.02f,0.018f));
    NoseVisual = MakePart(TEXT("NoseVisual"), SphereMeshAsset.Object, FVector(15,0,90), FVector(0.045f,0.045f,0.055f), FLinearColor(0.48f,0.31f,0.22f));
    MouthVisual = MakePart(TEXT("MouthVisual"), CubeMeshAsset.Object, FVector(13.8f,0,85.5f), FVector(0.018f,0.055f,0.018f), FLinearColor(0.12f,0.035f,0.03f));
    BackpackVisual = MakePart(TEXT("BackpackVisual"), CubeMeshAsset.Object, FVector(-23,0,54), FVector(0.20f,0.22f,0.30f), FLinearColor(0.09f,0.13f,0.14f));
    BeltVisual = MakePart(TEXT("BeltVisual"), CubeMeshAsset.Object, FVector(0,0,27), FVector(0.40f,0.29f,0.07f), FLinearColor(0.08f,0.10f,0.12f));
    ChestCrestVisual = MakePart(TEXT("ChestCrestVisual"), CubeMeshAsset.Object, FVector(23,0,59), FVector(0.045f,0.075f,0.085f), FLinearColor(0.10f,0.22f,0.48f));
    LeftShoulderStripe = MakePart(TEXT("LeftShoulderStripe"), CubeMeshAsset.Object, FVector(1,-30,60), FVector(0.08f,0.035f,0.18f), FLinearColor(0.10f,0.22f,0.48f));
    RightShoulderStripe = MakePart(TEXT("RightShoulderStripe"), CubeMeshAsset.Object, FVector(1,30,60), FVector(0.08f,0.035f,0.18f), FLinearColor(0.10f,0.22f,0.48f));
    LeftBootVisual = MakePart(TEXT("LeftBootVisual"), CubeMeshAsset.Object, FVector(13,-13,-10), FVector(0.22f,0.16f,0.12f), FLinearColor(0.035f,0.04f,0.045f));
    RightBootVisual = MakePart(TEXT("RightBootVisual"), CubeMeshAsset.Object, FVector(13,13,-10), FVector(0.22f,0.16f,0.12f), FLinearColor(0.035f,0.04f,0.045f));

    NeckVisual->AttachToComponent(BodyVisual, FAttachmentTransformRules::KeepWorldTransform);
    BackpackVisual->AttachToComponent(BodyVisual, FAttachmentTransformRules::KeepWorldTransform);
    BeltVisual->AttachToComponent(BodyVisual, FAttachmentTransformRules::KeepWorldTransform);
    ChestCrestVisual->AttachToComponent(BodyVisual, FAttachmentTransformRules::KeepWorldTransform);
    HairVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    LeftEyeVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    RightEyeVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    NoseVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    MouthVisual->AttachToComponent(HeadVisual, FAttachmentTransformRules::KeepWorldTransform);
    LeftShoulderStripe->AttachToComponent(LeftArmVisual, FAttachmentTransformRules::KeepWorldTransform);
    RightShoulderStripe->AttachToComponent(RightArmVisual, FAttachmentTransformRules::KeepWorldTransform);
    WeaponVisual->AttachToComponent(RightArmVisual, FAttachmentTransformRules::KeepWorldTransform);
    LeftBootVisual->AttachToComponent(LeftLegVisual, FAttachmentTransformRules::KeepWorldTransform);
    RightBootVisual->AttachToComponent(RightLegVisual, FAttachmentTransformRules::KeepWorldTransform);

}
void ADawsPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    InitialSpawnLocation = GetActorLocation();
    if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &ADawsPlayerCharacter::HandlePlayerDeath);
}
void ADawsPlayerCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    VisualAnimationTime += DeltaSeconds;
    StatusMessageTime = FMath::Max(0.0f, StatusMessageTime - DeltaSeconds);
    const float Speed = GetVelocity().Size2D();
    const float Swing = Speed > 12.0f ? FMath::Sin(VisualAnimationTime * (Speed > SprintSpeed ? 13.0f : 9.0f)) * (Speed > SprintSpeed ? 28.0f : 18.0f) : 0.0f;
    if (LeftArmVisual) LeftArmVisual->SetRelativeRotation(FRotator(Swing,0,0));
    if (RightArmVisual) RightArmVisual->SetRelativeRotation(FRotator(-Swing,0,0));
    if (LeftLegVisual) LeftLegVisual->SetRelativeRotation(FRotator(-Swing,0,0));
    if (RightLegVisual) RightLegVisual->SetRelativeRotation(FRotator(Swing,0,0));
    if (BodyVisual) BodyVisual->SetRelativeLocation(FVector(0,0,48.0f + (Speed > 12.0f ? FMath::Abs(FMath::Sin(VisualAnimationTime * 9.0f)) * 2.0f : 0.0f)));
}
void ADawsPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ADawsPlayerCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ADawsPlayerCharacter::MoveRight);
    PlayerInputComponent->BindAxis("Turn", this, &ADawsPlayerCharacter::TurnCamera);
    PlayerInputComponent->BindAxis("LookUp", this, &ADawsPlayerCharacter::LookUp);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ADawsPlayerCharacter::StartJump);
    PlayerInputComponent->BindAction("Jump", IE_Released, this, &ADawsPlayerCharacter::StopJump);
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADawsPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADawsPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADawsPlayerCharacter::FireWeapon);
    PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ADawsPlayerCharacter::StartAim);
    PlayerInputComponent->BindAction("Aim", IE_Released, this, &ADawsPlayerCharacter::StopAim);
    PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ADawsPlayerCharacter::ReloadWeapon);
    PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ADawsPlayerCharacter::InteractWithWorld);
    PlayerInputComponent->BindAction("SaveGame", IE_Pressed, this, &ADawsPlayerCharacter::SaveCheckpoint);
    PlayerInputComponent->BindAction("LoadGame", IE_Pressed, this, &ADawsPlayerCharacter::LoadCheckpoint);
}
void ADawsPlayerCharacter::MoveForward(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X), Value);
}
void ADawsPlayerCharacter::MoveRight(float Value)
{
    if (!Controller || FMath::IsNearlyZero(Value)) return;
    const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);
    AddMovementInput(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y), Value);
}
void ADawsPlayerCharacter::TurnCamera(float Value)
{
    const ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(Controller);
    AddControllerYawInput(Value * (DawsPC ? DawsPC->GetMouseSensitivity() : 1.0f));
}
void ADawsPlayerCharacter::LookUp(float Value)
{
    const ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(Controller);
    AddControllerPitchInput(Value * (DawsPC ? DawsPC->GetMouseSensitivity() : 1.0f));
}
void ADawsPlayerCharacter::SetCameraFOV(float NewFOV)
{
    if (FollowCamera) FollowCamera->SetFieldOfView(FMath::Clamp(NewFOV - (bIsAiming ? 18.0f : 0.0f), 60.0f, 110.0f));
}
void ADawsPlayerCharacter::StartJump() { Jump(); }
void ADawsPlayerCharacter::StopJump() { StopJumping(); }
void ADawsPlayerCharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
void ADawsPlayerCharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }
void ADawsPlayerCharacter::StartAim()
{
    if (bIsAiming) return;
    bIsAiming = true;
    const ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(Controller);
    SetCameraFOV(DawsPC ? DawsPC->GetCameraFOV() : 90.0f);
}
void ADawsPlayerCharacter::StopAim()
{
    if (!bIsAiming) return;
    bIsAiming = false;
    const ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(Controller);
    SetCameraFOV(DawsPC ? DawsPC->GetCameraFOV() : 90.0f);
}
void ADawsPlayerCharacter::FireWeapon()
{
    if (!CombatComponent || !FollowCamera || !GetWorld()) return;
    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * WeaponRange;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(DawsWeaponTrace), true, this);
    const bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
    ADawsEnemyCharacter* EnemyTarget = bHit ? Cast<ADawsEnemyCharacter>(Hit.GetActor()) : nullptr;
    const UDawsMissionSubsystem* Missions = GetGameInstance() ? GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>() : nullptr;
    if (EnemyTarget && (!Missions || Missions->GetCurrentTaskType() != TEXT("defeat"))) return;
    if (!CombatComponent->ConsumeShot()) return;
    if (!bHit) return;
    if (EnemyTarget) {
        EnemyTarget->ReceiveWeaponDamage(WeaponDamage);
        return;
    }
    if (AActor* HitActor = Hit.GetActor()) {
        if (UDawsHealthComponent* TargetHealth = HitActor->FindComponentByClass<UDawsHealthComponent>())
            TargetHealth->ApplyDamage(WeaponDamage);
    }
}
void ADawsPlayerCharacter::ReloadWeapon()
{
    if (CombatComponent) CombatComponent->Reload();
}

void ADawsPlayerCharacter::SaveCheckpoint()
{
    if (!GetGameInstance()) return;
    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
        const float Health = HealthComponent ? HealthComponent->CurrentHealth : 100.0f;
        FString MissionId = TEXT("chapter01_plaza");
        int32 Chapter = 1;
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            MissionId = Missions->GetCurrentMissionId();
            if (MissionId.StartsWith(TEXT("chapter"))) Chapter = FCString::Atoi(*MissionId.Mid(7,2));
        }
        const bool bSaved = Saves->SaveCheckpoint(Chapter, MissionId, this, Health);
        SetStatusMessage(bSaved ? TEXT("CHECKPOINT SAVED") : TEXT("SAVE FAILED"));
    }
}
void ADawsPlayerCharacter::LoadCheckpoint()
{
    if (!GetGameInstance()) return;
    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
        int32 Chapter = 1;
        FString Mission;
        float SavedHealth = 100.0f;
        int32 SavedTaskProgress = 0;
        if (Saves->LoadCheckpoint(this, Chapter, Mission, SavedHealth, SavedTaskProgress)) {
            if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) { Missions->RestoreCampaignAtMission(Mission); Missions->RestoreTaskProgress(SavedTaskProgress); }
            for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->RefreshMissionMarker();
            if (!HealthComponent) return;
            HealthComponent->CurrentHealth = FMath::Clamp(SavedHealth, 1.0f, HealthComponent->MaxHealth);
            HealthComponent->OnHealthChanged.Broadcast(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
            SetStatusMessage(TEXT("CHECKPOINT LOADED"));
        } else {
            SetStatusMessage(TEXT("NO CHECKPOINT FOUND"));
        }
    }
}

FText ADawsPlayerCharacter::GetTargetInteractionPrompt() const
{
    if (!FollowCamera || !GetWorld()) return FText::GetEmpty();
    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * 650.0f;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(DawsPromptTrace), true, this);
    if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) return FText::GetEmpty();
    AActor* Target = Hit.GetActor();
    if (ADawsEnemyCharacter* Enemy = Cast<ADawsEnemyCharacter>(Target)) {
        const UDawsMissionSubsystem* Missions = GetGameInstance() ? GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>() : nullptr;
        if (!Missions || Missions->GetCurrentTaskType() != TEXT("defeat")) return FText::FromString(TEXT("Combat is locked until a hostile objective is active"));
        return FText::FromString(FString::Printf(TEXT("HOSTILE  |  %.0f%% HEALTH"), Enemy->GetHealthRatio() * 100.0f));
    }
    if (Target && Target->GetClass()->ImplementsInterface(UDawsInteractable::StaticClass())) return IDawsInteractable::Execute_GetInteractionPrompt(Target);
    return FText::GetEmpty();
}
void ADawsPlayerCharacter::InteractWithWorld()
{
    if (!FollowCamera || !GetWorld()) return;
    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * 650.0f;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(DawsInteractionTrace), true, this);
    if (!GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) return;
    AActor* Target = Hit.GetActor();
    if (Target && Target->GetClass()->ImplementsInterface(UDawsInteractable::StaticClass()))
        IDawsInteractable::Execute_Interact(Target, this);
}

void ADawsPlayerCharacter::HandlePlayerDeath()
{
    StopAim();
    GetCharacterMovement()->DisableMovement();
    if (APlayerController* PC = Cast<APlayerController>(Controller)) DisableInput(PC);
    if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ADawsPlayerCharacter::RespawnAtCheckpoint, 3.0f, false);
}
void ADawsPlayerCharacter::RespawnAtCheckpoint()
{
    StopAim();
    bool bLoadedCheckpoint = false;
    if (GetGameInstance()) {
        if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
            int32 Chapter = 1;
            FString Mission;
            float SavedHealth = 100.0f;
            int32 SavedTaskProgress = 0;
            bLoadedCheckpoint = Saves->LoadCheckpoint(this, Chapter, Mission, SavedHealth, SavedTaskProgress);
            if (bLoadedCheckpoint) {
                if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) { Missions->RestoreCampaignAtMission(Mission); Missions->RestoreTaskProgress(SavedTaskProgress); }
                for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->RefreshMissionMarker();
                if (HealthComponent) HealthComponent->CurrentHealth = FMath::Clamp(SavedHealth, 35.0f, HealthComponent->MaxHealth);
            }
        }
    }
    if (!bLoadedCheckpoint) {
        SetActorLocation(InitialSpawnLocation);
        if (HealthComponent) HealthComponent->CurrentHealth = HealthComponent->MaxHealth;
    }
    if (HealthComponent) HealthComponent->OnHealthChanged.Broadcast(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    if (APlayerController* PC = Cast<APlayerController>(Controller)) EnableInput(PC);
    SetStatusMessage(bLoadedCheckpoint ? TEXT("RESPAWNED AT CHECKPOINT") : TEXT("RESPAWNED AT START"));
}

void ADawsPlayerCharacter::ResetForNewCampaign()
{
    StopAim();
    if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
    SetActorLocation(InitialSpawnLocation, false, nullptr, ETeleportType::TeleportPhysics);
    SetActorRotation(FRotator::ZeroRotator);
    GetCharacterMovement()->SetMovementMode(MOVE_Walking);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    if (HealthComponent) {
        HealthComponent->CurrentHealth = HealthComponent->MaxHealth;
        HealthComponent->OnHealthChanged.Broadcast(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
    }
    if (CombatComponent) {
        CombatComponent->MagazineAmmo = CombatComponent->MagazineCapacity;
        CombatComponent->ReserveAmmo = 72;
        CombatComponent->OnAmmoChanged.Broadcast(CombatComponent->MagazineAmmo, CombatComponent->ReserveAmmo);
    }
    if (APlayerController* PC = Cast<APlayerController>(Controller)) EnableInput(PC);
    SetStatusMessage(TEXT("NEW CAMPAIGN STARTED"));
}
