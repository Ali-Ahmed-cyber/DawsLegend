from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "Source/DAWSLegend"

def read(relative):
    return (ROOT / relative).read_text(encoding="utf-8")

def array_lengths(text):
    found = {}
    pattern = re.compile(r"static const (?:TCHAR\*|int32)\s+(\w+)\[\]\s*=\s*\{(.*?)\};", re.S)
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
hud = read("Source/DAWSLegend/Private/DawsHUD.cpp")
assert 'BindAction("Jump", IE_Pressed' in player and "StartJump" in player, "Jump input must be bound"
assert 'GetCurrentTaskType() != TEXT("defeat")' in enemy, "Enemy AI must respect combat mission gating"
assert "ReceiveWeaponDamage" in player and "ReceiveWeaponDamage" in enemy, "Enemy weapon damage API must be connected"
assert "VInterpTo" in npc and "chapter03_escort" in npc, "Technician escort behavior must exist"
assert "OBJECTIVE WAYPOINT" in hud, "Objective waypoint HUD must exist"
print("Campaign integrity passed: 15 aligned missions, required controls, combat gating, escort behavior and waypoint HUD.")
