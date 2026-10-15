#include "DawsEnemyCharacter.h"
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
#include "Engine/GameInstance.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
ADawsEnemyCharacter::ADawsEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = 245.0f;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    HealthComponent = CreateDefaultSubobject<UDawsHealthComponent>(TEXT("EnemyHealth"));
    HealthComponent->MaxHealth = 100.0f;
    HealthComponent->CurrentHealth = 100.0f;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
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
}
void ADawsEnemyCharacter::ConfigureEnemy(FName NewEnemyId, bool bSpecialTarget)
{
    EnemyId = NewEnemyId;
    FLinearColor BodyColor(0.42f,0.08f,0.07f);
    float MaxHealth = 100.0f;
    RangedDamage = 8.0f;
    MeleeDamage = 12.0f;
    GetCharacterMovement()->MaxWalkSpeed = 245.0f;
    if (EnemyId == FName(TEXT("scout"))) { BodyColor = FLinearColor(0.38f,0.16f,0.08f); GetCharacterMovement()->MaxWalkSpeed = 310.0f; MaxHealth = 80.0f; RangedDamage = 6.0f; MeleeDamage = 9.0f; }
    else if (EnemyId == FName(TEXT("marksman"))) { BodyColor = FLinearColor(0.68f,0.42f,0.10f); GetCharacterMovement()->MaxWalkSpeed = 185.0f; MaxHealth = 90.0f; RangedDamage = 13.0f; MeleeDamage = 7.0f; }
    else if (EnemyId == FName(TEXT("guard"))) { BodyColor = FLinearColor(0.18f,0.27f,0.31f); MaxHealth = 130.0f; RangedDamage = 9.0f; MeleeDamage = 15.0f; }
    else if (EnemyId == FName(TEXT("enforcer")) || bSpecialTarget) { BodyColor = FLinearColor(0.24f,0.06f,0.18f); MaxHealth = 180.0f; GetCharacterMovement()->MaxWalkSpeed = 210.0f; RangedDamage = 12.0f; MeleeDamage = 22.0f; }
    if (HealthComponent) { HealthComponent->MaxHealth = MaxHealth; HealthComponent->CurrentHealth = MaxHealth; }
    if (BodyVisual) if (UMaterialInstanceDynamic* Mat = UMaterialInstanceDynamic::Create(BodyVisual->GetMaterial(0), this)) { Mat->SetVectorParameterValue(TEXT("Color"), BodyColor); BodyVisual->SetMaterial(0, Mat); }
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
    AttackCooldown -= DeltaSeconds;
    if (!GetGameInstance()) return;
    const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    if (!Missions || Missions->GetCurrentTaskType() != TEXT("defeat")) return;
    APawn* TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (!TargetPawn || !HealthComponent || !HealthComponent->IsAlive()) return;
    const float Distance = FVector::Dist2D(GetActorLocation(), TargetPawn->GetActorLocation());
    if (Distance > 1100.0f && Distance < 2400.0f) {
        AddMovementInput((TargetPawn->GetActorLocation() - GetActorLocation()).GetSafeNormal2D(), 0.8f);
    } else if (Distance > 170.0f && Distance <= 1100.0f && AttackCooldown <= 0.0f) {
        FHitResult ShotHit;
        FCollisionQueryParams ShotParams(SCENE_QUERY_STAT(DawsEnemyShot), true, this);
        const FVector ShotStart = GetActorLocation() + FVector(0,0,65);
        const FVector ShotEnd = TargetPawn->GetActorLocation() + FVector(0,0,45);
        if (GetWorld()->LineTraceSingleByChannel(ShotHit, ShotStart, ShotEnd, ECC_Visibility, ShotParams) && ShotHit.GetActor() == TargetPawn) {
            if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(TargetPawn)) {
                if (Player->HealthComponent) Player->HealthComponent->ApplyDamage(RangedDamage);
            } else if (ADawsVehiclePawn* Vehicle = Cast<ADawsVehiclePawn>(TargetPawn)) {
                Vehicle->ApplyVehicleDamage(RangedDamage * 1.5f);
            }
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
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->RegisterTaskProgress(TEXT("defeat"));
    }
    SetActorEnableCollision(false);
    SetLifeSpan(0.2f);
}
