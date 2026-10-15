#include "DawsMissionSubsystem.h"

namespace
{
    static const TCHAR* MissionIds[] = {
        TEXT("chapter01_radio"), TEXT("chapter01_plaza"), TEXT("chapter01_workshop"), TEXT("chapter01_supplies"), TEXT("chapter01_driver"),
        TEXT("chapter02_market"), TEXT("chapter02_nurse"), TEXT("chapter02_minibus"), TEXT("chapter02_convoy"), TEXT("chapter02_sami"),
        TEXT("chapter03_tawfiq"), TEXT("chapter03_amjad"), TEXT("chapter03_clinic"), TEXT("chapter03_needle"), TEXT("chapter03_bashir"),
        TEXT("chapter04_relay"), TEXT("chapter04_escort"), TEXT("chapter04_broadcast"), TEXT("chapter04_ferry"), TEXT("chapter04_charity"),
        TEXT("chapter05_roadblocks"), TEXT("chapter05_workers"), TEXT("chapter05_tawfiq_debt"), TEXT("chapter05_ledger"), TEXT("chapter05_amjad_truth"),
        TEXT("chapter06_river_road"), TEXT("chapter06_witness_run"), TEXT("chapter06_depot"), TEXT("chapter06_muntasir"), TEXT("chapter06_dawn")
    };
    static const TCHAR* MissionTitles[] = {
        TEXT("A Radio Before Sunrise"), TEXT("Crates With No Seal"), TEXT("Bashir's Back Gate"), TEXT("Three Bundles, One Lie"), TEXT("The Driver Who Ran"),
        TEXT("Smoke Over the Market"), TEXT("The Nurse's Stamp"), TEXT("A Minibus Full of Rumors"), TEXT("The Night Convoy"), TEXT("Sami's Second Chance"),
        TEXT("A Favor for Tawfiq"), TEXT("Amjad's Missing Hours"), TEXT("The Clinic at Dusk"), TEXT("The Needle in the Static"), TEXT("No One Leaves Bashir Behind"),
        TEXT("The Silent Relay"), TEXT("Keep the Technician Moving"), TEXT("A Voice Without a Name"), TEXT("Paper on the Water"), TEXT("The Broker's Charity"),
        TEXT("A City of Roadblocks"), TEXT("Free the Depot Workers"), TEXT("Tawfiq's Debt"), TEXT("The Ledger Has Two Pages"), TEXT("Amjad Tells the Truth"),
        TEXT("Road to the River"), TEXT("The Witness Protection Run"), TEXT("Hold the Central Depot"), TEXT("The Man Behind the Manifest"), TEXT("Dawn of Al-Mirfa")
    };
    static const TCHAR* DialogueText[] = {
        TEXT("Hiba on the radio: Al-Mirfa, listen carefully. A clinic shipment disappeared before sunrise. Meet your friends and follow the seal."),
        TEXT("Tawfiq: These crates are empty, ya zalameh. Who signs a delivery for air?"),
        TEXT("Bashir: Somebody wants my ledger gone. Help me save it and I will show you the back road."),
        TEXT("Amjad: Three bundles, three clinic codes, but the paper says one truck. Something does not add up."),
        TEXT("Bashir: That pickup belongs to Sami. Catch him if you can, but find out why he is running."),
        TEXT("Hiba: Smoke is rising over the souq. Search the store before the evidence disappears."),
        TEXT("Salwa: This seal is fake. The boxes were relabeled after leaving the depot."),
        TEXT("Idris: Sit tight. Every tea stall has a different story, and one of them will match the route."),
        TEXT("Amjad: Follow the tire marks. The recorder may tell us who ordered the night convoy."),
        TEXT("Sami: I drove the truck because they threatened my family. I can testify if you keep them safe."),
        TEXT("Tawfiq: I found us a shortcut. Relax, brother. What could possibly go wrong?"),
        TEXT("Amjad: I once delivered a sealed package for Muntasir. I should have told you sooner."),
        TEXT("Salwa: The generator is failing. Protect the clinic while we move the medicine."),
        TEXT("Hiba: Someone is jamming the relay. The frequency list may expose who is listening."),
        TEXT("Bashir: They trapped me in my own workshop. Get me out and keep that ledger dry."),
        TEXT("Technician: The relay is almost back. Keep the street clear while I reconnect the line."),
        TEXT("Captain Nadir: Do not leave the Technician behind. Without his testimony, our case is weak."),
        TEXT("Hiba: We can broadcast verified evidence without naming the witness. Protect the studio."),
        TEXT("Mariam: I saw a case under the ferry landing. The men who left it were in a hurry."),
        TEXT("The Broker: We feed this city when others only talk. Strange how every accusation comes before proof."),
        TEXT("Idris: These roadblocks are fake. Somebody wants the clinic convoy to miss its window."),
        TEXT("Worker: We refused to change the cargo count. They locked us in the yard."),
        TEXT("Tawfiq: The shortcut was tracked. I got careless, and now the whole team is in danger."),
        TEXT("Bashir: The ledger has a clean tear. Find the carbon copy before they burn the records."),
        TEXT("Amjad: I did not know what was in that package. Here is the route and the vehicle log. Judge me by what I do now."),
        TEXT("Bashir: The pickup is ready. Idris will call the safe turns. Get that clinic truck through."),
        TEXT("Captain Nadir: Keep Sami and the Survivor moving. No heroics that put them at risk."),
        TEXT("Fawzi: The neighborhood is bringing water and bandages. Hold the depot and protect the evidence room."),
        TEXT("Muntasir: Paper is power. Without the missing page, you have suspicions, not a case."),
        TEXT("Hiba: Al-Mirfa, these records are verified. The city deserves the truth. Bring the workers home first.")
    };
    static const TCHAR* TaskTypes[] = {
        TEXT("talk"), TEXT("reach"), TEXT("talk"), TEXT("collect"), TEXT("defeat"),
        TEXT("evidence"), TEXT("talk"), TEXT("reach"), TEXT("evidence"), TEXT("talk"),
        TEXT("reach"), TEXT("talk"), TEXT("defeat"), TEXT("evidence"), TEXT("talk"),
        TEXT("talk"), TEXT("reach"), TEXT("reach"), TEXT("evidence"), TEXT("talk"),
        TEXT("reach"), TEXT("defeat"), TEXT("talk"), TEXT("evidence"), TEXT("talk"),
        TEXT("defeat"), TEXT("reach"), TEXT("defeat"), TEXT("talk"), TEXT("talk")
    };
    static const TCHAR* TaskTargets[] = {
        TEXT("hiba"), TEXT(""), TEXT("bashir"), TEXT(""), TEXT(""),
        TEXT(""), TEXT("salwa"), TEXT(""), TEXT(""), TEXT("sami"),
        TEXT(""), TEXT("amjad"), TEXT(""), TEXT(""), TEXT("bashir"),
        TEXT("technician"), TEXT(""), TEXT(""), TEXT(""), TEXT("the_broker"),
        TEXT(""), TEXT(""), TEXT("tawfiq"), TEXT(""), TEXT("amjad"),
        TEXT(""), TEXT(""), TEXT(""), TEXT("muntasir"), TEXT("the_broker")
    };
    static const int32 RequiredCounts[] = {
        1,0,1,3,2, 1,1,0,1,1, 0,1,3,1,1, 1,0,0,1,1, 0,4,1,1,1, 4,0,5,1,1
    };
    static const TCHAR* ObjectivesText[] = {
        TEXT("Meet Tawfiq and Amjad, then listen to Hiba's report"), TEXT("Secure the plaza and inspect the empty crates"),
        TEXT("Reach Bashir's workshop and save his ledger"), TEXT("Recover three supply bundles and compare their tags"),
        TEXT("Intercept the fleeing driver and recover his delivery token"),
        TEXT("Search the burned market store"), TEXT("Show Salwa the counterfeit medical seal"),
        TEXT("Ride with Idris and visit the witness stops"), TEXT("Recover the night convoy recorder"),
        TEXT("Speak to Sami and get him safely to Captain Nadir"),
        TEXT("Reach the pickup and regroup after the shortcut"), TEXT("Speak to Amjad about the missing hours"),
        TEXT("Protect the clinic supply route"), TEXT("Recover the coded radio frequency list"),
        TEXT("Reach Bashir and rescue him from the workshop"),
        TEXT("Speak to the Technician and restore the relay"), TEXT("Escort the Technician to the broadcast room"),
        TEXT("Reach Hiba's studio and protect the verified broadcast"), TEXT("Recover the waterproof case at the ferry landing"),
        TEXT("Reach the charity event and identify Muntasir's handoff"),
        TEXT("Reopen the safe route through the false roadblocks"), TEXT("Defeat the guards and free the depot workers"),
        TEXT("Find Tawfiq and bring him back safely"), TEXT("Recover the missing ledger page"),
        TEXT("Speak to Amjad and secure the vehicle logbook"), TEXT("Clear the river road for the clinic truck"),
        TEXT("Move Sami and the Survivor to the safehouse"), TEXT("Defend the central depot and its evidence room"),
        TEXT("Confront Muntasir and recover the missing ledger page"), TEXT("Confront the Broker and complete the final broadcast")
    };
}

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
    if (MissionId == TEXT("campaign_complete")) {
        bCampaignActive = false; CampaignMissionIndex = 30; CurrentMissionId = MissionId;
        CurrentMissionTitle = FText::FromString(TEXT("Dawn of Al-Mirfa — Campaign Complete"));
        CurrentObjectives.Reset(); return true;
    }
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(MissionIds); ++Index) {
        if (MissionId == MissionIds[Index]) { bCampaignActive = true; StartCampaignMission(Index); return true; }
    }
    return false;
}
void UDawsMissionSubsystem::StartCampaignMission(int32 MissionIndex)
{
    if (MissionIndex < 0 || MissionIndex >= UE_ARRAY_COUNT(MissionIds)) {
        bCampaignActive = false; CampaignMissionIndex = 30;
        CurrentMissionId = TEXT("campaign_complete");
        CurrentMissionTitle = FText::FromString(TEXT("Dawn of Al-Mirfa — Campaign Complete"));
        CurrentObjectives.Reset(); CurrentTaskType = TEXT("none"); CurrentTaskTargetId.Reset();
        CurrentTaskProgress = 0; RequiredTaskProgress = 0; return;
    }
    CampaignMissionIndex = MissionIndex;
    CurrentDialogue = FText::FromString(DialogueText[MissionIndex]);
    CurrentTaskType = TaskTypes[MissionIndex];
    CurrentTaskTargetId = TaskTargets[MissionIndex];
    CurrentTaskProgress = 0;
    RequiredTaskProgress = RequiredCounts[MissionIndex];
    FDawsMissionObjective Objective;
    Objective.Id = FString(MissionIds[MissionIndex]) + TEXT("_reach");
    Objective.Description = FText::FromString(ObjectivesText[MissionIndex]);
    Objective.bCompleted = false;
    TArray<FDawsMissionObjective> Objectives; Objectives.Add(Objective);
    StartMission(MissionIds[MissionIndex], FText::FromString(MissionTitles[MissionIndex]), Objectives);
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
    for (FDawsMissionObjective& Objective : CurrentObjectives) {
        if (Objective.Id == ObjectiveId && !Objective.bCompleted) {
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
    for (const FDawsMissionObjective& Objective : CurrentObjectives) if (!Objective.bCompleted) return false;
    return true;
}
