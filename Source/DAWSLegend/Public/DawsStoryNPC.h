#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DawsInteractable.h"
#include "DawsStoryNPC.generated.h"
class UBoxComponent;
class UStaticMeshComponent;
UCLASS()
class DAWSLEGEND_API ADawsStoryNPC : public AActor, public IDawsInteractable
{
    GENERATED_BODY()
public:
    ADawsStoryNPC();
    void ConfigureNPC(FName NewId, const FText& NewName, const FText& NewDialogue, FLinearColor NewClothingColor);
    void ResetForNewCampaign() { bHasTalked = false; }
    FName GetNPCId() const { return NPCId; }
    virtual FText GetInteractionPrompt_Implementation() const override;
    virtual void Interact_Implementation(AActor* InteractingActor) override;
    virtual void Tick(float DeltaSeconds) override;
private:
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UBoxComponent> InteractionBounds;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> BodyVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> HeadVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> AccessoryVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> LeftArmVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> RightArmVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> LeftLegVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> RightLegVisual;
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<UStaticMeshComponent> HairVisual;
    UPROPERTY(EditAnywhere, Category="Dialogue")
    FName NPCId = TEXT("hiba");
    UPROPERTY(EditAnywhere, Category="Dialogue")
    FText DisplayName = FText::FromString(TEXT("Hiba"));
    UPROPERTY(EditAnywhere, Category="Dialogue")
    FText DialogueLine;
    UPROPERTY()
    FLinearColor ClothingColor = FLinearColor(0.1f,0.25f,0.3f);
    bool bHasTalked = false;
    float VisualAnimationTime = 0.0f;
    void ApplyClothingColor();
};