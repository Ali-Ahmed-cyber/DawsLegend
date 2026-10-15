#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DawsHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDawsHealthChanged, float, CurrentHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDawsDeathEvent);

UCLASS(ClassGroup=(DAWS), meta=(BlueprintSpawnableComponent))
class DAWSLEGEND_API UDawsHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UDawsHealthComponent();
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Health")
    float MaxHealth = 100.0f;
    UPROPERTY(BlueprintReadOnly, Category="Health")
    float CurrentHealth = 100.0f;
    UPROPERTY(BlueprintAssignable, Category="Health")
    FDawsHealthChanged OnHealthChanged;
    UPROPERTY(BlueprintAssignable, Category="Health")
    FDawsDeathEvent OnDeath;
    UFUNCTION(BlueprintCallable, Category="Health")
    void ApplyDamage(float Amount);
    UFUNCTION(BlueprintCallable, Category="Health")
    void RestoreHealth(float Amount);
    UFUNCTION(BlueprintPure, Category="Health")
    bool IsAlive() const { return CurrentHealth > 0.0f; }
protected:
    virtual void BeginPlay() override;
};