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
    bool HasStartedGame() const { return bHasStartedGame; }
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
private:
    bool bDawsPaused = false;
    bool bHasStartedGame = false;
    void TogglePause();
    void StartGame();
};