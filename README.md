# DAWS LEGEND

**Engine:** Unreal Engine 5.4  
**Genre:** Third-person story action / exploration  
**Production rule:** Build the complete game systems in the project; the three main character photos are the final character-specific additions.

## Implemented in the project source
- UE5 C++ runtime module, game mode, player controller setup and custom HUD.
- Third-person camera, movement, jump, sprint, weapon trace fire, magazine and reload logic.
- Player and enemy health components with damage/death events.
- Runtime-generated city-street environment with road, building blocks, cover and mission beacons.
- Three mission trigger areas, mission objective state, and a save/load subsystem.
- Enemy pursuit and close-range attack behavior.
- Story bible, secondary character roster and production notes.
- Performance-oriented renderer defaults and keyboard/mouse bindings.

## Controls
- W/A/S/D: move
- Mouse: camera
- Space: jump
- Left Shift: sprint
- Left mouse: fire
- R: reload

## Final character-photo rule
- DINRITO: user photo wearing the white Real Madrid shirt.
- TAWFIQ: user photo wearing yellow.
- AMJAD: user photo wearing black.

These three photos remain deferred until the rest of the project is ready. Supporting characters are original fictional characters, not substitutions for the three main people.

## Validation status
The files are being authored directly on the ue5-clean-rebuild branch. **An Unreal Engine compile and in-engine play test have not yet been run**, so no successful build is claimed. Unreal Editor/Build Tools are required for the real compile, asset import, animation, audio, lighting, level polish and final packaging. The next validation pass must compile against the selected UE version and fix every compiler/runtime error before calling the project release-ready.

## Open the project
Open DAWSLegend.uproject with Unreal Engine 5.4 or a compatible UE5 installation. If the editor asks to rebuild the C++ module, use the matching installed toolchain; review any build errors before launching.
