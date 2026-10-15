#include "DawsMissionSubsystem.h"
void UDawsMissionSubsystem::StartMission(FString MissionId, FText MissionTitle, const TArray<FDawsMissionObjective>& Objectives)
{
    CurrentMissionId = MoveTemp(MissionId);
    CurrentMissionTitle = MoveTemp(MissionTitle);
    CurrentObjectives = Objectives;
}
bool UDawsMissionSubsystem::CompleteObjective(FString ObjectiveId)
{
    for (FDawsMissionObjective& Objective : CurrentObjectives)
    {
        if (Objective.Id == ObjectiveId && !Objective.bCompleted)
        {
            Objective.bCompleted = true;
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