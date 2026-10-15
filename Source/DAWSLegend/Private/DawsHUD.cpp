#include "DawsHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "DawsPlayerCharacter.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
#include "DawsPlayerController.h"
#include "DawsVehiclePawn.h"
#include "DawsMissionSubsystem.h"
#include "DawsSaveSubsystem.h"
#include "DawsMissionTrigger.h"
#include "DawsStoryNPC.h"
#include "EngineUtils.h"
#include "Engine/GameInstance.h"
void ADawsHUD::DrawTextLine(const FString& Text, float X, float Y, float Scale, FLinearColor Color)
{
    if (!Canvas || !GEngine) return;
    UFont* Font = GEngine->GetMediumFont();
    FCanvasTextItem Item(FVector2D(X,Y), FText::FromString(Text), Font, Color);
    Item.Scale = FVector2D(Scale,Scale);
    Item.EnableShadow(FLinearColor::Black);
    Canvas->DrawItem(Item);
}
void ADawsHUD::DrawOptionsPanel(float W, float H, const ADawsPlayerController* DawsPC)
{
    if (!DawsPC) return;
    DrawTextLine(TEXT("OPTIONS"), W*0.5f-85.0f, H*0.27f, 1.7f, FLinearColor(0.15f,0.95f,0.78f));
    DrawTextLine(FString::Printf(TEXT("%s Mouse sensitivity: %.1f"), DawsPC->GetSelectedSetting()==0 ? TEXT(">") : TEXT(" "), DawsPC->GetMouseSensitivity()), W*0.5f-180.0f, H*0.40f, 0.95f, FLinearColor::White);
    DrawTextLine(FString::Printf(TEXT("%s Camera FOV: %.0f"), DawsPC->GetSelectedSetting()==1 ? TEXT(">") : TEXT(" "), DawsPC->GetCameraFOV()), W*0.5f-180.0f, H*0.47f, 0.95f, FLinearColor::White);
    DrawTextLine(FString::Printf(TEXT("%s Subtitles: %s"), DawsPC->GetSelectedSetting()==2 ? TEXT(">") : TEXT(" "), DawsPC->AreSubtitlesEnabled() ? TEXT("ON") : TEXT("OFF")), W*0.5f-180.0f, H*0.54f, 0.95f, FLinearColor::White);
    DrawTextLine(TEXT("UP/DOWN SELECT   LEFT/RIGHT CHANGE   F1 CLOSE   ESC RESUME"), W*0.5f-255.0f, H*0.68f, 0.75f, FLinearColor::White);
}
void ADawsHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;
    const float W = Canvas->SizeX, H = Canvas->SizeY;
    if (APlayerController* PC = GetOwningPlayerController()) {
        if (ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(PC)) {
            if (!DawsPC->HasStartedGame()) {
                if (DawsPC->IsSettingsMenuOpen()) { DrawOptionsPanel(W, H, DawsPC); return; }
                DrawTextLine(TEXT("DAWS LEGEND"), W*0.5f-170.0f, H*0.26f, 2.0f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("THE FIRST SIGNAL"), W*0.5f-145.0f, H*0.37f, 1.15f, FLinearColor(1.0f,0.72f,0.25f));
                DrawTextLine(TEXT("A story of loyalty, courage and a city in crisis"), W*0.5f-245.0f, H*0.46f, 0.9f, FLinearColor::White);
                bool bHasSavedGame = false;
                if (GetWorld() && GetWorld()->GetGameInstance()) {
                    if (const UDawsSaveSubsystem* Saves = GetWorld()->GetGameInstance()->GetSubsystem<UDawsSaveSubsystem>()) bHasSavedGame = Saves->HasSave();
                }
                DrawTextLine(bHasSavedGame ? TEXT("ENTER NEW GAME   |   F2 CONTINUE   |   F1 OPTIONS") : TEXT("ENTER NEW GAME   |   NO SAVE YET   |   F1 OPTIONS"), W*0.5f-225.0f, H*0.60f, 0.95f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("WASD MOVE   MOUSE LOOK   SHIFT SPRINT   SPACE JUMP"), W*0.5f-245.0f, H*0.70f, 0.78f, FLinearColor::White);
                DrawTextLine(TEXT("LEFT CLICK FIRE   R RELOAD   E INTERACT   F5 SAVE   F9 LOAD"), W*0.5f-260.0f, H*0.75f, 0.72f, FLinearColor::White);
                DrawTextLine(TEXT("GAMEPAD START: NEW GAME   Y: CONTINUE"), W*0.5f-200.0f, H*0.80f, 0.72f, FLinearColor::White);
                return;
            }
        }
        if (PC->IsPaused()) {
            if (ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(PC); DawsPC && DawsPC->IsSettingsMenuOpen()) {
                DrawOptionsPanel(W, H, DawsPC);
                return;
            }
            DrawTextLine(TEXT("PAUSED"), W*0.5f-70.0f, H*0.38f, 1.8f, FLinearColor(0.15f,0.95f,0.78f));
            DrawTextLine(TEXT("F1 OPTIONS   ESC RESUME"), W*0.5f-135.0f, H*0.46f, 1.0f, FLinearColor::White);
            return;
        }
    }
    DrawTextLine(TEXT("DAWS LEGEND"), 42, 32, 1.25f, FLinearColor(0.15f,0.95f,0.78f));
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetOwningPawn())) {
        const FString Status = Player->GetStatusMessage();
        if (!Status.IsEmpty()) DrawTextLine(Status, W*0.5f-145.0f, 118.0f, 0.95f, FLinearColor(0.15f,0.95f,0.78f));
    }
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>())
            DrawTextLine(FString::Printf(TEXT("STORY CAMPAIGN  |  CHAPTER %02d"), Missions->GetCurrentChapterNumber()), 44, 68, 0.78f, FLinearColor::White);
    }
    if (!Cast<ADawsVehiclePawn>(GetOwningPawn())) DrawTextLine(TEXT("WASD MOVE   SHIFT SPRINT   SPACE JUMP   E INTERACT"), 44, H-72, 0.78f, FLinearColor::White);
    if (const APawn* CurrentPawn = GetOwningPawn()) {
        const float DistrictY = CurrentPawn->GetActorLocation().Y;
        const TCHAR* DistrictName = DistrictY < 2200.0f ? TEXT("CENTRAL PLAZA") : (DistrictY < 4300.0f ? TEXT("MARKET & CLINIC") : (DistrictY < 6500.0f ? TEXT("RADIO DISTRICT") : (DistrictY < 8700.0f ? TEXT("INDUSTRIAL ROUTE") : TEXT("CENTRAL DEPOT"))));
        DrawTextLine(DistrictName, W-230.0f, H-112.0f, 0.72f, FLinearColor(0.75f,0.85f,0.82f));
    }
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            DrawTextLine(FString::Printf(TEXT("MISSION %02d / 15  |  %s"), Missions->GetCampaignMissionNumber(), *Missions->GetCurrentMissionTitle().ToString()), 44, H-145, 0.84f, FLinearColor(1.0f,0.72f,0.25f));
            const TArray<FDawsMissionObjective>& Objectives = Missions->GetObjectives();
            if (Objectives.Num() > 0) DrawTextLine(FString::Printf(TEXT("OBJECTIVE: %s"), *Objectives[0].Description.ToString()), 44, H-108, 0.78f, FLinearColor::White);
            if (APawn* CurrentPawn = GetOwningPawn()) {
                for (TActorIterator<ADawsMissionTrigger> It(GetWorld()); It; ++It) {
                    if (It->MissionId.ToString() != Missions->GetCurrentMissionId()) continue;
                    FVector TargetLocation = It->GetActorLocation();
                    if (Missions->GetCurrentTaskType() == TEXT("talk")) {
                        for (TActorIterator<ADawsStoryNPC> NPCIt(GetWorld()); NPCIt; ++NPCIt) {
                            if (NPCIt->GetNPCId().ToString() == Missions->GetCurrentTaskTargetId()) { TargetLocation = NPCIt->GetActorLocation(); break; }
                        }
                    }
                    const FVector Delta = TargetLocation - CurrentPawn->GetActorLocation();
                    const float DistanceMeters = Delta.Size2D() / 100.0f;
                    const FVector Direction = Delta.GetSafeNormal2D();
                    const float ForwardDot = FVector::DotProduct(CurrentPawn->GetActorForwardVector().GetSafeNormal2D(), Direction);
                    const float RightDot = FVector::DotProduct(CurrentPawn->GetActorRightVector().GetSafeNormal2D(), Direction);
                    const TCHAR* Bearing = ForwardDot > 0.65f ? TEXT("AHEAD") : (ForwardDot < -0.65f ? TEXT("BEHIND") : (RightDot > 0.0f ? TEXT("RIGHT") : TEXT("LEFT")));
                    DrawTextLine(FString::Printf(TEXT("OBJECTIVE WAYPOINT  |  %.0f m  |  %s"), DistanceMeters, Bearing), W*0.5f-185.0f, 34, 0.9f, FLinearColor(0.95f,0.78f,0.18f));
                    break;
                }
            }

            if (Missions->GetRequiredTaskProgress() > 0) {
                const FString TaskLabel = Missions->GetCurrentTaskType() == TEXT("collect") ? TEXT("SUPPLIES") : (Missions->GetCurrentTaskType() == TEXT("evidence") ? TEXT("EVIDENCE") : (Missions->GetCurrentTaskType() == TEXT("talk") ? TEXT("TALK TO CONTACT") : TEXT("HOSTILES")));
                DrawTextLine(FString::Printf(TEXT("%s: %d / %d"), *TaskLabel, Missions->GetTaskProgress(), Missions->GetRequiredTaskProgress()), 44, H-225, 0.72f, FLinearColor(0.95f,0.78f,0.18f));
            }
            if (const ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(GetOwningPlayerController()); !DawsPC || DawsPC->AreSubtitlesEnabled()) DrawTextLine(Missions->GetCurrentDialogue().ToString(), 44, H-185, 0.68f, FLinearColor(0.85f,0.92f,1.0f));
        }
    }
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            if (Missions->GetCurrentMissionId() == TEXT("campaign_complete")) {
                DrawTextLine(TEXT("DAWS LEGEND"), W*0.5f-120.0f, H*0.28f, 1.8f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("CAMPAIGN COMPLETE"), W*0.5f-170.0f, H*0.39f, 1.35f, FLinearColor(1.0f,0.72f,0.25f));
                DrawTextLine(TEXT("The city will remember what you did."), W*0.5f-190.0f, H*0.48f, 0.95f, FLinearColor::White);
                DrawTextLine(TEXT("F10 RESTART CAMPAIGN   |   ESC PAUSE"), W*0.5f-190.0f, H*0.57f, 0.8f, FLinearColor::White);
                return;
            }
        }
    }
    if (const ADawsVehiclePawn* Vehicle = Cast<ADawsVehiclePawn>(GetOwningPawn())) {
        DrawTextLine(FString::Printf(TEXT("VEHICLE HEALTH  %.0f / 250"), Vehicle->GetVehicleHealth()), W-285, 34, 0.9f, FLinearColor(0.95f,0.78f,0.18f));
        Canvas->K2_DrawLine(FVector2D(W-285,58), FVector2D(W-65,58), 8.0f, FLinearColor(0.12f,0.12f,0.12f));
        Canvas->K2_DrawLine(FVector2D(W-285,58), FVector2D(W-285+220.0f*FMath::Clamp(Vehicle->GetVehicleHealth()/250.0f,0.0f,1.0f),58), 5.0f, FLinearColor(0.95f,0.65f,0.12f));
        DrawTextLine(TEXT("W/S DRIVE   A/D STEER   E EXIT VEHICLE"), 44, H-72, 0.78f, FLinearColor::White);
        if (Vehicle->IsVehicleDisabled()) DrawTextLine(TEXT("VEHICLE DISABLED — PRESS E TO EXIT"), W*0.5f-190.0f, H*0.42f, 1.0f, FLinearColor(1.0f,0.2f,0.12f));
    } else if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetOwningPawn())) {
        if (Player->HealthComponent) {
            DrawTextLine(FString::Printf(TEXT("HEALTH  %.0f / %.0f"), Player->HealthComponent->CurrentHealth, Player->HealthComponent->MaxHealth), W-245, 34, 0.9f, FLinearColor(0.25f,0.95f,0.55f));
            const float HealthRatio = Player->HealthComponent->MaxHealth > 0.0f ? FMath::Clamp(Player->HealthComponent->CurrentHealth/Player->HealthComponent->MaxHealth,0.0f,1.0f) : 0.0f;
            Canvas->K2_DrawLine(FVector2D(W-245,58), FVector2D(W-35,58), 8.0f, FLinearColor(0.12f,0.12f,0.12f));
            const FLinearColor HealthBarColor = HealthRatio < 0.25f ? FLinearColor(0.95f,0.12f,0.10f) : (HealthRatio < 0.5f ? FLinearColor(0.98f,0.62f,0.12f) : FLinearColor(0.20f,0.90f,0.42f));
            Canvas->K2_DrawLine(FVector2D(W-245,58), FVector2D(W-245+210.0f*HealthRatio,58), 5.0f, HealthBarColor);
            if (Player->HealthComponent->CurrentHealth <= 0.0f) DrawTextLine(TEXT("DOWN — RESPAWNING"), W*0.5f-125.0f, H*0.42f, 1.25f, FLinearColor(1.0f,0.18f,0.12f));
        }
        if (Player->CombatComponent) {
            DrawTextLine(FString::Printf(TEXT("AMMO  %d / %d"), Player->CombatComponent->MagazineAmmo, Player->CombatComponent->ReserveAmmo), W-245, 68, 0.9f, FLinearColor::White);
            const float AmmoRatio = Player->CombatComponent->MagazineCapacity > 0 ? FMath::Clamp(static_cast<float>(Player->CombatComponent->MagazineAmmo)/Player->CombatComponent->MagazineCapacity,0.0f,1.0f) : 0.0f;
            Canvas->K2_DrawLine(FVector2D(W-245,92), FVector2D(W-35,92), 6.0f, FLinearColor(0.12f,0.12f,0.12f));
            Canvas->K2_DrawLine(FVector2D(W-245,92), FVector2D(W-245+210.0f*AmmoRatio,92), 4.0f, FLinearColor(0.95f,0.78f,0.18f));
        }
    }
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetOwningPawn())) {
        const FText Prompt = Player->GetTargetInteractionPrompt();
        if (!Prompt.IsEmpty()) DrawTextLine(Prompt.ToString(), W*0.5f-180.0f, H*0.58f, 0.82f, FLinearColor(0.95f,0.85f,0.32f));
    }
    // Subtle centre reticle.
    const ADawsPlayerCharacter* AimPlayer = Cast<ADawsPlayerCharacter>(GetOwningPawn());
    const float ReticleSize = AimPlayer && AimPlayer->IsAiming() ? 4.0f : 8.0f;
    const float ReticleThickness = AimPlayer && AimPlayer->IsAiming() ? 2.0f : 1.5f;
    Canvas->K2_DrawLine(FVector2D(W*0.5f-ReticleSize,H*0.5f), FVector2D(W*0.5f+ReticleSize,H*0.5f), ReticleThickness, FLinearColor::White);
    Canvas->K2_DrawLine(FVector2D(W*0.5f,H*0.5f-ReticleSize), FVector2D(W*0.5f,H*0.5f+ReticleSize), ReticleThickness, FLinearColor::White);
}