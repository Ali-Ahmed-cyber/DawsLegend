#include "DawsEnemyCharacter.h"
#include "DawsHealthComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DawsPlayerCharacter.h"
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
    HeadVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeadVisual"));
    HeadVisual->SetupAttachment(GetRootComponent());
    if (Sphere.Succeeded()) HeadVisual->SetStaticMesh(Sphere.Object);
    HeadVisual->SetRelativeLocation(FVector(0,0,92));
    HeadVisual->SetRelativeScale3D(FVector(0.38f,0.38f,0.38f));
    HeadVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
    ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (!Player || !HealthComponent || !HealthComponent->IsAlive()) return;
    const float Distance = FVector::Dist2D(GetActorLocation(), Player->GetActorLocation());
    if (Distance > 1100.0f && Distance < 2400.0f) {
        AddMovementInput((Player->GetActorLocation() - GetActorLocation()).GetSafeNormal2D(), 0.8f);
    } else if (Distance > 170.0f && Distance <= 1100.0f && AttackCooldown <= 0.0f) {
        FHitResult ShotHit;
        FCollisionQueryParams ShotParams(SCENE_QUERY_STAT(DawsEnemyShot), true, this);
        const FVector ShotStart = GetActorLocation() + FVector(0,0,65);
        const FVector ShotEnd = Player->GetActorLocation() + FVector(0,0,45);
        if (GetWorld()->LineTraceSingleByChannel(ShotHit, ShotStart, ShotEnd, ECC_Visibility, ShotParams) && ShotHit.GetActor() == Player) {
            if (Player->HealthComponent) Player->HealthComponent->ApplyDamage(8.0f);
        }
        AttackCooldown = 1.8f;
    } else if (Distance <= 170.0f && AttackCooldown <= 0.0f) {
        if (Player->HealthComponent) Player->HealthComponent->ApplyDamage(12.0f);
        AttackCooldown = 1.25f;
    }
}
void ADawsEnemyCharacter::HandleDeath()
{
    SetActorEnableCollision(false);
    SetLifeSpan(0.2f);
}
