#include "DawsGameMode.h"
#include "DawsPlayerCharacter.h"
#include "DawsWorldBuilder.h"
#include "DawsHUD.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
ADawsGameMode::ADawsGameMode()
{
    DefaultPawnClass = ADawsPlayerCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
    HUDClass = ADawsHUD::StaticClass();
}
void ADawsGameMode::StartPlay()
{
    Super::StartPlay();
    if (GetWorld()) GetWorld()->SpawnActor<ADawsWorldBuilder>(FVector::ZeroVector, FRotator::ZeroRotator);
}
