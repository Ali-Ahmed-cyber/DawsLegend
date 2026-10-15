#include "DawsHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "DawsPlayerCharacter.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
#include "DawsPlayerController.h"
#include "DawsMissionSubsystem.h"
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
void ADawsHUD::DrawHUD()
{
    Super::DrawHUD();
    if (!Canvas) return;
    const float W = Canvas->SizeX, H = Canvas->SizeY;
    if (APlayerController* PC = GetOwningPlayerController()) {
        if (ADawsPlayerController* DawsPC = Cast<ADawsPlayerController>(PC)) {
            if (!DawsPC->HasStartedGame()) {
                DrawTextLine(TEXT("DAWS LEGEND"), W*0.5f-170.0f, H*0.26f, 2.0f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("THE FIRST SIGNAL"), W*0.5f-145.0f, H*0.37f, 1.15f, FLinearColor(1.0f,0.72f,0.25f));
                DrawTextLine(TEXT("A story of loyalty, courage and a city in crisis"), W*0.5f-245.0f, H*0.46f, 0.9f, FLinearColor::White);
                DrawTextLine(TEXT("PRESS ENTER TO BEGIN"), W*0.5f-150.0f, H*0.60f, 1.0f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("WASD MOVE   MOUSE LOOK   SHIFT SPRINT   SPACE JUMP"), W*0.5f-245.0f, H*0.70f, 0.78f, FLinearColor::White);
                DrawTextLine(TEXT("LEFT CLICK FIRE   R RELOAD   E INTERACT   F5 SAVE   F9 LOAD"), W*0.5f-260.0f, H*0.75f, 0.72f, FLinearColor::White);
                return;
            }
        }
        if (PC->IsPaused()) {
            DrawTextLine(TEXT("PAUSED"), W*0.5f-70.0f, H*0.38f, 1.8f, FLinearColor(0.15f,0.95f,0.78f));
            DrawTextLine(TEXT("Press ESC to resume"), W*0.5f-100.0f, H*0.46f, 1.0f, FLinearColor::White);
            return;
        }
    }
    DrawTextLine(TEXT("DAWS LEGEND"), 42, 32, 1.25f, FLinearColor(0.15f,0.95f,0.78f));
    DrawTextLine(TEXT("STORY CAMPAIGN  |  CHAPTER 01"), 44, 68, 0.78f, FLinearColor::White);
    DrawTextLine(TEXT("WASD MOVE   SHIFT SPRINT   SPACE JUMP   E INTERACT"), 44, H-72, 0.78f, FLinearColor::White);
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            DrawTextLine(FString::Printf(TEXT("MISSION: %s"), *Missions->GetCurrentMissionTitle().ToString()), 44, H-145, 0.84f, FLinearColor(1.0f,0.72f,0.25f));
            const TArray<FDawsMissionObjective>& Objectives = Missions->GetObjectives();
            if (Objectives.Num() > 0) DrawTextLine(FString::Printf(TEXT("OBJECTIVE: %s"), *Objectives[0].Description.ToString()), 44, H-108, 0.78f, FLinearColor::White);
            DrawTextLine(Missions->GetCurrentDialogue().ToString(), 44, H-185, 0.68f, FLinearColor(0.85f,0.92f,1.0f));
        }
    }
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            if (Missions->GetCurrentMissionId() == TEXT("campaign_complete")) {
                DrawTextLine(TEXT("DAWS LEGEND"), W*0.5f-120.0f, H*0.28f, 1.8f, FLinearColor(0.15f,0.95f,0.78f));
                DrawTextLine(TEXT("CAMPAIGN COMPLETE"), W*0.5f-170.0f, H*0.39f, 1.35f, FLinearColor(1.0f,0.72f,0.25f));
                DrawTextLine(TEXT("The city will remember what you did."), W*0.5f-190.0f, H*0.48f, 0.95f, FLinearColor::White);
                DrawTextLine(TEXT("Press ESC to pause or resume"), W*0.5f-150.0f, H*0.57f, 0.8f, FLinearColor::White);
                return;
            }
        }
    }
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetOwningPawn())) {
        if (Player->HealthComponent) {
            DrawTextLine(FString::Printf(TEXT("HEALTH  %.0f / %.0f"), Player->HealthComponent->CurrentHealth, Player->HealthComponent->MaxHealth), W-245, 34, 0.9f, FLinearColor(0.25f,0.95f,0.55f));
            if (Player->HealthComponent->CurrentHealth <= 0.0f) DrawTextLine(TEXT("DOWN — RESPAWNING"), W*0.5f-125.0f, H*0.42f, 1.25f, FLinearColor(1.0f,0.18f,0.12f));
        }
        if (Player->CombatComponent) DrawTextLine(FString::Printf(TEXT("AMMO  %d / %d"), Player->CombatComponent->MagazineAmmo, Player->CombatComponent->ReserveAmmo), W-245, 68, 0.9f, FLinearColor::White);
    }
    // Subtle centre reticle.
    Canvas->K2_DrawLine(FVector2D(W*0.5f-8,H*0.5f), FVector2D(W*0.5f+8,H*0.5f), 1.5f, FLinearColor::White);
    Canvas->K2_DrawLine(FVector2D(W*0.5f,H*0.5f-8), FVector2D(W*0.5f,H*0.5f+8), 1.5f, FLinearColor::White);
}