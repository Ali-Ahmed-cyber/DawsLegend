#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DawsMissionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FDawsMissionObjective
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Id;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCompleted = false;
};

UCLASS()
class DAWSLEGEND_API UDawsMissionSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Missions")
    void StartMission(FString MissionId, FText MissionTitle, const TArray<FDawsMissionObjective>& Objectives);
    UFUNCTION(BlueprintCallable, Category="Campaign")
    void StartCampaign();
    UFUNCTION(BlueprintCallable, Category="Campaign")
    bool RestoreCampaignAtMission(const FString& MissionId);
    UFUNCTION(BlueprintCallable, Category="Missions")
    bool CompleteObjective(FString ObjectiveId);
    UFUNCTION(BlueprintCallable, Category="Missions")
    void RegisterTaskProgress(const FString& TaskType, int32 Amount = 1);
    UFUNCTION(BlueprintPure, Category="Missions")
    bool CanCompleteCurrentMission() const;
    UFUNCTION(BlueprintPure, Category="Missions")
    FString GetCurrentTaskType() const { return CurrentTaskType; }
    UFUNCTION(BlueprintPure, Category="Missions")
    int32 GetTaskProgress() const { return CurrentTaskProgress; }
    UFUNCTION(BlueprintPure, Category="Missions")
    int32 GetRequiredTaskProgress() const { return RequiredTaskProgress; }
    UFUNCTION(BlueprintCallable, Category="Missions")
    void ResetCurrentMission();
    UFUNCTION(BlueprintPure, Category="Missions")
    FString GetCurrentMissionId() const { return CurrentMissionId; }
    UFUNCTION(BlueprintPure, Category="Missions")
    FText GetCurrentMissionTitle() const { return CurrentMissionTitle; }
    UFUNCTION(BlueprintPure, Category="Dialogue")
    FText GetCurrentDialogue() const { return CurrentDialogue; }
    UFUNCTION(BlueprintCallable, Category="Dialogue")
    void SetCurrentDialogue(FText DialogueLine) { CurrentDialogue = MoveTemp(DialogueLine); }
    UFUNCTION(BlueprintPure, Category="Campaign")
    int32 GetCampaignMissionNumber() const { return FMath::Clamp(CampaignMissionIndex + 1, 1, 15); }
    UFUNCTION(BlueprintPure, Category="Missions")
    TArray<FDawsMissionObjective> GetObjectives() const { return CurrentObjectives; }
    UFUNCTION(BlueprintPure, Category="Missions")
    bool IsMissionComplete() const;
private:
    void StartCampaignMission(int32 MissionIndex);
    int32 CampaignMissionIndex = 0;
    bool bCampaignActive = false;
    FString CurrentTaskType = TEXT("reach");
    int32 CurrentTaskProgress = 0;
    int32 RequiredTaskProgress = 0;
    UPROPERTY()
    FString CurrentMissionId;
    UPROPERTY()
    FText CurrentMissionTitle;
    UPROPERTY()
    FText CurrentDialogue;
    UPROPERTY()
    TArray<FDawsMissionObjective> CurrentObjectives;
};