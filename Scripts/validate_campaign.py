from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "Source/DAWSLegend"

def read(relative):
    return (ROOT / relative).read_text(encoding="utf-8")

def array_lengths(text):
    found = {}
    pattern = re.compile(r"(?:static\s+)?const\s+(?:TCHAR\*|int32)\s+(\w+)\[(?:\d*)\]\s*=\s*\{(.*?)\};", re.S)
    for name, body in pattern.findall(text):
        if "TEXT(" in body:
            found.setdefault(name, []).append(len(re.findall(r"TEXT\(", body)))
        else:
            found.setdefault(name, []).append(len([item for item in body.split(",") if item.strip()]))
    return found

mission = read("Source/DAWSLegend/Private/DawsMissionSubsystem.cpp")
for name in ("Ids", "Titles", "DialogueText", "TaskTypes", "TaskTargets", "RequiredCounts", "ObjectivesText"):
    lengths = array_lengths(mission).get(name, [])
    assert lengths and all(size == 15 for size in lengths), f"{name} campaign array must contain 15 entries: {lengths}"

trigger = read("Source/DAWSLegend/Private/DawsMissionTrigger.cpp")
game_mode = read("Source/DAWSLegend/Private/DawsGameMode.cpp")
assert any(size == 15 for size in array_lengths(trigger).get("MissionOrder", [])), "MissionOrder must contain 15 entries"
assert any(size == 15 for size in array_lengths(game_mode).get("MissionNames", [])), "MissionNames must contain 15 entries"

for path in list(SOURCE.rglob("*.cpp")) + list(SOURCE.rglob("*.h")):
    text = path.read_text(encoding="utf-8")
    assert "\\n    for (" not in text, f"Escaped newline may disable code in {path}"

player = read("Source/DAWSLegend/Private/DawsPlayerCharacter.cpp")
enemy = read("Source/DAWSLegend/Private/DawsEnemyCharacter.cpp")
npc = read("Source/DAWSLegend/Private/DawsStoryNPC.cpp")
game_mode = read("Source/DAWSLegend/Private/DawsGameMode.cpp")
assert "StoryNPCs[10]" in game_mode and 'TEXT("tawfiq")' in game_mode and 'TEXT("amjad")' in game_mode, "Tawfiq and Amjad must be present as replaceable final-likeness companions"
hud = read("Source/DAWSLegend/Private/DawsHUD.cpp")
assert 'BindAction("Jump", IE_Pressed' in player and "StartJump" in player, "Jump input must be bound"
assert 'GetCurrentTaskType() != TEXT("defeat")' in enemy, "Enemy AI must respect combat mission gating"
assert "ReceiveWeaponDamage" in player and "ReceiveWeaponDamage" in enemy, "Enemy weapon damage API must be connected"
assert "VInterpTo" in npc and "chapter03_escort" in npc, "Technician escort behavior must exist"
assert "ActiveMission == TEXT(\"chapter05_finale\")" in npc and "NPCId == FName(TEXT(\"tawfiq\"))" in npc, "Main companions need contextual campaign dialogue"
assert "bIsMainCompanion" in npc and "SideOffset" in npc, "Tawfiq and Amjad should accompany the player during the campaign"
assert "OBJECTIVE WAYPOINT" in hud, "Objective waypoint HUD must exist"
pickup_visuals = read("Source/DAWSLegend/Private/DawsPickup.cpp")
assert "AccentVisual" in pickup_visuals and "SecondaryAccentVisual" in pickup_visuals, "Readable pickup silhouettes are missing"
enemy_cpp = read("Source/DAWSLegend/Private/DawsEnemyCharacter.cpp")
npc_cpp = read("Source/DAWSLegend/Private/DawsStoryNPC.cpp")
controller = read("Source/DAWSLegend/Private/DawsPlayerController.cpp")
assert "FObjectFinder<UStaticMesh> Cylinder" in enemy_cpp and "Cylinder.Succeeded()" in enemy_cpp, "Enemy helmet mesh reference must be declared"
assert "FObjectFinder<UStaticMesh> Cylinder" in npc_cpp and "Cylinder.Succeeded()" in npc_cpp, "NPC accessory mesh reference must be declared"
assert "SetOverallScalabilityLevel" in controller and "GraphicsQuality" in controller, "Graphics quality setting must be connected"
save_code = read("Source/DAWSLegend/Private/DawsSaveSubsystem.cpp")
game_mode = read("Source/DAWSLegend/Private/DawsGameMode.cpp")
assert "CollectedPickupIds" in save_code and "IsPickupCollected" in save_code, "Collected mission pickups must persist"
assert 'task_pickup_%02d' in game_mode, "Mission pickups must have stable IDs"
restart_controller = controller[controller.index("void ADawsPlayerController::RestartCampaign()"):controller.index("void ADawsPlayerController::StartNewCampaign()")]
assert restart_controller.index("DeleteSave") < restart_controller.index("RestartCampaignWorld"), "Delete old save before rebuilding campaign pickups"
world_builder = read("Source/DAWSLegend/Private/DawsWorldBuilder.cpp")
assert "StallCenters" in world_builder and "AwningColors" in world_builder, "Market district dressing is missing"
for landmark in ("Clinic district landmark", "Silent relay landmark", "Industrial yard", "River-side route"):
    assert landmark in world_builder, f"Campaign district landmark missing: {landmark}"

save_code = read("Source/DAWSLegend/Private/DawsSaveSubsystem.cpp")
trigger_code = read("Source/DAWSLegend/Private/DawsMissionTrigger.cpp")
assert "bUseOverrideLocation ? OverrideLocation" in save_code, "Checkpoint override location must be supported"
assert "NextCheckpointLocation" in trigger_code and "bHasNextCheckpoint" in trigger_code, "Mission completion must checkpoint at the next marker"
assert "bFinalConfrontation" in enemy and "bIsSpecialTarget" in read("Source/DAWSLegend/Public/DawsEnemyCharacter.h"), "Final enforcer encounter must be gated to the designated boss"
assert "CAMPAIGN COMPLETE" in hud and "MISSION COMPLETE" in trigger_code, "Campaign completion feedback is missing"

print("Campaign integrity passed: 15 aligned missions, companions, final encounter, checkpoint routing, movement/combat, escort, save progress, waypoint HUD, graphics settings, and market props.")
