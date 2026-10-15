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
    bool SaveCheckpoint(int32 StoryChapter, const FString& MissionId, AActor* PlayerActor, float PlayerHealth);
    UFUNCTION(BlueprintCallable, Category="Save")
    bool LoadCheckpoint(AActor* PlayerActor, int32& OutStoryChapter, FString& OutMissionId, float& OutPlayerHealth);
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