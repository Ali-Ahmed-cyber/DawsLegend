#include "DawsPlayerCharacter.h"
#include "Camera/CameraComponent.h"
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
#include "TimerManager.h"
ADawsPlayerCharacter::ADawsPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = false;
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 620.0f);
    GetCharacterMovement()->JumpZVelocity = 520.0f;
    GetCharacterMovement()->AirControl = 0.32f;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    InitialSpawnLocation = GetActorLocation();
    if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &ADawsPlayerCharacter::HandlePlayerDeath);
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
}
void ADawsPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
    PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ADawsPlayerCharacter::StartSprint);
    PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ADawsPlayerCharacter::StopSprint);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADawsPlayerCharacter::FireWeapon);
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
void ADawsPlayerCharacter::TurnCamera(float Value) { AddControllerYawInput(Value); }
void ADawsPlayerCharacter::LookUp(float Value) { AddControllerPitchInput(Value); }
void ADawsPlayerCharacter::StartSprint() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; }
void ADawsPlayerCharacter::StopSprint() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; }
void ADawsPlayerCharacter::FireWeapon()
{
    if (!CombatComponent || !CombatComponent->ConsumeShot() || !FollowCamera) return;
    const FVector Start = FollowCamera->GetComponentLocation();
    const FVector End = Start + FollowCamera->GetForwardVector() * WeaponRange;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(DawsWeaponTrace), true, this);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params)) {
        if (AActor* HitActor = Hit.GetActor()) {
            if (UDawsHealthComponent* TargetHealth = HitActor->FindComponentByClass<UDawsHealthComponent>())
                TargetHealth->ApplyDamage(WeaponDamage);
        }
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
        Saves->SaveCheckpoint(Chapter, MissionId, this, Health);
    }
}
void ADawsPlayerCharacter::LoadCheckpoint()
{
    if (!GetGameInstance()) return;
    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
        int32 Chapter = 1;
        FString Mission;
        float SavedHealth = 100.0f;
        if (Saves->LoadCheckpoint(this, Chapter, Mission, SavedHealth)) {
            if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->RestoreCampaignAtMission(Mission);
            if (!HealthComponent) return;
            HealthComponent->CurrentHealth = FMath::Clamp(SavedHealth, 1.0f, HealthComponent->MaxHealth);
            HealthComponent->OnHealthChanged.Broadcast(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
        }
    }
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
    GetCharacterMovement()->DisableMovement();
    if (APlayerController* PC = Cast<APlayerController>(Controller)) DisableInput(PC);
    if (GetWorld()) GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ADawsPlayerCharacter::RespawnAtCheckpoint, 3.0f, false);
}
void ADawsPlayerCharacter::RespawnAtCheckpoint()
{
    bool bLoadedCheckpoint = false;
    if (GetGameInstance()) {
        if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) {
            int32 Chapter = 1;
            FString Mission;
            float SavedHealth = 100.0f;
            bLoadedCheckpoint = Saves->LoadCheckpoint(this, Chapter, Mission, SavedHealth);
            if (bLoadedCheckpoint) {
                if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->RestoreCampaignAtMission(Mission);
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
}
