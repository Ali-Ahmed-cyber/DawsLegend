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
    bool IsSettingsMenuOpen() const { return bSettingsMenuOpen; }
    float GetMouseSensitivity() const { return MouseSensitivity; }
    float GetCameraFOV() const { return CameraFOV; }
    bool AreSubtitlesEnabled() const { return bSubtitlesEnabled; }
    int32 GetSelectedSetting() const { return SelectedSetting; }
    int32 GetGraphicsQuality() const { return GraphicsQuality; }
protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
private:
    bool bDawsPaused = false;
    bool bHasStartedGame = false;
    bool bSettingsMenuOpen = false;
    float MouseSensitivity = 1.0f;
    float CameraFOV = 90.0f;
    bool bSubtitlesEnabled = true;
    int32 SelectedSetting = 0;
    int32 GraphicsQuality = 1;
    void TogglePause();
    void StartGame();
    void StartNewCampaign();
    void ContinueGame();
    void RestartCampaign();
    void ToggleSettings();
    void NavigateSettingsUp();
    void NavigateSettingsDown();
    void AdjustSettingLeft();
    void AdjustSettingRight();
    void ApplyCameraFOV();
    void ApplyGraphicsQuality();
    void LoadOptions();
    void SaveOptions();
};