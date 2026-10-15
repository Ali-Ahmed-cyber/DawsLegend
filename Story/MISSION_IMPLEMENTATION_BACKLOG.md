# DAWS LEGEND — Production status

This checklist separates source systems already authored from the work that still requires Unreal Editor, real assets, and playtesting. A passing source audit is not an Unreal compile.

## Implemented in source

- [x] Third-person player, camera, movement, jump, sprint, aim zoom, weapon trace fire and reload.
- [x] Procedural white-kit hero silhouette with clothing details, facial features, weapon attachment and limb animation; final likeness is deferred.
- [x] Health and ammo systems, visual HUD bars, enemy health feedback and reticle.
- [x] Four enemy archetypes with different health, speed and damage; mission-gated AI; hostile pursuit, ranged/melee attacks, death handling and ammo/health drops.
- [x] Five-chapter, 15-mission sequential campaign with supply, evidence, combat, named-NPC and final confrontation objectives.
- [x] Story dialogue subtitles and original NPCs; Tawfiq and Amjad follow the hero as companions, while the technician follows during the escort mission and must remain nearby to finish it.
- [x] Runtime-generated city streets, buildings, cover, market stalls, street props, palm silhouettes, rooftop tanks, daylight and ambient fill.
- [x] Mission waypoint distance/direction, interactive markers, contextual prompts and task-progress display.
- [x] Arcade vehicles with enter/exit, driving, camera control, vehicle health and disabled state.
- [x] Local save/load checkpoints, task-progress autosaves that preserve the safe checkpoint position, stable IDs for collected mission pickups, ammo persistence and death respawn.
- [x] Title screen, fresh-campaign start, continue when a save exists, pause/options, persistent sensitivity/FOV/subtitle settings, gamepad mappings and campaign restart.
- [x] Low-to-mid scalability defaults and automated source/campaign consistency checks.

## Remaining production work

- [ ] Build the C++ project in the selected Unreal Engine 5.4 toolchain; fix all compile and runtime errors.
- [ ] Run a full 15-mission playthrough, including death during supply/evidence/hostile objectives, escort distance, vehicle entry/exit, checkpoint restore and campaign restart.
- [ ] Integrate the three final main-character references last and preserve exact mapping: hero in the white Real Madrid shirt, Tawfiq in yellow, Amjad in black.
- [ ] Replace procedural characters/props with polished optimized meshes, proper materials, skeletal rigs, full animation sets and final weapon/vehicle art.
- [ ] Import original sound effects, ambience, music and voice; connect audio controls.
- [ ] Add authored district art, collision/navigation polish, environmental storytelling, cinematics and final UI assets.
- [ ] Decide final Arabic/English localization and verify fonts, text shaping and common display resolutions.
- [ ] Profile on target hardware, tune scalability and memory, then package and verify a distributable build.

## Release gate

Do not call the project release-ready until the Unreal build succeeds and the full campaign is tested in-engine. The GitHub workflow currently validates project/source structure and campaign consistency; it does not compile Unreal C++.
