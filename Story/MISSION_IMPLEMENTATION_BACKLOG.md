# DAWS LEGEND — Production status

This checklist separates source systems already authored from the work that still requires Unreal Editor, real assets, and playtesting. A passing source audit is not an Unreal compile.

## Implemented in source

- [x] Third-person player, camera, movement, jump, sprint, aim zoom, weapon trace fire and reload.
- [x] Procedural white-kit hero silhouette with clothing details, facial features, weapon attachment and limb animation; final likeness is deferred.
- [x] Health and ammo systems, visual HUD bars, enemy health feedback, moving player/enemy shot tracers, hit-confirmation reticle, and final enforcer health bar.
- [x] Four enemy archetypes with different health, speed and damage; mission-gated AI; hostile pursuit, ranged/melee attacks, death handling and ammo/health drops.
- [x] Six-act, 30-mission campaign definitions and ordered mission markers are now authored in source; full playthrough verification is still required with supply, evidence, combat, named-NPC and final confrontation objectives.
- [x] Story dialogue subtitles and original NPCs; Tawfiq and Amjad follow the hero as companions, while the technician follows during the escort mission and must remain nearby to finish it. The 30-mission Sudanese campaign has matching runtime definitions, mission markers, and task targets; end-to-end playtesting is still required.
- [x] Runtime-generated city streets, buildings, cover, market stalls, street props, palm silhouettes, rooftop tanks, daylight and ambient fill.
- [x] Mission waypoint distance/direction, interactive markers, contextual prompts and task-progress display.
- [x] Arcade vehicles with enter/exit, driving, camera control, vehicle health and disabled state.
- [x] Local save/load checkpoints, task-progress autosaves that preserve the safe checkpoint position, stable IDs for collected mission pickups, ammo persistence and death respawn.
- [x] Title screen, fresh-campaign start, continue when a save exists, pause/options, persistent sensitivity/FOV/subtitle settings, conflict-free gamepad continue/restart routing, and campaign restart.
- [x] Low-to-mid scalability defaults, Windows Unreal build/package helper, and automated source/campaign consistency checks.

## Authored but not yet runtime-playable
- [x] Ten detailed optional Sudanese side-story scripts with clear beats, rewards, continuity safeguards and a proposed implementation order in `Story/SIDE_MISSION_SCRIPT_PACK.md`.
- [ ] Implement side-story state, start/finish markers, objectives, rewards and save-safe progress in C++ before describing these optional stories as playable.

## Remaining production work

- [ ] Build the C++ project in the selected Unreal Engine 5.4 toolchain; fix all compile and runtime errors.
- [ ] Run a full 30-mission playthrough, including death during supply/evidence/hostile objectives, escort distance, vehicle entry/exit, checkpoint restore and campaign restart.
- [ ] Integrate the three final main-character references last and preserve exact mapping: hero in the white Real Madrid shirt, Tawfiq in yellow, Amjad in black.
- [ ] Replace procedural characters/props with polished optimized meshes, proper materials, skeletal rigs, full animation sets and final weapon/vehicle art.
- [ ] Import original sound effects, ambience, music and voice; connect audio controls.
- [ ] Add authored district art, collision/navigation polish, environmental storytelling, cinematics and final UI assets.
- [ ] Decide final Arabic/English localization and verify fonts, text shaping and common display resolutions.
- [ ] Profile on target hardware, tune scalability and memory, then package and verify a distributable build.

## Release gate

Do not call the project release-ready until the Unreal build succeeds and the full campaign is tested in-engine. The GitHub workflow currently validates project/source structure and campaign consistency; it does not compile Unreal C++.
