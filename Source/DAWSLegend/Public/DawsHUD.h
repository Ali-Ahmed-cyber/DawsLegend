#pragma once
#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DawsHUD.generated.h"
UCLASS()
class DAWSLEGEND_API ADawsHUD : public AHUD
{
    GENERATED_BODY()
public:
    virtual void DrawHUD() override;
private:
    void DrawTextLine(const FString& Text, float X, float Y, float Scale, FLinearColor Color);
};