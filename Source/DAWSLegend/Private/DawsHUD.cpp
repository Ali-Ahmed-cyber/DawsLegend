#include "DawsHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
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
    DrawTextLine(TEXT("OBJECTIVE: SECURE THE PLAZA"), 44, H-108, 0.86f, FLinearColor(1.0f,0.72f,0.25f));
    // Subtle centre reticle.
    Canvas->K2_DrawLine(FVector2D(W*0.5f-8,H*0.5f), FVector2D(W*0.5f+8,H*0.5f), 1.5f, FLinearColor::White);
    Canvas->K2_DrawLine(FVector2D(W*0.5f,H*0.5f-8), FVector2D(W*0.5f,H*0.5f+8), 1.5f, FLinearColor::White);
}