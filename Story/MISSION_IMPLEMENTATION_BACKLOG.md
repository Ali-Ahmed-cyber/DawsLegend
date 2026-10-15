# Mission implementation backlog

This is the implementation checklist for the full game, not a claim that each feature is complete.

## Core gameplay
- [x] Third-person character class, camera, walk/jump/sprint bindings.
- [x] Reusable health component with damage/heal events.
- [x] Mission objective data framework.
- [x] Local checkpoint save/load subsystem.
- [x] Generic interaction interface.
- [ ] Input rebinding and gamepad support.
- [x] Interaction trace, contextual prompt text and active mission markers.
- [x] Basic weapon trace combat, enemy health, pursuit/ranged/melee attacks and defeat handling.
- [x] Basic arcade vehicle possession, driving, enter/exit and chase camera.\n- [ ] Vehicle damage, repair and handling polish.
- [ ] Dialogue graph, choices and relationship flags.
- [x] Objective tracker, title screen, pause menu, sensitivity/FOV/subtitle options.\n- [x] 15-step campaign state with collection/evidence/defeat requirements and subtitle dialogue.
- [x] Mission markers, autosave checkpoints, manual save/load and death respawn.

## World and narrative
- [ ] Authored district maps and streaming boundaries.
- [ ] Navigation mesh, NPC schedules and pedestrian traffic.
- [x] Basic hostile AI pursuit and attack behavior.\n- [ ] Advanced perception, cover tactics and squad coordination.
- [x] Five-chapter campaign outline and 15-step runtime mission progression.
- [ ] Side missions and optional relationship choices.
- [ ] Audio, original music, environmental ambience and Arabic voice/subtitles.
- [ ] Original optimized environment assets, animations and final main-character likenesses.

- [ ] Vehicle possession, driving, enter/exit and damage.\n- [ ] Dialogue choices, relationship flags and branching outcomes.\n\n## Quality gates
- [ ] Compile in the chosen UE5 version.
- [ ] Playtest on target hardware and resolve crashes.
- [ ] Test saving/loading after mission transitions.
- [ ] Test controls and Arabic UI at common resolutions.
- [ ] Package a distributable build only after validation.
