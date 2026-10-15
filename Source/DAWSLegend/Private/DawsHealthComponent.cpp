#include "DawsHealthComponent.h"
UDawsHealthComponent::UDawsHealthComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}
void UDawsHealthComponent::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}
void UDawsHealthComponent::ApplyDamage(float Amount)
{
    if (Amount <= 0.0f || CurrentHealth <= 0.0f) return;
    CurrentHealth = FMath::Clamp(CurrentHealth - Amount, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
    if (CurrentHealth <= 0.0f) OnDeath.Broadcast();
}
void UDawsHealthComponent::RestoreHealth(float Amount)
{
    if (Amount <= 0.0f || CurrentHealth <= 0.0f) return;
    CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
    OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}