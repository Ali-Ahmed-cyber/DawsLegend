#include "DawsCombatComponent.h"
UDawsCombatComponent::UDawsCombatComponent() { PrimaryComponentTick.bCanEverTick = false; }
void UDawsCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    MagazineAmmo = FMath::Clamp(MagazineAmmo, 0, MagazineCapacity);
    OnAmmoChanged.Broadcast(MagazineAmmo, ReserveAmmo);
}
bool UDawsCombatComponent::ConsumeShot()
{
    if (MagazineAmmo <= 0) return false;
    --MagazineAmmo;
    OnAmmoChanged.Broadcast(MagazineAmmo, ReserveAmmo);
    return true;
}
bool UDawsCombatComponent::Reload()
{
    if (MagazineAmmo >= MagazineCapacity || ReserveAmmo <= 0) return false;
    const int32 Needed = MagazineCapacity - MagazineAmmo;
    const int32 Loaded = FMath::Min(Needed, ReserveAmmo);
    MagazineAmmo += Loaded;
    ReserveAmmo -= Loaded;
    OnAmmoChanged.Broadcast(MagazineAmmo, ReserveAmmo);
    return true;
}