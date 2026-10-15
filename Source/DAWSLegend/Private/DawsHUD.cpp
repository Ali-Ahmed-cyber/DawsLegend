#include "DawsHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "DawsPlayerCharacter.h"
#include "DawsHealthComponent.h"
#include "DawsCombatComponent.h"
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
    DrawTextLine(TEXT("DAWS LEGEND"), 42, 32, 1.25f, FLinearColor(0.15f,0.95f,0.78f));
    DrawTextLine(TEXT("STORY CAMPAIGN  |  CHAPTER 01"), 44, 68, 0.78f, FLinearColor::White);
    DrawTextLine(TEXT("WASD MOVE   SHIFT SPRINT   SPACE JUMP"), 44, H-72, 0.78f, FLinearColor::White);
    if (GetWorld() && GetWorld()->GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetWorld()->GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) {
            DrawTextLine(FString::Printf(TEXT("MISSION: %s"), *Missions->GetCurrentMissionTitle().ToString()), 44, H-145, 0.84f, FLinearColor(1.0f,0.72f,0.25f));
            const TArray<FDawsMissionObjective>& Objectives = Missions->GetObjectives();
            if (Objectives.Num() > 0) DrawTextLine(FString::Printf(TEXT("OBJECTIVE: %s"), *Objectives[0].Description.ToString()), 44, H-108, 0.78f, FLinearColor::White);
        }
    }
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(GetOwningPawn())) {
        if (Player->HealthComponent) DrawTextLine(FString::Printf(TEXT("HEALTH  %.0f / %.0f"), Player->HealthComponent->CurrentHealth, Player->HealthComponent->MaxHealth), W-245, 34, 0.9f, FLinearColor(0.25f,0.95f,0.55f));
        if (Player->CombatComponent) DrawTextLine(FString::Printf(TEXT("AMMO  %d / %d"), Player->CombatComponent->MagazineAmmo, Player->CombatComponent->ReserveAmmo), W-245, 68, 0.9f, FLinearColor::White);
    }
    // Subtle centre reticle.
    Canvas->K2_DrawLine(FVector2D(W*0.5f-8,H*0.5f), FVector2D(W*0.5f+8,H*0.5f), 1.5f, FLinearColor::White);
    Canvas->K2_DrawLine(FVector2D(W*0.5f,H*0.5f-8), FVector2D(W*0.5f,H*0.5f+8), 1.5f, FLinearColor::White);
}