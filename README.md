# Loopbreak: HKU After Hours

Loopbreak is a text-based stealth and puzzle game set around HKU at night. The player wakes up in Swire Hall, learns the campus route across repeated time loops, finds a sealed file in the Main Building records room, and reaches the rooftop before time or suspicion ends the run.

## Team Members

| Team member | UID | GitHub |
| --- | --- | --- |
| George Adon Abraham | 3036476267 | @adon-george |
| Gupta Aikagra | 3036474855 | @AikagraGupta |
| Gupta Akshat | 3036476011 | @Ak5hat-Gupta |
| Kalpally Pulapra Mayur Menon | 3036609187 | @mayurmenon |
| Mathur Shikhar | 3036476085 | @Adion114 |

## Quick Start

On the academy server:

```bash
make
./loopbreak
```

Alternative manual compilation:

```bash
g++ -pedantic-errors -Wall -Wextra -std=c++11 -Iinclude main.cpp src/*.cpp -o loopbreak
./loopbreak
```

Windows PowerShell:

```powershell
g++ -pedantic-errors -Wall -Wextra -std=c++11 -Iinclude main.cpp src/*.cpp -o loopbreak.exe
.\loopbreak.exe
```

No command-line arguments are needed.

## Game Features

- HKU-inspired connected map, including Swire Hall, University Street, Main Library, Chi Wah, Kadoorie, Main Building offices, records room, footbridge, clock tower, and rooftop.
- Time-loop structure where the player learns routes across repeated runs.
- Suspicion system with hiding, waiting, restricted rooms, and item-based risk control.
- Random campus events that can change pressure, timing, and route safety.
- Save and load system through `savegame.txt`, available from the start of the game.
- Easy, Normal, and Hard modes with different time limits, event pressure, and hint levels.
- High-contrast terminal UI with focused panels for current objective, location, paths, actions, and status.
- Wider 116-character room screen with separate columns for room details, current status, objective progress, items, events, and location-specific ASCII art.
- End-of-run achievement summary with escape rating, run statistics, rooms visited, peak suspicion, play time, and a play-again option.

## Main Commands

- `look`
- `move <room>`
- `search`
- `hide`
- `wait <minutes>`
- `use <item>`
- `inventory`
- `journal`
- `status`
- `map`
- `objective`
- `event`
- `save`
- `load`
- `help`
- `menu`
- `quit`

Room movement accepts both short ids and full names, for example `move admin` or `move Main Library`.

## How To Win

1. Search the Main Library to find the first clue.
2. Use the clue to map the Main Building records route.
3. Prepare tools or access items while keeping suspicion low.
4. Enter the records room and take the sealed file.
5. Reach the Main Building rooftop with the file before time runs out or suspicion reaches `100`.

## How The Project Requirements Are Met

Loopbreak includes random events through the event system in `include/event.h` and `src/event.cpp`. During gameplay, events such as rain, inspections, power dips, crowds, and staff meetings can trigger probabilistically and change route pressure, suspicion, or access conditions.

The game uses data structures throughout the codebase. Rooms, routes, items, journal entries, events, inventory, player state, and progression flags are stored using classes, structs, enums, `vector`, and `map`.

Dynamic memory management is used in the main game state. The active event list is owned through `unique_ptr<vector<game_event> >` in `include/game.h` and initialized in `src/game.cpp`, so dynamically allocated state is cleaned up automatically.

File input and output are handled by the save system in `include/save.h` and `src/save.cpp`. The save file stores the current difficulty, time, room, suspicion, inventory, progress flags, run statistics, and event state. Loading is written defensively so missing or malformed values do not crash the game.

The program is split across multiple files instead of being kept in one source file. `main.cpp` starts the game, headers live in `include/`, implementations live in `src/`, and the `Makefile` provides the build target used by the academy server.

The game has three difficulty levels: Easy, Normal, and Hard. These change the time limit, event pressure, and amount of guidance shown to the player.

The project includes in-code documentation. The header files describe what each function does, what inputs it expects, and what output or result it provides.

The interface stays fully terminal-based while still being easy to read. The latest room screen uses color-coded headings, a status sidebar, and hand-written ASCII art for campus locations; these are implemented with standard console output in `src/game.cpp` and declarations in `include/game.h`, so no graphics library is needed.

The final summary screen adds another gameplay feedback layer by using tracked player statistics such as moves, searches, items used, unique rooms visited, peak suspicion, and elapsed time. These values are saved and loaded with the rest of the run state so resumed games still produce accurate end-of-run results.

This README also covers the required submission information: team members, game description, implemented features, libraries used, and compilation/execution instructions.

## Non-Standard Libraries

None. The project uses only standard C++ libraries and should compile without downloading or installing extra packages.

## Save And Load Notes

- Save file name: `savegame.txt`
- `save` can be used before the first clue or at any later point in gameplay.
- `load` resumes at the saved room and state without replaying the tutorial.
- The save file is local runtime data and is ignored by Git.
