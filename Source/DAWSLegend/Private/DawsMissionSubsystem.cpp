#include "DawsMissionSubsystem.h"
void UDawsMissionSubsystem::StartMission(FString MissionId, FText MissionTitle, const TArray<FDawsMissionObjective>& Objectives)
{
    CurrentMissionId = MoveTemp(MissionId);
    CurrentMissionTitle = MoveTemp(MissionTitle);
    CurrentObjectives = Objectives;
}
void UDawsMissionSubsystem::StartCampaign()
{
    CampaignMissionIndex = 0;
    bCampaignActive = true;
    StartCampaignMission(CampaignMissionIndex);
}
bool UDawsMissionSubsystem::RestoreCampaignAtMission(const FString& MissionId)
{
    if (MissionId == TEXT("campaign_complete")) { bCampaignActive = false; CampaignMissionIndex = 15; CurrentMissionId = MissionId; CurrentMissionTitle = FText::FromString(TEXT("Dawn of the City — Campaign Complete")); CurrentObjectives.Reset(); return true; }
    static const TCHAR* Ids[] = {
        TEXT("chapter01_plaza"), TEXT("chapter01_supply"), TEXT("chapter01_rescue"),
        TEXT("chapter02_market"), TEXT("chapter02_clinic"), TEXT("chapter02_courier"),
        TEXT("chapter03_relay"), TEXT("chapter03_escort"), TEXT("chapter03_broadcast"),
        TEXT("chapter04_yard"), TEXT("chapter04_workers"), TEXT("chapter04_escape"),
        TEXT("chapter05_evidence"), TEXT("chapter05_depot"), TEXT("chapter05_finale")
    };
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(Ids); ++Index) {
        if (MissionId == Ids[Index]) { bCampaignActive = true; StartCampaignMission(Index); return true; }
    }
    return false;
}
void UDawsMissionSubsystem::StartCampaignMission(int32 MissionIndex)
{
    static const TCHAR* Ids[] = {
        TEXT("chapter01_plaza"), TEXT("chapter01_supply"), TEXT("chapter01_rescue"),
        TEXT("chapter02_market"), TEXT("chapter02_clinic"), TEXT("chapter02_courier"),
        TEXT("chapter03_relay"), TEXT("chapter03_escort"), TEXT("chapter03_broadcast"),
        TEXT("chapter04_yard"), TEXT("chapter04_workers"), TEXT("chapter04_escape"),
        TEXT("chapter05_evidence"), TEXT("chapter05_depot"), TEXT("chapter05_finale")
    };
    static const TCHAR* Titles[] = {
        TEXT("The First Signal"), TEXT("The Empty Storehouse"), TEXT("A Mechanic's Warning"),
        TEXT("Smoke Over the Market"), TEXT("Protect the Clinic"), TEXT("The Courier"),
        TEXT("The Silent Relay"), TEXT("Escort the Technician"), TEXT("Broadcast the Truth"),
        TEXT("The Broker's Route"), TEXT("Free the Workers"), TEXT("Road to the River"),
        TEXT("Dawn of the City"), TEXT("Hold the Depot"), TEXT("The Final Broadcast")
    };
    static const TCHAR* DialogueText[] = {
        TEXT("Hiba (radio): The plaza is not safe. Get to the marker and keep your eyes open."),
        TEXT("Bashir: These crates were emptied before the convoy arrived. Someone knew the route."),
        TEXT("Bashir: A note from the convoy points to a night transfer through the market."),
        TEXT("Hiba (radio): Smoke is rising over the market. Check the burned store before they clean it up."),
        TEXT("Fawzi: The clinic needs those supplies. I will move the families while you hold the lane."),
        TEXT("Bashir: The courier has the transfer code. Stop him before he reaches the bridge."),
        TEXT("Hiba (radio): The relay went silent. Restore it and I can reach the whole district."),
        TEXT("Captain Nadir: Keep the technician alive. Without his evidence, the case will collapse."),
        TEXT("Hiba (radio): I have the transmitter. Send me proof I can verify, not rumours."),
        TEXT("Captain Nadir: The Broker's yard has three entrances. Choose your approach carefully."),
        TEXT("Worker: They locked us in after we refused to move the medical cargo."),
        TEXT("Bashir: The river road is blocked. I can get the truck moving, but you must clear the route."),
        TEXT("Hiba (radio): One last record remains. Bring it to the depot intact."),
        TEXT("Fawzi: The depot is holding. People are bringing water and bandages. Do not let them through."),
        TEXT("Captain Nadir: This is the final broadcast. Once it goes out, there is no turning back.")
    };
    static const TCHAR* TaskTypes[] = {
        TEXT("reach"), TEXT("collect"), TEXT("talk"),
        TEXT("evidence"), TEXT("defeat"), TEXT("defeat"),
        TEXT("talk"), TEXT("reach"), TEXT("reach"),
        TEXT("reach"), TEXT("talk"), TEXT("defeat"),
        TEXT("evidence"), TEXT("defeat"), TEXT("talk")
    };
    static const TCHAR* TaskTargets[] = { TEXT(""), TEXT(""), TEXT("bashir"), TEXT(""), TEXT(""), TEXT(""), TEXT("technician"), TEXT(""), TEXT(""), TEXT(""), TEXT("worker"), TEXT(""), TEXT(""), TEXT(""), TEXT("the_broker") };
    static const int32 RequiredCounts[] = { 0,3,1,1,3,1,1,0,0,0,1,4,1,5,1 };
    static const TCHAR* ObjectivesText[] = {
        TEXT("Reach the plaza and secure the area"), TEXT("Recover the missing supplies"), TEXT("Speak to Bashir at the workshop"),
        TEXT("Investigate the burned market store"), TEXT("Protect the clinic supply route"), TEXT("Intercept the courier"),
        TEXT("Speak to the technician at the relay"), TEXT("Escort the technician safely"), TEXT("Reach the broadcast station"),
        TEXT("Scout the broker's industrial yard"), TEXT("Speak to the workers and learn who detained them"), TEXT("Secure the river-side route"),
        TEXT("Recover the last piece of evidence"), TEXT("Defend the central depot"), TEXT("Confront the Broker before the final broadcast")
    };
    if (MissionIndex < 0 || MissionIndex >= UE_ARRAY_COUNT(Ids)) {
        bCampaignActive = false;
        CurrentMissionId = TEXT("campaign_complete");
        CurrentMissionTitle = FText::FromString(TEXT("Dawn of the City — Campaign Complete"));
        CurrentObjectives.Reset();
        return;
    }
    CampaignMissionIndex = MissionIndex;
    FDawsMissionObjective Objective;
    CurrentDialogue = FText::FromString(DialogueText[MissionIndex]);
    CurrentTaskType = TaskTypes[MissionIndex];
    CurrentTaskTargetId = TaskTargets[MissionIndex];
    CurrentTaskProgress = 0;
    RequiredTaskProgress = RequiredCounts[MissionIndex];
    Objective.Id = FString(Ids[MissionIndex]) + TEXT("_reach");
    Objective.Description = FText::FromString(ObjectivesText[MissionIndex]);
    Objective.bCompleted = false;
    TArray<FDawsMissionObjective> Objectives;
    Objectives.Add(Objective);
    StartMission(Ids[MissionIndex], FText::FromString(Titles[MissionIndex]), Objectives);
}
void UDawsMissionSubsystem::RegisterTaskProgress(const FString& TaskType, int32 Amount)
{
    if (TaskType != CurrentTaskType || Amount <= 0 || RequiredTaskProgress <= 0) return;
    CurrentTaskProgress = FMath::Clamp(CurrentTaskProgress + Amount, 0, RequiredTaskProgress);
}
void UDawsMissionSubsystem::RestoreTaskProgress(int32 Progress)
{
    CurrentTaskProgress = FMath::Clamp(Progress, 0, RequiredTaskProgress);
}
void UDawsMissionSubsystem::RegisterNPCInteraction(const FString& NPCId)
{
    if (CurrentTaskType == TEXT("talk") && CurrentTaskTargetId == NPCId) RegisterTaskProgress(TEXT("talk"));
}
bool UDawsMissionSubsystem::CanCompleteCurrentMission() const
{
    return RequiredTaskProgress <= 0 || CurrentTaskProgress >= RequiredTaskProgress;
}
bool UDawsMissionSubsystem::CompleteObjective(FString ObjectiveId)
{
    for (FDawsMissionObjective& Objective : CurrentObjectives)
    {
        if (Objective.Id == ObjectiveId && !Objective.bCompleted)
        {
            Objective.bCompleted = true;
            if (bCampaignActive && IsMissionComplete()) StartCampaignMission(CampaignMissionIndex + 1);
            return true;
        }
    }
    return false;
}
void UDawsMissionSubsystem::ResetCurrentMission()
{
    for (FDawsMissionObjective& Objective : CurrentObjectives) Objective.bCompleted = false;
}
bool UDawsMissionSubsystem::IsMissionComplete() const
{
    if (CurrentObjectives.IsEmpty()) return false;
    for (const FDawsMissionObjective& Objective : CurrentObjectives)
        if (!Objective.bCompleted) return false;
    return true;
}