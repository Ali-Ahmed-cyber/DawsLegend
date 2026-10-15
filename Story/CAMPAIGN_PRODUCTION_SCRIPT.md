# DAWS LEGEND — Campaign Production Script

## Campaign promise
A cinematic third-person action story grounded in an original Sudanese city setting. The hero and his two closest friends become involved in a supply theft and a conspiracy exploiting a city-wide emergency. The story is about loyalty, courage, and protecting ordinary people. This document is the production source of truth; the final likenesses of the three real main characters are deliberately deferred.

## Chapter 1 — The First Signal
**Playable spaces:** plaza, market street, side alley, improvised aid station.  
**Opening:** a radio report warns that emergency supplies are disappearing while residents are being redirected away from safe routes. The hero arrives at the plaza to help.  
**Mission 1: Secure the Plaza**
- Reach the plaza marker.
- Defeat or evade the first hostile patrol.
- Speak to Hiba, the community radio presenter.
- Outcome: Hiba gives the player a frequency and points toward the market.

**Mission 2: The Empty Storehouse**
- Follow the marked street.
- Search three supply crates.
- Recover a delivery ledger.
- Survive an ambush and return to the aid station.
- Outcome: the ledger reveals the same broker has intercepted several shipments.

**Mission 3: A Mechanic's Warning**
- Meet Bashir at his workshop.
- Restore power to a radio relay by finding two fuses.
- Defend the workshop while Bashir finishes repairs.
- Outcome: the relay records a coded message about a night transfer.

**Chapter finale: The Night Convoy**
- Move through a market route using cover.
- Disable the convoy's lead vehicle.
- Rescue a trapped driver; do not harm civilians.
- Escape to the safehouse before reinforcements arrive.
- Cliffhanger: the ledger bears a mark associated with a respected local contractor.

## Chapter 2 — Smoke Over the Market
**Spaces:** market rooftops, narrow lanes, warehouse interiors.
- Mission 1: investigate a burned-out storage room and photograph evidence.
- Mission 2: track the stolen cargo without raising the alarm; stealth routes reward careful play.
- Mission 3: defend the market clinic while Fawzi moves civilians to safety.
- Finale: intercept a courier, choose whether to trade evidence for a hostage's location. The choice affects later dialogue and the resources available, never the core identity of the three heroes.

## Chapter 3 — The Silent Relay
**Spaces:** radio station, power substation, outskirts checkpoint.
- Mission 1: restore the community relay and reconnect the radio network.
- Mission 2: escort a technician through an active threat zone.
- Mission 3: Captain Nadir requests evidence; the player must compare the ledger with the relay recording.
- Finale: enter the relay building, disable a jammer, and broadcast the verified evidence. A false clue reveals that the Broker has an inside source.

## Chapter 4 — The Broker's Route
**Spaces:** industrial yard, roadblock, river-side loading area.
- Mission 1: scout three entry points and choose stealth or direct approach.
- Mission 2: disable the cargo scanners and free detained workers.
- Mission 3: protect the mechanic's vehicle during a timed escape.
- Finale: confront the Broker's field commander. The commander escapes but drops proof linking the scheme to a network of shell companies.

## Chapter 5 — Dawn of the City
**Spaces:** city rooftops, central depot, public square.
- Mission 1: gather the last two pieces of evidence from optional side routes.
- Mission 2: coordinate Hiba's broadcast while Bashir keeps the relay online.
- Mission 3: hold the depot until residents and investigators arrive.
- Finale: face the Broker, disable the supply-control system, and choose whether to publish all evidence immediately or hand it to Captain Nadir for verified release.
- Epilogue: the city receives aid; character-specific final scenes are staged only after the three real character likenesses are imported.

## Supporting cast and dialogue beats
- **Hiba:** practical, calm under pressure; radio lines are short and actionable, never exposition dumps.
- **Bashir:** mechanic and friend; dry humour under stress; explains equipment and repair objectives.
- **Captain Nadir:** investigator; insists on verified evidence and protects witnesses.
- **Fawzi:** shopkeeper; knows the local routes and helps civilians.
- **The Broker:** antagonist who presents himself as a benefactor while controlling the supply chain.

## Implementation and acceptance checklist
- Every mission needs an objective state, completion condition, fail/retry path, checkpoint and short debrief.
- Civilians must be protected; hostile AI must not target non-combatants.
- Include subtitles for every spoken line, Arabic/English text localization, remappable controls, sensitivity, volume sliders, accessibility contrast, and a pause menu.
- All story progress must persist in save slots. Optional evidence and choices persist separately from current health/ammo.
- Final character photo import is the last character-specific task. No substitute likenesses.
- This is the campaign source script; in-engine levels, cinematics, voice acting and all acceptance tests must still be implemented and verified before release.
