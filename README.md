# Eix

UE4-based game being developed in purpose to create soulslike mechanics.

## Warning

There is no **Content** folder or any assets in this repository for two particular reasons:
- I am using paid assets and shall not share them via open repository
- I am using large assets which simple git can not support. There's Git-LFS for this purpose, but for now I decided to go without it

There is a possibility in future that this repository will contain basic Content folder with free assets which would combine in a fully playable demo project.

## Combat system

There's melee combat system inspired by games from "souls" family, but also adding some new stuff. It's in active development process.

The main feature is combos (e.g. attacks combinations). Every primary (left click mouse or right bumper button) or secondary (right click mouse or right trigger button) attack can combine with the next primary or secondary attack.
Attacks can be chargeable. Chargeable attacks can also be a part of a combo, or start a combo.

Here are few examples at work.

- Consequent primary attacks:

<img src="docs/preview/preview_combo_A.gif" alt="drawing" width="720"/>

- Combining primary and secondary attacks:

<img src="docs/preview/preview_combo_B.gif" alt="drawing" width="720"/>

- Chargeable attack (which can also be part of a combo):

<img src="docs/preview/preview_chargeable.gif" alt="drawing" width="720"/>

- Attack after roll or during run (both could also start a combo):

<img src="docs/preview/preview_roll_run_attack.gif" alt="drawing" width="720"/>

## More things to go...

Current plan:
- Damage types with different severity impacts (for example, long heavy sword has greater severity than a short one) which will be fully data-oriented.
- Hit reactions along with stagger times (also configurable via data) and stuns.
- AI characters who will also implement the character combat behavior.
- Ranged weapons
