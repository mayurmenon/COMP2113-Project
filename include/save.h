#ifndef SAVE_H
#define SAVE_H
#include <string>
using namespace std;
class Game;
class SaveSystem {
public:
    // Writes the current run to a file. Input: game state and optional path. Output: success flag.
    static bool save(const Game& game, const string& path = "savegame.txt");
    // Loads a run from a file. Input: game reference and optional path. Output: success flag.
    static bool load(Game& game, const string& path = "savegame.txt");
};
#endif
