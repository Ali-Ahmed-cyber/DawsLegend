#pragma once
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DawsGameMode.generated.h"
UCLASS()
class DAWSLEGEND_API ADawsGameMode : public AGameModeBase
{
    GENERATED_BODY()
public:
    ADawsGameMode();
    virtual void StartPlay() override;
    void RestartCampaignWorld();
private:
    void SpawnCampaignWorldActors();
};