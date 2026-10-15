# DAWS LEGEND

**Engine:** Unreal Engine 5.4  
**Genre:** Third-person story action / exploration  
**Production rule:** Build the complete game systems in the project; the three main character photos are the final character-specific additions.

## Implemented in the project source
- UE5 C++ runtime module, game mode, player controller setup and custom HUD.
- Third-person character with a white-kit silhouette, shoulder trim, chest crest, backpack, belt, boots, animated limbs, plus movement, jump, sprint, aim zoom, weapon trace fire, magazine and reload logic.
- Player and enemy health components with damage/death events.
- Runtime-generated city environment with an extended road, building blocks, cover, streetlights, market stalls, palm trees, rooftop tanks, cargo containers, the final depot district, a broadcast mast and mission beacons.
- A title screen and start flow; a 15-step sequential campaign across five chapters; mission-specific supply/evidence/hostile-count and named-NPC conversation tasks; interactive mission markers; story dialogue subtitles; mission objective state; save/load checkpoints; task-progress autosaves and collected-pickup persistence that avoid mission softlocks after death or restart; and persistent options.
- Four enemy archetypes with different health, movement speed and damage; mission-gated combat; ranged pressure, close-range attacks, death handling, and checkpoint respawn.
- Live objective waypoint distance/direction, named district labels, health/ammo/vehicle durability bars, and a restartable campaign world.
- Arcade driving with enter/exit interaction and a chase camera.
- Eight original supporting characters plus Tawfiq and Amjad as persistent yellow-kit and black-kit companions with follow movement and contextual dialogue; campaign story bible and character roster.
- Performance-oriented renderer defaults and keyboard/mouse bindings.

## Main menu and controls
- Enter: start a fresh campaign from the title screen (clears the previous checkpoint)
- F2: continue from the latest saved checkpoint
- W/A/S/D: move
- Mouse: camera
- Space: jump
- Left Shift: sprint
- Right mouse / gamepad left trigger: aim zoom
- Left mouse / gamepad right trigger: fire
- R: reload
- E: interact with mission markers, story characters or vehicles
- In a vehicle: W/S accelerate/reverse, A/D steer, E exit
- F5: save checkpoint
- F9: load checkpoint
- F10: restart the campaign after the final mission
- Esc: pause/resume
- F1: options (mouse sensitivity, camera field of view, subtitles, low/medium/high graphics)
- Arrow keys: navigate and change options
- Gamepad: left stick move/steer, right stick camera, left trigger aim, right trigger fire, face buttons interact/jump/reload, Y continues or restarts when relevant, left shoulder sprint

## Final character-photo rule
- HERO / PLAYER: user photo wearing the white Real Madrid shirt.
- TAWFIQ: user photo wearing yellow.
- AMJAD: user photo wearing black.

These three photos remain deferred until the rest of the project is ready. Supporting characters are original fictional characters, not substitutions for the three main people.

## Validation status
The files are being authored directly on the ue5-clean-rebuild branch. **An Unreal Engine compile and in-engine play test have not yet been run**, so no successful build is claimed. Unreal Editor/Build Tools are required for the real compile, asset import, animation, audio, lighting, level polish and final packaging. The next validation pass must compile against the selected UE version and fix every compiler/runtime error before calling the project release-ready.

## Open the project
Open DAWSLegend.uproject with Unreal Engine 5.4 or a compatible UE5 installation. If the editor asks to rebuild the C++ module, use the matching installed toolchain; review any build errors before launching.
