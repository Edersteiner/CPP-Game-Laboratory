# Battleships Buggy Lab — Answers

## Team
- Team name: 
- Partner A: Gabriel
- Partner B: Neo

---

## 1) Requirement → Code Map (spec verification)

Fill this table by reading `games/battleships/specification.md` and mapping requirements to code.

Requirement (from specification.md) | Pass/Fail/Unsure | Code location (file + class/function) | Notes (what it does)
---|---|---|---
Example: Turns alternate after each valid shot |  | `battleships_buggy/src/core/Game.cpp` → `Game::AdvanceTurn` | flips current player index
Example: Tracking board records shots |  | `battleships_buggy/src/core/Game.cpp` → `Game::ShootAtOpponent` | updates `Player::tracking`
Placement must be validated |  | `` → `` |
After player 1 places, player 2 must take hot-seat |  | `` → `` |
Playing phase starts only when both players have placed |  | `` → `` |
Game reports shot results |  | `` → `` |
Turns alternate until one player has sunk all opponent ships |  | `` → `` |
Template |  | `` → `` |

Add rows for all major requirements you verify.

---

## 2) Spec verification summary

Write Pass/Fail/Unsure for each area and explain *how you verified it* (test steps or observation).

- Setup phase:
- Placement rules:
- Input commands:
- Shot resolution:
- Tracking board:
- Win condition:

---

## 3) Bugs found (at least 6 total)

You must list at least:
- 3 functional bugs (incorrect behavior vs spec)
- 3 robustness issues (bad input handling, silent failures, crashes, etc.)

### Bug 1 functional
- Symptom: Second player never got to place ships
- Steps to reproduce (exact inputs): Player 1 places 5 ships
- Expected (spec): Let player 2 place ships
- Actual: Player 2 never got to place ships, proceded directly to attacking
- Suspected root cause (file/function): In game.cpp, setup if ready, never checked for the second player
- Fix approach: Check for the second player

### Bug 2 robustness
- Symptom: Could place vertical ships with their last segment in another
- Steps to reproduce (exact inputs): Place a ship with its last segment winding up in another
- Expected (spec): Not allowed to place
- Actual: Were allowed to place, ships overlapped
- Suspected root cause (file/function): board.cpp CanPlaceShip() had - 1 on ship length, line 41
- Fix approach: Remove - 1, line 41

### Bug 3 robustness
- Symptom: When shooting out of bounds, you still get a hit
- Steps to reproduce (exact inputs): Type shoot and any coordinate that is out of bounds
- Expected (spec): Get a sort of failiure to attack confirmation
- Actual: Get a hit instead
- Suspected root cause (file/function): Board.cpp, Line 112, we have Hit as our return if we fail to fire, and in enums we don't have a failiure enum in ShotResult
- Fix approach: Add Invalid to ShotResult, then return Invalid if we fail to fire, then let the player try again

### Bug 4 robustness
- Symptom: When placing a ship, entering anything but v or V, including nothing, defaults to horizontal
- Steps to reproduce (exact inputs): Enter anything but v or V
- Expected (spec): Give an invalid input warning, and make them type either v or h again
- Actual: Just defaults to horizontal if it is not v or V as an input
- Suspected root cause (file/function): Line 26, only checking if input is not empty and if there is a v or V
- Fix approach: Rewrite function to check for specifically v or h, either capitalized or uncapitalized

### Bug 5
- Symptom:
- Steps to reproduce (exact inputs):
- Expected (spec):
- Actual:
- Suspected root cause (file/function):
- Fix approach:

### Bug 6
- Symptom:
- Steps to reproduce (exact inputs):
- Expected (spec):
- Actual:
- Suspected root cause (file/function):
- Fix approach:

(Add more if you find more.)

---

## 4) Code smells found (at least 5)

A “smell” is not necessarily a bug, but a design/implementation choice that increases risk or cost.

### Smell 1
- What (where in code): Line 87, main.cpp, very unclear why a ship couldn't be placed
- Why risky: Confusing player on why a ship could not be placed at set position and orientation
- Better approach (short plan): Give more verbose feedback on why ship placement failed, instead of a bool we give a PlaceResult

### Smell 2
- What (where in code): 
- Why risky: 
- Better approach (short plan): 

### Smell 3
- What (where in code):
- Why risky:
- Better approach (short plan):

### Smell 4
- What (where in code):
- Why risky:
- Better approach (short plan):

### Smell 5
- What (where in code):
- Why risky:
- Better approach (short plan):

---

## 5) Good patterns found (at least 4)

Find at least 4 things the code does reasonably well.

### Good thing 1
- Where:
- Why good:
- How to reuse:

### Good thing 2
- Where:
- Why good:
- How to reuse:

### Good thing 3
- Where:
- Why good:
- How to reuse:

### Good thing 4
- Where:
- Why good:
- How to reuse:

---

## 6) Team bug-fix PR (pair programming)

- Branch name:
- PR link:
- Bugs fixed (list):
- Tests added/updated:
- How you verified the fixes:
- What you learned:

---

## 7) Team feature PR (pair programming)

- Feature implemented (from spec’s “Explicit Missing Features”):
- Branch name:
- PR link:

Mini-spec (write BEFORE coding):
- Behavior:
- Edge cases:
- How to verify (test or manual steps):

Verification performed:
- Tests/manual steps used:
- Result:

---

## 8) Individual feature PRs

### Partner A
- Feature:
- Branch:
- PR link:

Mini-spec:
- Behavior:
- Edge cases:
- How to verify:

Verification performed:
- Tests/manual steps used:
- Result:

### Partner B
- Feature:
- Branch:
- PR link:

Mini-spec:
- Behavior:
- Edge cases:
- How to verify:

Verification performed:
- Tests/manual steps used:
- Result:

---

## 9) Manual test script (if needed)

If you could not write tests for some behavior, write a reproducible manual test script.

Example format:
1) Start game
2) Place ships for both players with valid placements
3) Player 1 shoots A1 → expect: Miss/Hit/Sunk
4) Player 1 shoots A1 again → expect: AlreadyTried and turn should NOT change
