#include "DawsGameMode.h"
#include "DawsPlayerCharacter.h"
#include "GameFramework/PlayerController.h"
ADawsGameMode::ADawsGameMode()
{
    DefaultPawnClass = ADawsPlayerCharacter::StaticClass();
    PlayerControllerClass = APlayerController::StaticClass();
}
