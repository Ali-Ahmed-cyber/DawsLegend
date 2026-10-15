# DAWS LEGEND — Sudanese Side-Mission Script Pack

These ten side stories expand Al-Mirfa beyond the main conspiracy. They are authored narrative designs, not yet runtime missions unless a matching objective, NPC, trigger and reward is implemented in C++.

## Side story design rules
- Every side story is rooted in ordinary people, not disposable background scenery.
- Each has a clear start, a short sequence of actions, a consequence, and a tangible reward.
- Side stories should take 3–8 minutes each once implemented, with no mandatory combat in every story.
- Rewards support the campaign: supplies, safe routes, witness trust, vehicle access or new dialogue. No pay-to-win systems.
- Sudanese Arabic should sound natural and local; jokes belong in safe moments and never undercut serious scenes.

## 1. Tea Before Trouble — Mariam's Cart
**Start:** Mariam finds her tea cart missing after a false roadblock clears the lane.
**Beats:** Ask nearby stall owners; follow a trail of spilled tea tins; recover the cart from a storage yard; decide whether to return a small envelope found beneath it to Mariam unopened.
**Dialogue tone:** Mariam: “يا ولدي، العدة دي ما بتجيب قروش ساكت؛ دي بتلمّ الحلة كلها في مكان واحد.”
**Payoff:** Mariam becomes a trusted neighborhood contact. Later she can point out a suspicious vehicle without naming an innocent person.
**Reward:** Small health/ammo resupply and one clue about Abu Sahl's fake road closures.

## 2. The Lost Football — One More Match
**Start:** A group of children cannot find their ball after a hurried evacuation.
**Beats:** Search the courtyard and alley; retrieve the ball from a roof or safe yard; return before sunset; play a short, non-combat passing challenge with Tawfiq and Amjad.
**Dialogue tone:** Tawfiq: “لو الكرة دي اتقطعت، أنا ما مسؤول من الزعل الحيحصل في الحلة!”
**Payoff:** The friends relax for once. After Act V, Amjad admits he kept his old delivery log because he was ashamed, not because he trusted Muntasir.
**Reward:** Companion dialogue and a small morale scene at the tea stall.

## 3. Last Minibus Home — Idris' Route
**Start:** Idris' minibus has a damaged wheel and residents are stranded before dark.
**Beats:** Find a spare part at Bashir's workshop; carry it back; protect Idris while he tests the vehicle; ride with him through three route markers.
**Dialogue tone:** Idris: “يا جماعة، الراكب المستعجل ما بخلّي الشارع يختصر براهو!”
**Payoff:** Idris marks a safe detour around one later roadblock.
**Reward:** Unlock a safer travel route and a minibus ride point.

## 4. Auntie's Medicine — No Gun Needed
**Start:** Salwa asks for a medicine package to reach an elderly resident while the main clinic road is blocked.
**Beats:** Collect the sealed package; choose a footpath or vehicle route; avoid dropping or losing the package; deliver it and wait for confirmation.
**Payoff:** The resident's family confirms the delivery and provides a small witness detail about a convoy time.
**Reward:** Health supplies and a verified timeline clue. Combat is never required.

## 5. The Workshop Bet — Bashir's Missing Part
**Start:** Bashir wagers that nobody can identify the right alternator from a pile of salvaged parts.
**Beats:** Inspect three marked parts; bring the correct one back; help test a repaired pickup; complete a short driving loop without wrecking it.
**Dialogue tone:** Bashir: “العربية دي لو اتكلمت، حتفضح نص الشغلانة في السوق!”
**Payoff:** Bashir trusts the hero with a repaired vehicle and explains why the ledger was hidden behind a false wall.
**Reward:** Vehicle repair/resilience and workshop dialogue.

## 6. Market Day — The Decoy Crew
**Start:** Fawzi notices strangers asking shopkeepers to close early, repeating the same suspicious wording.
**Beats:** Interview three vendors; compare descriptions; follow the decoy crew without attacking bystanders; stop their vehicle at a clear point and recover a forged closure notice.
**Payoff:** The notice links Abu Sahl to the false roadblocks. This supports the main story but does not replace its mandatory evidence.
**Reward:** Market vendors warn the player about ambushes in a later district.

## 7. The Old Photograph — A Family's Keepsake
**Start:** The Survivor finds an old family photograph among convoy belongings and fears the family was displaced.
**Beats:** Inspect the back for a neighborhood name; ask Mariam and Fawzi; carry the photograph to the correct home; listen to the family's account without pressuring them.
**Payoff:** A quiet human scene shows what the stolen routes have done to residents. Their testimony is optional context, not proof required for the main case.
**Reward:** Witness trust and a new safehouse marker.

## 8. A Radio for the Block — Hiba's Community Signal
**Start:** Hiba's neighborhood transmitter keeps cutting out when the district generator dips.
**Beats:** Retrieve a cable from Bashir; collect a battery from the market; install the parts at the community radio point; defend the equipment only if the optional hostile encounter is triggered.
**Dialogue tone:** Hiba: “ما حنبث إشاعة. يا خبر متأكد، يا نسكت لحدي ما نتأكد.”
**Payoff:** Hiba can share a short, verified update between later acts and remind the player to protect witness identities.
**Reward:** Radio updates and a resupply point.

## 9. The Missing Water Tank — Abu Sahl's Paper Trail
**Start:** Residents discover that a shared rooftop water tank has been sold on paper to a company that never visited the neighborhood.
**Beats:** Speak to the caretaker; compare the posted notice with a copied contract; follow a courier to a records office; recover a duplicate stamped receipt.
**Payoff:** This side story foreshadows the riverfront land grab and shows Abu Sahl's method: make a false document look official, then intimidate people who question it.
**Reward:** Extra evidence context and a later alternate dialogue line with Captain Nadir.

## 10. One More Match — The Friends' Promise
**Start:** After the depot crisis, Tawfiq asks the hero to meet Amjad at the neighborhood pitch.
**Beats:** Visit the pitch; choose a playful challenge; hear Tawfiq apologize without hiding behind jokes; let Amjad explain the delivery job in his own words.
**Payoff:** Resolves the friendship arc through a calm scene after the climax. The main ending remains available whether or not this optional story is played.
**Reward:** Final companion banter and a neighborhood epilogue scene.

## Implementation order
1. Add side-mission data definitions with stable IDs and save-safe progress.
2. Add one reusable side-mission interaction/marker system rather than hard-coding ten independent systems.
3. Implement the three non-combat deliveries first (Auntie's Medicine, Old Photograph, Tea Before Trouble).
4. Add route and vehicle stories, then evidence-oriented side stories.
5. Add automated checks for every side-mission ID, NPC target, objective count, reward ID and save/restore path.
6. Verify in Unreal Engine 5.4 before calling any side story playable.

## Continuity safeguards
- Optional evidence never contradicts the 30-mission main case.
- No side mission makes Tawfiq or Amjad secretly evil; their conflict is guilt, trust and repair.
- The Broker's charity remains a public image strategy, not proof that all charitable people are corrupt.
- Civilians must be able to refuse a conversation; the player never has to threaten a witness to finish the main campaign.
- The three real main-character likenesses remain reserved for the final art pass: hero in white Real Madrid shirt, Tawfiq in yellow, Amjad in black.
