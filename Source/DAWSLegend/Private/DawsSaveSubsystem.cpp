#include "DawsSaveSubsystem.h"
#include "DawsSaveGame.h"
#include "DawsMissionSubsystem.h"
#include "DawsPlayerCharacter.h"
#include "DawsCombatComponent.h"
#include "Kismet/GameplayStatics.h"

bool UDawsSaveSubsystem::SaveCheckpoint(int32 StoryChapter, const FString& MissionId, AActor* PlayerActor, float PlayerHealth)
{
    if (!PlayerActor) return false;
    UDawsSaveGame* Save = Cast<UDawsSaveGame>(UGameplayStatics::CreateSaveGameObject(UDawsSaveGame::StaticClass()));
    if (!Save) return false;
    Save->StoryChapter = StoryChapter;
    Save->CurrentMissionId = MissionId;
    Save->PlayerLocation = PlayerActor->GetActorLocation();
    Save->PlayerRotation = PlayerActor->GetActorRotation();
    Save->PlayerHealth = FMath::Clamp(PlayerHealth, 0.0f, 100.0f);
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(PlayerActor)) {
        if (Player->CombatComponent) { Save->MagazineAmmo = Player->CombatComponent->MagazineAmmo; Save->ReserveAmmo = Player->CombatComponent->ReserveAmmo; }
    }
    if (GetGameInstance()) {
        if (const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>()) Save->CurrentTaskProgress = Missions->GetTaskProgress();
    }
    if (!UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, UserIndex)) return false;
    CachedSave = Save;
    return true;
}
bool UDawsSaveSubsystem::SaveTaskProgress(AActor* PlayerActor, float PlayerHealth)
{
    if (!PlayerActor || !GetGameInstance()) return false;
    const UDawsMissionSubsystem* Missions = GetGameInstance()->GetSubsystem<UDawsMissionSubsystem>();
    if (!Missions) return false;
    const FString MissionId = Missions->GetCurrentMissionId();
    const int32 Chapter = MissionId.StartsWith(TEXT("chapter")) ? FCString::Atoi(*MissionId.Mid(7,2)) : 5;
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
        return SaveCheckpoint(Chapter, MissionId, PlayerActor, PlayerHealth);
    UDawsSaveGame* Save = Cast<UDawsSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
    if (!Save) return SaveCheckpoint(Chapter, MissionId, PlayerActor, PlayerHealth);
    Save->StoryChapter = Chapter;
    Save->CurrentMissionId = MissionId;
    Save->PlayerHealth = FMath::Clamp(PlayerHealth, 0.0f, 100.0f);
    Save->CurrentTaskProgress = Missions->GetTaskProgress();
    if (const ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(PlayerActor)) {
        if (Player->CombatComponent) { Save->MagazineAmmo = Player->CombatComponent->MagazineAmmo; Save->ReserveAmmo = Player->CombatComponent->ReserveAmmo; }
    }
    if (!UGameplayStatics::SaveGameToSlot(Save, SaveSlotName, UserIndex)) return false;
    CachedSave = Save;
    return true;
}
bool UDawsSaveSubsystem::LoadCheckpoint(AActor* PlayerActor, int32& OutStoryChapter, FString& OutMissionId, float& OutPlayerHealth, int32& OutTaskProgress)
{
    if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex)) return false;
    UDawsSaveGame* Save = Cast<UDawsSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));
    if (!Save) return false;
    CachedSave = Save;
    OutStoryChapter = Save->StoryChapter;
    OutMissionId = Save->CurrentMissionId;
    OutPlayerHealth = Save->PlayerHealth;
    OutTaskProgress = Save->CurrentTaskProgress;
    if (PlayerActor)
    {
        PlayerActor->SetActorLocation(Save->PlayerLocation);
        PlayerActor->SetActorRotation(Save->PlayerRotation);
        if (ADawsPlayerCharacter* Player = Cast<ADawsPlayerCharacter>(PlayerActor)) {
            if (Player->CombatComponent) {
                Player->CombatComponent->MagazineAmmo = FMath::Clamp(Save->MagazineAmmo, 0, Player->CombatComponent->MagazineCapacity);
                Player->CombatComponent->ReserveAmmo = FMath::Max(0, Save->ReserveAmmo);
                Player->CombatComponent->OnAmmoChanged.Broadcast(Player->CombatComponent->MagazineAmmo, Player->CombatComponent->ReserveAmmo);
            }
        }
    }
    return true;
}
bool UDawsSaveSubsystem::HasSave() const
{
    return UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);
}
void UDawsSaveSubsystem::DeleteSave()
{
    if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex))
        UGameplayStatics::DeleteGameInSlot(SaveSlotName, UserIndex);
    CachedSave = nullptr;
}