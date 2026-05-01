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

| Requirement | How Loopbreak meets it |
| --- | --- |
| Random game events | `include/event.h` and `src/event.cpp` define events such as rain, inspections, power dips, crowds, and staff meetings. These can trigger during gameplay and change route pressure, suspicion, or access conditions. |
| Data structures for storing data | Rooms, routes, items, journal entries, events, inventory, player state, and progression flags are stored with classes, structs, enums, `vector`, and `map`. |
| Dynamic memory management | The active event list is owned through `unique_ptr<vector<game_event> >` in `include/game.h` and initialized in `src/game.cpp`, so dynamically allocated state is cleaned up automatically. |
| File input/output | The save system in `include/save.h` and `src/save.cpp` writes and reads `savegame.txt`, including difficulty, time, room, suspicion, inventory, progress flags, run statistics, and event state. |
| Multiple source files | `main.cpp` starts the game, headers are kept in `include/`, implementations are kept in `src/`, and the `Makefile` builds the project on the academy server. |
| Multiple difficulty levels | Easy, Normal, and Hard change the time limit, event pressure, and amount of guidance shown to the player. |
| In-code documentation and style | Header files document what each function does, what inputs it expects, and what output or result it provides. The source code is organized with readable naming and indentation. |
| Terminal-based game requirement | The interface uses standard console output only, with color-coded headings, a status sidebar, room panels, and hand-written ASCII art. No graphics or non-standard UI library is required. |
| Save/load robustness | Saving is available from the start of the game, loading resumes without replaying the tutorial, and malformed values are handled defensively instead of crashing. |
| README submission details | This README includes team members, game description, implemented features, non-standard library information, and compilation/execution instructions. |

## Non-Standard Libraries

None. The project uses only standard C++ libraries and should compile without downloading or installing extra packages.

## Save And Load Notes

- Save file name: `savegame.txt`
- `save` can be used before the first clue or at any later point in gameplay.
- `load` resumes at the saved room and state without replaying the tutorial.
- The save file is local runtime data and is ignored by Git.
