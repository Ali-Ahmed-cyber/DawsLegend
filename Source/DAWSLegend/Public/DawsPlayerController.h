#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DawsPlayerController.generated.h"
UCLASS()
class DAWSLEGEND_API ADawsPlayerController : public APlayerController
{
    GENERATED_BODY()
public:
    ADawsPlayerController();
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
private:
    bool bDawsPaused = false;
    void TogglePause();
};