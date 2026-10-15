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
    bDawsPaused = true;
    SetPause(true);
}
void ADawsPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    FInputActionBinding& PauseBinding = InputComponent->BindAction("Pause", IE_Pressed, this, &ADawsPlayerController::TogglePause);
    PauseBinding.bExecuteWhenPaused = true;
    FInputActionBinding& StartBinding = InputComponent->BindAction("StartGame", IE_Pressed, this, &ADawsPlayerController::StartGame);
    StartBinding.bExecuteWhenPaused = true;
}
void ADawsPlayerController::TogglePause()
{
    if (!bHasStartedGame) { bDawsPaused = true; SetPause(true); return; }
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

void ADawsPlayerController::StartGame()
{
    if (bHasStartedGame) return;
    bHasStartedGame = true;
    bDawsPaused = false;
    SetPause(false);
    bShowMouseCursor = false;
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
}
