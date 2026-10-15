#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DawsInteractable.generated.h"

UINTERFACE(BlueprintType)
class UDawsInteractable : public UInterface
{
    GENERATED_BODY()
};

class DAWSLEGEND_API IDawsInteractable
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    FText GetInteractionPrompt() const;
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void Interact(AActor* InteractingActor);
};