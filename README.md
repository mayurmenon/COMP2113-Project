# Loopbreak: HKU After Hours

Loopbreak is a small text-based campus stealth game.

You are stuck in a repeating school day. The goal is to find the library clue, work out the admin route, take the sealed folder from the archive, and reach the rooftop before the loop resets.

## Current Prototype

- start menu and difficulty selection
- campus movement between connected rooms
- actions like `move`, `search`, `hide`, `wait`, `map`, `status`, `journal`, and `save`
- suspicion system and loop reset system
- simple save/load support

## How The Loop Works

The loop resets in two situations:

- the day runs out of time
- your suspicion reaches `100`

When the loop resets:

- time goes back to `08:00`
- you return to the starting room
- the folder is removed
- important progress like the library clue and admin route stays unlocked
- the loop counter goes up by 1

## How Time Works

The game stores time as total minutes since midnight.

Examples:

- `480` = `08:00`
- `540` = `09:00`
- `1200` = `20:00`

Actions add minutes to the day:

- `move` adds `10`
- `search` adds `8`
- `hide` adds `5`
- `wait` adds the number you enter

## Build And Run

```bash
make
./loopbreak
```

On Windows PowerShell:

```powershell
g++ -pedantic-errors -Wall -Wextra -std=c++11 -Iinclude main.cpp src/*.cpp -o loopbreak.exe
.\loopbreak.exe
```
