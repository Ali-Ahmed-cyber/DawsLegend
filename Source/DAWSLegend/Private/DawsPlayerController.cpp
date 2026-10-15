#include "DawsPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "DawsGameMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/GameplayStatics.h"
#include "DawsPlayerCharacter.h"
#include "DawsVehiclePawn.h"
#include "DawsSaveGame.h"
#include "DawsSaveSubsystem.h"
#include "DawsMissionSubsystem.h"
#include "DawsMissionTrigger.h"
#include "DawsStoryNPC.h"
#include "Engine/GameInstance.h"
#include "EngineUtils.h"
ADawsPlayerController::ADawsPlayerController()
{
    bShowMouseCursor = false;
}
void ADawsPlayerController::BeginPlay()
{
    Super::BeginPlay();
    LoadOptions();
    ApplyCameraFOV();
    ApplyGraphicsQuality();
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
    FInputActionBinding& StartBinding = InputComponent->BindAction("StartGame", IE_Pressed, this, &ADawsPlayerController::StartNewCampaign);
    StartBinding.bExecuteWhenPaused = true;
    FInputActionBinding& ContinueBinding = InputComponent->BindAction("ContinueGame", IE_Pressed, this, &ADawsPlayerController::ContinueGame);
    FInputActionBinding& RestartBinding = InputComponent->BindAction("RestartCampaign", IE_Pressed, this, &ADawsPlayerController::RestartCampaign);
    RestartBinding.bExecuteWhenPaused = true;
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
        GraphicsQuality = FMath::Clamp(Options->GraphicsQuality, 0, 2);
    }
}
void ADawsPlayerController::SaveOptions()
{
    UDawsSaveGame* Options = Cast<UDawsSaveGame>(UGameplayStatics::CreateSaveGameObject(UDawsSaveGame::StaticClass()));
    if (!Options) return;
    Options->MouseSensitivity = MouseSensitivity;
    Options->CameraFOV = CameraFOV;
    Options->bSubtitlesEnabled = bSubtitlesEnabled;
    Options->GraphicsQuality = GraphicsQuality;
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
    if (bSettingsMenuOpen) SelectedSetting = (SelectedSetting + 3) % 4;
}
void ADawsPlayerController::NavigateSettingsDown()
{
    if (bSettingsMenuOpen) SelectedSetting = (SelectedSetting + 1) % 4;
}
void ADawsPlayerController::AdjustSettingLeft()
{
    if (!bSettingsMenuOpen) return;
    if (SelectedSetting == 0) MouseSensitivity = FMath::Clamp(MouseSensitivity - 0.1f, 0.3f, 2.5f);
    else if (SelectedSetting == 1) { CameraFOV = FMath::Clamp(CameraFOV - 5.0f, 75.0f, 110.0f); ApplyCameraFOV(); }
    else if (SelectedSetting == 2) bSubtitlesEnabled = !bSubtitlesEnabled;
    else { GraphicsQuality = FMath::Clamp(GraphicsQuality - 1, 0, 2); ApplyGraphicsQuality(); }
    SaveOptions();
}
void ADawsPlayerController::AdjustSettingRight()
{
    if (!bSettingsMenuOpen) return;
    if (SelectedSetting == 0) MouseSensitivity = FMath::Clamp(MouseSensitivity + 0.1f, 0.3f, 2.5f);
    else if (SelectedSetting == 1) { CameraFOV = FMath::Clamp(CameraFOV + 5.0f, 75.0f, 110.0f); ApplyCameraFOV(); }
    else if (SelectedSetting == 2) bSubtitlesEnabled = !bSubtitlesEnabled;
    else { GraphicsQuality = FMath::Clamp(GraphicsQuality + 1, 0, 2); ApplyGraphicsQuality(); }
    SaveOptions();
}
void ADawsPlayerController::ApplyGraphicsQuality()
{
    if (UGameUserSettings* Settings = UGameUserSettings::GetGameUserSettings()) {
        Settings->SetOverallScalabilityLevel(FMath::Clamp(GraphicsQuality, 0, 2));
        Settings->ApplySettings(false);
    }
}
void ADawsPlayerController::ApplyCameraFOV()
{
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) Player->SetCameraFOV(CameraFOV);
    else if (ADawsVehiclePawn* Vehicle = Cast<ADawsVehiclePawn>(GetPawn())) Vehicle->SetCameraFOV(CameraFOV);
}

void ADawsPlayerController::ContinueGame()
{
    if (GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            if (Missions->GetCurrentMissionId() == TEXT("campaign_complete")) { RestartCampaign(); return; }
        }
    }
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) {
        if (UGameplayStatics::DoesSaveGameExist(TEXT("DAWSLegend_Main"), 0)) Player->LoadCheckpoint();
    }
    StartGame();
}

void ADawsPlayerController::RestartCampaign()
{
    if (!GetGameInstance()) return;
    UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    if (!Missions || Missions->GetCurrentMissionId() != TEXT("campaign_complete")) return;
    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) Saves->DeleteSave();
    if (ADawsGameMode* Mode = Cast<ADawsGameMode>(UGameplayStatics::GetGameMode(this))) Mode->RestartCampaignWorld();
    else Missions->StartCampaign();
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) Player->ResetForNewCampaign();
    bHasStartedGame = true;
    bSettingsMenuOpen = false;
    bDawsPaused = false;
    SetPause(false);
    bShowMouseCursor = false;
    FInputModeGameOnly Mode;
    SetInputMode(Mode);
}

void ADawsPlayerController::StartNewCampaign()
{
    if (bHasStartedGame || !GetGameInstance()) return;
    bool bHadSave = false;
    if (UDawsSaveSubsystem* Saves = GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) { bHadSave = Saves->HasSave(); Saves->DeleteSave(); }
    if (bHadSave) {
        if (ADawsGameMode* Mode = Cast<ADawsGameMode>(UGameplayStatics::GetGameMode(this))) Mode->RestartCampaignWorld();
        else if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->StartCampaign();
    } else {
        if (UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Missions->StartCampaign();
        for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) It->RefreshMissionMarker();
        for (TActorIterator<ADawsStoryNPC> It(GetWorld()); It; ++It) It->ResetForNewCampaign();
    }
    if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetPawn())) Player->ResetForNewCampaign();
    StartGame();
}
