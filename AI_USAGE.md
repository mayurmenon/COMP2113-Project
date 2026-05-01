# AI Usage Documentation

This project used AI as a support tool for polishing documentation, checking edge cases, and improving parts of the terminal user experience. The team reviewed the suggestions before applying them, and the final code and README were edited and tested by the team.

## Summary

AI was used in a limited way for:

- Improving README wording and Markdown formatting.
- Organizing team member and project requirement information.
- Suggesting cleaner terminal UI structure.
- Drafting this AI usage Markdown file in a concise format.
- Reviewing save/load edge cases.
- Creating test ideas for invalid commands, malformed saves, and replay flow.

## Representative AI Prompts And Responses

| Area | Short prompt used | Short AI response used |
| --- | --- | --- |
| README formatting | "Make the README easier for graders to read using better Markdown." | Use clear sections such as Quick Start, Game Features, Commands, Requirements, Libraries, and Save/Load Notes. Tables can make team and requirement information easier to scan. |
| Team member details | "Add team members, UIDs, and GitHub usernames neatly in the README." | Format the information as a table with columns for team member, UID, and GitHub username. |
| Project requirements | "Show how the project meets each course requirement clearly." | Create a requirement checklist/table mapping each requirement to the files or systems that satisfy it, such as events, data structures, dynamic memory, file I/O, and multiple difficulty levels. |
| Terminal UI polish | "The game screen feels cluttered. How can a text-based C++ game look cleaner?" | Split the screen into focused areas: current location, objective, status, available paths, command deck, and optional ASCII art. Use color-coded headings while staying fully terminal-based. |
| AI usage documentation | "Create a small AI_USAGE.md file tracking the main AI prompts and how they were used." | Keep it concise, include only representative prompts and responses, and explain that the team reviewed and tested the final work. |
| Save/load behavior | "Check save/load edge cases. Loading should resume the game without replaying the tutorial." | Save the current room, time, difficulty, suspicion, inventory, progress flags, event state, and run stats. On load, restore the state and keep the tutorial flag off. |
| End summary stats | "The win screen tracks peak suspicion, rooms visited, and real play time. What edge cases should be handled?" | Save and load summary-related data too, including peak suspicion, visited rooms, and elapsed seconds, so a resumed game still shows an accurate final summary. |
| Testing ideas | "Stress test the game for small edge cases a grader might try." | Test missing save files, malformed save values, invalid commands, negative wait times, early save/load, win after load, and play-again restart flow. |

## Notes

- No non-standard C++ libraries were added through AI suggestions.
- AI suggestions were used as guidance, not copied blindly.
- The final implementation was compiled and tested with strict warning flags and gameplay edge-case scripts.
