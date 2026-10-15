#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DawsSaveSubsystem.generated.h"

class UDawsSaveGame;

UCLASS()
class DAWSLEGEND_API UDawsSaveSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, Category="Save")
    bool SaveCheckpoint(int32 StoryChapter, const FString& MissionId, AActor* PlayerActor, float PlayerHealth, const FVector& OverrideLocation = FVector::ZeroVector, bool bUseOverrideLocation = false);
    UFUNCTION(BlueprintCallable, Category="Save")
    bool SaveTaskProgress(AActor* PlayerActor, float PlayerHealth, const FString& CollectedPickupId = FString());
    UFUNCTION(BlueprintPure, Category="Save")
    bool IsPickupCollected(const FString& PickupId) const;
    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadCheckpoint(AActor* PlayerActor, int32& OutStoryChapter, FString& OutMissionId, float& OutPlayerHealth, int32& OutTaskProgress);
    UFUNCTION(BlueprintCallable, Category="Save")
    bool HasSave() const;
    UFUNCTION(BlueprintCallable, Category="Save")
    void DeleteSave();
private:
    UPROPERTY()
    TObjectPtr<UDawsSaveGame> CachedSave;
    FString SaveSlotName = TEXT("DAWSLegend_Main");
    int32 UserIndex = 0;
};