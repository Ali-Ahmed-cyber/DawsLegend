#include "DawsPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "DawsPlayerCharacter.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
ADawsPickup::ADawsPickup()
{
    PrimaryActorTick.bCanEverTick = false;
    PickupCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollision"));
    SetRootComponent(PickupCollision);
    PickupCollision->InitSphereRadius(95.0f);
    PickupCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PickupCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    PickupCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    PickupVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupVisual"));
    PickupVisual->SetupAttachment(PickupCollision);
    PickupVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> Sphere(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (Sphere.Succeeded()) PickupVisual->SetStaticMesh(Sphere.Object);
    PickupVisual->SetRelativeScale3D(FVector(0.48f));
}
void ADawsPickup::BeginPlay()
{
    Super::BeginPlay();
    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ADawsPickup::HandleOverlap);
}
void ADawsPickup::HandleOverlap(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, int32, bool, const FHitResult&)
{
    ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(OtherActor);
    if (!Player) return;
    bool bConsumed = false;
    if (PickupType == EDawsPickupType::Health && Player->HealthComponent &&
        Player->HealthComponent->CurrentHealth < Player->HealthComponent->MaxHealth)
    {
        Player->HealthComponent->RestoreHealth(PickupAmount);
        bConsumed = true;
    }
    else if (PickupType == EDawsPickupType::Ammo && Player->CombatComponent)
    {
        Player->CombatComponent->ReserveAmmo += FMath::RoundToInt(PickupAmount);
        Player->CombatComponent->OnAmmoChanged.Broadcast(Player->CombatComponent->MagazineAmmo, Player->CombatComponent->ReserveAmmo);
        bConsumed = true;
    }
    if (bConsumed) Destroy();
}
