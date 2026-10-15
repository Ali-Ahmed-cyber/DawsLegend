#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DawsCombatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams( FOnCombatAmmoChanged, int32, Magazine, int32, Reserve );
UCLASS(ClassGroup=(DAWS), meta=(BlueprintSpawnableComponent))
class DAWSLEGEND_API UDawsCombatComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    UDawsCombatComponent();
    UPROPERTY(BlueprintAssignable, Category="Combat")
    FOnCombatAmmoChanged OnAmmoChanged;
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
    int32 MagazineCapacity = 12;
    UPROPERTY(BlueprintReadOnly, Category="Combat")
    int32 MagazineAmmo = 12;
    UPROPERTY(BlueprintReadOnly, Category="Combat")
    int32 ReserveAmmo = 72;
    UFUNCTION(BlueprintCallable, Category="Combat")
    bool ConsumeShot();
    UFUNCTION(BlueprintCallable, Category="Combat")
    bool Reload();
    UFUNCTION(BlueprintCallable, Category="Combat")
    int32 GetTotalAmmo() const { return MagazineAmmo + ReserveAmmo; }
protected:
    virtual void BeginPlay() override;
};