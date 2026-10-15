#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsMissionTrigger.generated.h"
class UBoxComponent;
UCLASS()
class DAWSLEGEND_API ADawsMissionTrigger : public AActor
{
    GENERATED_BODY()
public:
    ADawsMissionTrigger();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> TriggerBox;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission")
    FName MissionId = TEXT("chapter01_plaza");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission")
    FText ObjectiveText = FText::FromString(TEXT("Reach the plaza and secure the area"));
protected:
    virtual void BeginPlay() override;
private:
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                   bool bFromSweep, const FHitResult& SweepResult);
};