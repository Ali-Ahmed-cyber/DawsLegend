#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DawsSaveGame.generated.h"

UCLASS()
class DAWSLEGEND_API UDawsSaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, Category="Progress")
    int32 StoryChapter = 0;
    UPROPERTY(BlueprintReadWrite, Category="Progress")
    FString CurrentMissionId = TEXT("CH01_M01");
    UPROPERTY(BlueprintReadWrite, Category="Progress")
    TArray<FString> CompletedMissionIds;
    UPROPERTY(BlueprintReadWrite, Category="Player")
    FVector PlayerLocation = FVector::ZeroVector;
    UPROPERTY(BlueprintReadWrite, Category="Player")
    FRotator PlayerRotation = FRotator::ZeroRotator;
    UPROPERTY(BlueprintReadWrite, Category="Player")
    float PlayerHealth = 100.0f;
    UPROPERTY(BlueprintReadWrite, Category="Progress")
    int32 CurrentTaskProgress = 0;
    UPROPERTY(BlueprintReadWrite, Category="Settings")
    float MasterVolume = 1.0f;
    UPROPERTY(BlueprintReadWrite, Category="Settings")
    float MouseSensitivity = 1.0f;
    UPROPERTY(BlueprintReadWrite, Category="Settings")
    float CameraFOV = 90.0f;
    UPROPERTY(BlueprintReadWrite, Category="Settings")
    bool bSubtitlesEnabled = true;
};