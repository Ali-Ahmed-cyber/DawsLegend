#include "DawsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "DawsPlayerCharacter.h"
#include "DawsSaveGame.h"
ADawsPlayerController::ADawsPlayerController()
{
    bShowMouseCursor = false;
}
void ADawsPlayerController::BeginPlay()
{
    Super::BeginPlay();
    LoadOptions();
    ApplyCameraFOV();
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
    FInputActionBinding& ContinueBinding = InputComponent->BindAction("ContinueGame", IE_Pressed, this, &ADawsPlayerController::ContinueGame);
    ContinueBinding.bExecuteWhenPaused = true;
    FInputActionBinding& OptionsBinding = InputComponent->BindAction("Options", IE_Pressed, this, &ADawsPlayerController::ToggleSettings);
    OptionsBinding.bExecuteWhenPaused = true;
    FInputActionBinding& UpBinding = InputComponent->BindAction("UIUp", IE_Pressed, this, &ADawsPlayerController::NavigateSettingsUp);
    UpBinding.bExecuteWhenPaused = true;
    FInputActionBinding& DownBinding = InputComponent->BindAction("UIDown", IE_Pressed, this, &ADawsPlayerController::NavigateSettingsDown);
    DownBinding.bExecuteWhenPaused = true;
    FInputActionBinding& LeftBinding = InputComponent->BindAction("UILeft", IE_Pressed, this, &ADawsPlayerController::AdjustSettingLeft);
    LeftBinding.bExecuteWhenPaused = true;
    FInputActionBinding& RightBinding = InputComponent->BindAction("UIRight", IE_Pressed, this, &ADawsPlayerController::AdjustSettingRight);
    RightBinding.bExecuteWhenPaused = true;
}
void ADawsPlayerController::LoadOptions()
{
    if (!UGameplayStatics::DoesSaveGameExist(TEXT("DAWSLegend_Options"), 0)) return;
    if (UDawsSaveGame* Options = Cast<UDawsSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("DAWSLegend_Options"), 0))) {
        MouseSensitivity = FMath::Clamp(Options->MouseSensitivity, 0.3f, 2.5f);
        CameraFOV = FMath::Clamp(Options->CameraFOV, 75.0f, 110.0f);
        bSubtitlesEnabled = Options->bSubtitlesEnabled;
    }
}
void ADawsPlayerController::SaveOptions()
{
    UDawsSaveGame* Options = Cast<UDawsSaveGame>(UGameplayStatics::CreateSaveGameObject(UDawsSaveGame::StaticClass()));
    if (!Options) return;
    Options->MouseSensitivity = MouseSensitivity;
    Options->CameraFOV = CameraFOV;
    Options->bSubtitlesEnabled = bSubtitlesEnabled;
    UGameplayStatics::SaveGameToSlot(Options, TEXT("DAWSLegend_Options"), 0);
}
void ADawsPlayerController::TogglePause()
{
    if (!bHasStartedGame) { bDawsPaused = true; SetPause(true); return; }
    bDawsPaused = !bDawsPaused;
    bSettingsMenuOpen = false;
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
    ApplyCameraFOV();
    bDawsPaused = false;
    SetPause(false);
    bShowMouseCursor = false;
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
}

void ADawsPlayerController::ToggleSettings()
{
    if (!bHasStartedGame) return;
    bSettingsMenuOpen = !bSettingsMenuOpen;
    if (bSettingsMenuOpen) {
        bDawsPaused = true;
        SetPause(true);
        bShowMouseCursor = true;
        FInputModeGameAndUI Mode;
        Mode.SetHideCursorDuringCapture(false);
        SetInputMode(Mode);
    } else {
        bShowMouseCursor = false;
        FInputModeGameOnly Mode;
        SetInputMode(Mode);
    }
}
void ADawsPlayerController::NavigateSettingsUp()
{
    if (bSettingsMenuOpen) SelectedSetting = (SelectedSetting + 2) % 3;
}
void ADawsPlayerController::NavigateSettingsDown()
{
    if (bSettingsMenuOpen) SelectedSetting = (SelectedSetting + 1) % 3;
}
void ADawsPlayerController::AdjustSettingLeft()
{
    if (!bSettingsMenuOpen) return;
    if (SelectedSetting == 0) MouseSensitivity = FMath::Clamp(MouseSensitivity - 0.1f, 0.3f, 2.5f);
    else if (SelectedSetting == 1) { CameraFOV = FMath::Clamp(CameraFOV - 5.0f, 75.0f, 110.0f); ApplyCameraFOV(); }
    else bSubtitlesEnabled = !bSubtitlesEnabled;
    SaveOptions();
}
void ADawsPlayerController::AdjustSettingRight()
{
    if (!bSettingsMenuOpen) return;
    if (SelectedSetting == 0) MouseSensitivity = FMath::Clamp(MouseSensitivity + 0.1f, 0.3f, 2.5f);
    else if (SelectedSetting == 1) { CameraFOV = FMath::Clamp(CameraFOV + 5.0f, 75.0f, 110.0f); ApplyCameraFOV(); }
    else bSubtitlesEnabled = !bSubtitlesEnabled;
    SaveOptions();
}
void ADawsPlayerController::ApplyCameraFOV()
{
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) Player->SetCameraFOV(CameraFOV);
}

void ADawsPlayerController::ContinueGame()
{
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) {
        if (UGameplayStatics::DoesSaveGameExist(TEXT("DAWSLegend_Main"), 0)) Player->LoadCheckpoint();
    }
    StartGame();
}
