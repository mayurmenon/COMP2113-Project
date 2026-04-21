# Loopbreak: HKU After Hours

Loopbreak is a text-based campus stealth game set in a repeating school day.

You begin each run in a dorm room at `08:00`. To break the loop, you need to move around campus, find the clue in the library, work out the route through the admin area, take the sealed folder from the archive, and escape through the rooftop before time or suspicion forces a reset.

## Game Flow

Each action takes time. As you move deeper into restricted areas, your suspicion rises. If the day ends or suspicion reaches `100`, the loop resets and you return to the start of the day.

Important progress is kept between loops:

- library clue
- admin route knowledge
- loop count

The folder does not carry over, so each successful run still needs a clean final route.

## Current Prototype Features

- main menu and difficulty selection
- 11 connected campus rooms
- text-based movement and room exploration
- suspicion meter and hiding system
- loop reset system
- journal and objective display
- save/load support
- simple storybook-style terminal UI

## Main Commands

- `look` shows the current room again
- `move <room>` moves to a connected room
- `search` checks the current room for progress
- `hide` lowers suspicion for a short time
- `wait <minutes>` passes time
- `status` shows time, suspicion, and basic state
- `map` shows the campus layout
- `journal` shows saved clues
- `objective` shows the main goals
- `save` / `load` stores or restores the current run

## Time System

Time is stored as total minutes since midnight.

Examples:

- `480` = `08:00`
- `540` = `09:00`
- `1200` = `20:00`

Current action costs:

- `move` = `10` minutes
- `search` = `8` minutes
- `hide` = `5` minutes
- `wait` = the amount entered

## Campus Areas

The current map includes:

- dorm
- corridor
- lecture hall
- library
- commons
- lab
- faculty corridor
- admin office
- archive
- bridge
- rooftop

## Build And Run

Linux / academy server:

```bash
make
./loopbreak
```

Windows PowerShell:

```powershell
g++ -pedantic-errors -Wall -Wextra -std=c++11 -Iinclude main.cpp src/*.cpp -o loopbreak.exe
.\loopbreak.exe
```

## Notes

- non-standard libraries used: none
- the project is written in multiple source and header files
- current save file name: `savegame.txt`
