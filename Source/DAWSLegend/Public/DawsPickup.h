#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsPickup.generated.h"
class USphereComponent;
class UStaticMeshComponent;
UENUM(BlueprintType)
enum class EDawsPickupType : uint8
{
    Health UMETA(DisplayName="Medical Supply"),
    Ammo UMETA(DisplayName="Ammunition"),
    Supplies UMETA(DisplayName="Emergency Supplies"),
    Evidence UMETA(DisplayName="Evidence")
};
UCLASS()
class DAWSLEGEND_API ADawsPickup : public AActor
{
    GENERATED_BODY()
public:
    ADawsPickup();
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup")
    EDawsPickupType PickupType = EDawsPickupType::Health;
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pickup", meta=(ClampMin="1.0"))
    float PickupAmount = 35.0f;
    void ConfigurePickup(EDawsPickupType NewType, float NewAmount, const FString& NewPickupId = FString());
    const FString& GetPickupId() const { return PickupId; }
protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<USphereComponent> PickupCollision;
    UPROPERTY()
    FString PickupId;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> PickupVisual;
    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                       bool bFromSweep, const FHitResult& SweepResult);
};