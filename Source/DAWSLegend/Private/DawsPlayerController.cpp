#include "DawsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
ADawsPlayerController::ADawsPlayerController()
{
    bShowMouseCursor = false;
}
void ADawsPlayerController::BeginPlay()
{
    Super::BeginPlay();
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
}
void ADawsPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    FInputActionBinding& PauseBinding = InputComponent->BindAction("Pause", IE_Pressed, this, &ADawsPlayerController::TogglePause);
    PauseBinding.bExecuteWhenPaused = true;
}
void ADawsPlayerController::TogglePause()
{
    bDawsPaused = !bDawsPaused;
    SetPause(bDawsPaused);
    bShowMouseCursor = bDawsPaused;
    if (bDawsPaused) {
        FInputModeGameAndUI Mode;
        Mode.SetHideCursorDuringCapture(false);
        SetInputMode(Mode);
    } else {
        FInputModeGameOnly Mode;
        SetInputMode(Mode);
    }
}
