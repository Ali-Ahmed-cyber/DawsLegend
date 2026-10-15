#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsInteractable.h"
#include "DawsMissionTrigger.generated.h"
class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class DAWSLEGEND_API ADawsMissionTrigger : public AActor, public IDawsInteractable
{
    GENERATED_BODY()
public:
    ADawsMissionTrigger();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UBoxComponent> TriggerBox;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TObjectPtr<UStaticMeshComponent> MarkerVisual;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission")
    FName MissionId = TEXT("chapter01_plaza");
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Mission")
    FText ObjectiveText = FText::FromString(TEXT("Reach the plaza and secure the area"));
    virtual FText GetInteractionPrompt_Implementation() const override;
    void RefreshMissionMarker();
    virtual void Interact_Implementation(AActor* InteractingActor) override;
protected:
    virtual void BeginPlay() override;
};