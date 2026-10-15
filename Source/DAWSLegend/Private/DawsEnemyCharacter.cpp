#include "DawsEnemyCharacter.h"
#include "AIController.h"
#include "DawsHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DawsPlayerCharacter.h"
#include "DawsVehiclePawn.h"
#include "DawsMissionSubsystem.h"
#include "DawsPickup.h"
#include "DawsBulletTracer.h"
#include "DawsSaveSubsystem.h"
#include "Engine/GameInstance.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
ADawsEnemyCharacter::ADawsEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    AIControllerClass = AAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    GetCharacterMovement()->MaxWalkSpeed = 245.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    HealthComponent = CreateDefaultSubobject<UDawsHealthComponent>(TEXT("EnemyHealth"));
    HealthComponent->MaxHealth = 100.0f;
    HealthComponent->CurrentHealth = 100.0f;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
    BodyVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyVisual"));
    BodyVisual->SetupAttachment(GetRootComponent());
    if (Cube.Succeeded()) BodyVisual->SetStaticMesh(Cube.Object);
    BodyVisual->SetRelativeLocation(FVector(0,0,5));
    BodyVisual->SetRelativeScale3D(FVector(0.65f,0.45f,1.0f));
    BodyVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (UMaterialInstanceDynamic* BodyMat = UMaterialInstanceDynamic::Create(BodyVisual->GetMaterial(0), this)) { BodyMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.42f,0.08f,0.07f)); BodyVisual->SetMaterial(0, BodyMat); }
    HeadVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadVisual"));
    HeadVisual->SetupAttachment(GetRootComponent());
    if (Sphere.Succeeded()) HeadVisual->SetStaticMesh(Sphere.Object);
    HeadVisual->SetRelativeLocation(FVector(0,0,92));
    HeadVisual->SetRelativeScale3D(FVector(0.38f,0.38f,0.38f));
    HeadVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if (UMaterialInstanceDynamic* HeadMat = UMaterialInstanceDynamic::Create(HeadVisual->GetMaterial(0), this)) { HeadMat->SetVectorParameterValue(TEXT("Color"), FLinearColor(0.18f,0.12f,0.10f)); HeadVisual->SetMaterial(0, HeadMat); }
    LeftArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftArmVisual"));
    LeftArmVisual->SetupAttachment(GetRootComponent());
    if (Cube.Succeeded()) LeftArmVisual->SetStaticMesh(Cube.Object);
    LeftArmVisual->SetRelativeLocation(FVector(0,-42,45));
    LeftArmVisual->SetRelativeScale3D(FVector(0.20f,0.16f,0.48f));
    LeftArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightArmVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightArmVisual"));
    RightArmVisual->SetupAttachment(GetRootComponent());
    if (Cube.Succeeded()) RightArmVisual->SetStaticMesh(Cube.Object);
    RightArmVisual->SetRelativeLocation(FVector(0,42,45));
    RightArmVisual->SetRelativeScale3D(FVector(0.20f,0.16f,0.48f));
    RightArmVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    LeftLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftLegVisual"));
    LeftLegVisual->SetupAttachment(GetRootComponent());
    if (Cube.Succeeded()) LeftLegVisual->SetStaticMesh(Cube.Object);
    LeftLegVisual->SetRelativeLocation(FVector(0,-18,8));
    LeftLegVisual->SetRelativeScale3D(FVector(0.24f,0.18f,0.40f));
    LeftLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    RightLegVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightLegVisual"));
    RightLegVisual->SetupAttachment(GetRootComponent());
    if (Cube.Succeeded()) RightLegVisual->SetStaticMesh(Cube.Object);
    RightLegVisual->SetRelativeLocation(FVector(0,18,8));
    RightLegVisual->SetRelativeScale3D(FVector(0.24f,0.18f,0.40f));
    RightLegVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    HelmetVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelmetVisual"));
    HelmetVisual->SetupAttachment(GetRootComponent());
    if (Cylinder.Succeeded()) HelmetVisual->SetStaticMesh(Cylinder.Object);
    HelmetVisual->SetRelativeLocation(FVector(0,0,111));
    HelmetVisual->SetRelativeScale3D(FVector(0.34f,0.34f,0.12f));
    HelmetVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    WeaponVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponVisual"));
    WeaponVisual->SetupAttachment(RightArmVisual);
    if (Cube.Succeeded()) WeaponVisual->SetStaticMesh(Cube.Object);
    WeaponVisual->SetRelativeLocation(FVector(22,0,0));
    WeaponVisual->SetRelativeScale3D(FVector(0.42f,0.07f,0.07f));
    WeaponVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
float ADawsEnemyCharacter::GetHealthRatio() const
{
    return HealthComponent && HealthComponent->MaxHealth > 0.0f ? HealthComponent->CurrentHealth / HealthComponent->MaxHealth : 0.0f;
}
void ADawsEnemyCharacter::ConfigureEnemy(FName NewEnemyId, bool bSpecialTarget)
{
    EnemyId = NewEnemyId;
    bIsSpecialTarget = bSpecialTarget;
    SetActorScale3D(bSpecialTarget ? FVector(1.22f) : FVector(1.0f));
    FLinearColor BodyColor(0.42f,0.08f,0.07f);
    float MaxHealth = 100.0f;
    RangedDamage = 8.0f;
    MeleeDamage = 12.0f;
    AttackRange = 800.0f;
    bPreferMelee = false;
    GetCharacterMovement()->MaxWalkSpeed = 245.0f;
    if (EnemyId == FName(TEXT("scout"))) { BodyColor = FLinearColor(0.38f,0.16f,0.08f); GetCharacterMovement()->MaxWalkSpeed = 310.0f; MaxHealth = 80.0f; RangedDamage = 6.0f; MeleeDamage = 9.0f; AttackRange = 300.0f; bPreferMelee = true; }
    else if (EnemyId == FName(TEXT("marksman"))) { BodyColor = FLinearColor(0.68f,0.42f,0.10f); GetCharacterMovement()->MaxWalkSpeed = 185.0f; MaxHealth = 90.0f; RangedDamage = 13.0f; MeleeDamage = 7.0f; AttackRange = 1150.0f; }
    else if (EnemyId == FName(TEXT("guard"))) { BodyColor = FLinearColor(0.18f,0.27f,0.31f); MaxHealth = 130.0f; RangedDamage = 9.0f; MeleeDamage = 15.0f; AttackRange = 650.0f; }
    else if (EnemyId == FName(TEXT("enforcer")) || bSpecialTarget) { BodyColor = FLinearColor(0.24f,0.06f,0.18f); MaxHealth = 180.0f; GetCharacterMovement()->MaxWalkSpeed = 210.0f; RangedDamage = 12.0f; MeleeDamage = 22.0f; AttackRange = 280.0f; bPreferMelee = true; }
    if (HealthComponent) { HealthComponent->MaxHealth = MaxHealth; HealthComponent->CurrentHealth = MaxHealth; }
    if (BodyVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BodyVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), BodyColor); BodyVisual->SetMaterial(0, Mat); }
    if (LeftArmVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(LeftArmVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), BodyColor); LeftArmVisual->SetMaterial(0, Mat); }
    if (RightArmVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(RightArmVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), BodyColor); RightArmVisual->SetMaterial(0, Mat); }
    if (HelmetVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(HelmetVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), bSpecialTarget ? FLinearColor(0.05f,0.04f,0.06f) : BodyColor*0.55f); HelmetVisual->SetMaterial(0, Mat); }
    if (WeaponVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(WeaponVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), bSpecialTarget ? FLinearColor(0.30f,0.025f,0.035f) : FLinearColor(0.035f,0.04f,0.045f)); WeaponVisual->SetMaterial(0, Mat); }
    if (HeadVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(HeadVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), bSpecialTarget ? FLinearColor(0.08f,0.05f,0.08f) : FLinearColor(0.18f,0.12f,0.10f)); HeadVisual->SetMaterial(0, Mat); }
}
void ADawsEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    if (HealthComponent) HealthComponent->OnDeath.AddDynamic(this, &ADawsEnemyCharacter::HandleDeath);
}
void ADawsEnemyCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    VisualAnimationTime += DeltaSeconds;
    const float Swing = GetVelocity().Size2D() > 15.0f ? FMath::Sin(VisualAnimationTime * 9.0f) * 20.0f : FMath::Sin(VisualAnimationTime * 1.7f) * 2.0f;
    if (LeftArmVisual) LeftArmVisual->SetRelativeRotation(FRotator(Swing,0,0));
    if (RightArmVisual) RightArmVisual->SetRelativeRotation(FRotator(-Swing,0,0));
    if (LeftLegVisual) LeftLegVisual->SetRelativeRotation(FRotator(-Swing*0.8f,0,0));
    if (RightLegVisual) RightLegVisual->SetRelativeRotation(FRotator(Swing*0.8f,0,0));
    AttackCooldown -= DeltaSeconds;
    if (!GetGameInstance()) return;
    const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    const bool bFinalConfrontation = Missions && Missions->GetCurrentMissionId() == TEXT("chapter06_dawn") && bIsSpecialTarget;
    if (!Missions || ((!bFinalConfrontation) && (Missions->GetCurrentTaskType() != TEXT("defeat") || Missions->CanCompleteCurrentMission()))) return;
    APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!TargetPawn || !HealthComponent || !HealthComponent->IsAlive()) return;
    const float Distance = FVector::Dist2D(GetActorLocation(), TargetPawn->GetActorLocation());
    if (Distance > 170.0f && Distance < 2400.0f && (bPreferMelee || Distance > AttackRange)) {
        FVector MoveDirection = (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal2D();
        FHitResult ObstacleHit;
        FCollisionQueryParams MoveParams(SCENE_QUERY_STAT(DawsEnemyAvoidance), true, this);
        const FVector MoveStart = GetActorLocation() + FVector(0,0,45);
        const FVector MoveEnd = TargetPawn->GetActorLocation() + FVector(0,0,45);
        if (GetWorld()->LineTraceSingleByChannel(ObstacleHit, MoveStart, MoveEnd, ECC_Visibility, MoveParams) && ObstacleHit.GetActor() != TargetPawn) {
            FVector SideStep = FVector::CrossProduct(FVector::UpVector, MoveDirection).GetSafeNormal();
            if ((GetUniqueID() % 2) == 0) SideStep *= -1.0f;
            MoveDirection = (MoveDirection + SideStep * 0.85f).GetSafeNormal2D();
        }
        AddMovementInput(MoveDirection, 0.8f);
    } else if (Distance > 170.0f && Distance <= AttackRange && AttackCooldown <= 0.0f) {
        FHitResult ShotHit;
        FCollisionQueryParams ShotParams(SCENE_QUERY_STAT(DawsEnemyShot), true, this);
        const FVector ShotStart = GetActorLocation() + FVector(0,0,65);
        const FVector ShotEnd = TargetPawn->GetActorLocation() + FVector(0,0,45);
        const bool bClearShot = GetWorld()->LineTraceSingleByChannel(ShotHit, ShotStart, ShotEnd, ECC_Visibility, ShotParams) && ShotHit.GetActor() == TargetPawn;
        if (bClearShot) {
            if (ADawsBulletTracer* Tracer = GetWorld()->SpawnActor<ADawsBulletTracer>(ShotStart, (ShotEnd - ShotStart).Rotation()))
                Tracer->InitializeTracer(ShotStart, ShotEnd, FLinearColor(1.0f,0.24f,0.12f), 12500.0f);
            if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(TargetPawn)) {
                if (Player->HealthComponent) Player->HealthComponent->ApplyDamage(RangedDamage);
            } else if (ADawsVehiclePawn* Vehicle = Cast<ADawsVehiclePawn>(TargetPawn)) {
                Vehicle->ApplyVehicleDamage(RangedDamage * 1.5f);
            }
        } else {
            FVector FlankDirection = FVector::CrossProduct(FVector::UpVector, (TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal2D()).GetSafeNormal();
            if ((GetUniqueID() % 2) == 0) FlankDirection *= -1.0f;
            AddMovementInput(FlankDirection, 0.5f);
        }
        AttackCooldown = 1.8f;
    } else if (Distance <= 170.0f && AttackCooldown <= 0.0f) {
        if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(TargetPawn)) {
            if (Player->HealthComponent) Player->HealthComponent->ApplyDamage(MeleeDamage);
        } else if (ADawsVehiclePawn* Vehicle = Cast<ADawsVehiclePawn>(TargetPawn)) {
            Vehicle->ApplyVehicleDamage(MeleeDamage * 1.5f);
        }
        AttackCooldown = 1.25f;
    }
}
void ADawsEnemyCharacter::ReceiveWeaponDamage(float Amount)
{
    if (HealthComponent && HealthComponent->IsAlive()) HealthComponent->ApplyDamage(Amount);
}
void ADawsEnemyCharacter::HandleDeath()
{
    if (GetGameInstance()) {
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            if (Missions->GetCurrentTaskType() == TEXT("defeat")) {
                Missions->RegisterTaskProgress(TEXT("defeat"));
                if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))) {
                    const float PlayerHealth = Player->HealthComponent ? Player->HealthComponent->CurrentHealth : 100.0f;
                    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) Saves->SaveTaskProgress(Player, PlayerHealth);
                }
            }
        }
    }
    if (GetWorld()) {
        const int32 LootRoll = FMath::RandRange(0, 99);
        if (LootRoll < 25 || (LootRoll >= 25 && LootRoll < 35)) {
            ADawsPickup* Loot = GetWorld()->SpawnActor<ADawsPickup>(GetActorLocation() + FVector(0,0,20), FRotator::ZeroRotator);
            if (Loot) Loot->ConfigurePickup(LootRoll < 25 ? EDawsPickupType::Ammo : EDawsPickupType::Health, LootRoll < 25 ? 18.0f : 20.0f);
        }
    }
    SetActorEnableCollision(false);
    SetLifeSpan(0.2f);
}
