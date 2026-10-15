#include "DawsPickup.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "DawsPlayerCharacter.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
#include "DawsMissionSubsystem.h"
#include "DawsSaveSubsystem.h"
#include "Engine/GameInstance.h"
ADawsPickup::ADawsPickup()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
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
void ADawsPickup::ConfigurePickup(EDawsPickupType NewType, float NewAmount, const FString& NewPickupId)
{
    PickupType = NewType;
    PickupAmount = NewAmount;
    PickupId = NewPickupId;
    SetActorTickEnabled(PickupType == EDawsPickupType::Supplies || PickupType == EDawsPickupType::Evidence);
    if ((PickupType == EDawsPickupType::Supplies || PickupType == EDawsPickupType::Evidence) && !PickupId.IsEmpty()) {
        if (UGameInstance* GI = GetGameInstance()) {
            if (UDawsSaveSubsystem* Saves = GI->GetSubsystem<UDawsSaveSubsystem>(); Saves && Saves->IsPickupCollected(PickupId)) { Destroy(); return; }
        }
    }
    FLinearColor Tint(1.0f,0.55f,0.08f);
    if (PickupType == EDawsPickupType::Health) Tint = FLinearColor(0.08f,0.95f,0.28f);
    else if (PickupType == EDawsPickupType::Supplies) Tint = FLinearColor(0.95f,0.78f,0.18f);
    else if (PickupType == EDawsPickupType::Evidence) Tint = FLinearColor(0.62f,0.28f,0.95f);
    if (UMaterialInstanceDynamic* Material = UMaterialInstanceDynamic::Create(PickupVisual->GetMaterial(0), this)) {
        Material->SetVectorParameterValue(TEXT("Color"), Tint);
        PickupVisual->SetMaterial(0, Material);
    }
}
void ADawsPickup::BeginPlay()
{
    Super::BeginPlay();
    PickupCollision->OnComponentBeginOverlap.AddDynamic(this, &ADawsPickup::HandleOverlap);
}
void ADawsPickup::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if ((PickupType != EDawsPickupType::Supplies && PickupType != EDawsPickupType::Evidence) || !GetGameInstance()) return;
    const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    const FString RequiredType = PickupType == EDawsPickupType::Supplies ? TEXT("collect") : TEXT("evidence");
    if (!Missions || Missions->GetCurrentTaskType() != RequiredType) return;
    TArray<AActor*> OverlappingActors;
    PickupCollision->GetOverlappingActors(OverlappingActors, ADawsPlayerCharacter::StaticClass());
    for (AActor* OverlappingActor : OverlappingActors) {
        if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(OverlappingActor)) {
            const FHitResult EmptyHit;
            HandleOverlap(PickupCollision, Player, nullptr, 0, false, EmptyHit);
            break;
        }
    }
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
    else if ((PickupType == EDawsPickupType::Supplies || PickupType == EDawsPickupType::Evidence) && GetGameInstance()) {
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            const FString RequiredType = PickupType == EDawsPickupType::Supplies ? TEXT("collect") : TEXT("evidence");
            if (Missions->GetCurrentTaskType() == RequiredType) {
                Missions->RegisterTaskProgress(RequiredType);
                const float PlayerHealth = Player->HealthComponent ? Player->HealthComponent->CurrentHealth : 100.0f;
                if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) Saves->SaveTaskProgress(Player, PlayerHealth, PickupId);
                bConsumed = true;
            }
        }
    }
    if (bConsumed) {
        if (PickupType == EDawsPickupType::Health) Player->SetStatusMessage(TEXT("HEALTH RESTORED"));
        else if (PickupType == EDawsPickupType::Ammo) Player->SetStatusMessage(TEXT("AMMO COLLECTED"));
        else if (PickupType == EDawsPickupType::Supplies) Player->SetStatusMessage(TEXT("SUPPLIES RECOVERED"));
        else if (PickupType == EDawsPickupType::Evidence) Player->SetStatusMessage(TEXT("EVIDENCE RECOVERED"));
        Destroy();
    }
}
