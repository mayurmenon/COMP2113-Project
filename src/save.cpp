#include "save.h"
#include <fstream>
#include "game.h"
#include "utils.h"
using namespace std;
namespace {
// Converts saved difficulty text back to the enum. Input: difficulty text. Output: difficulty enum.
Difficulty parseDifficulty(const string& text) {
    string lowered = Utils::lowerCopy(text);
    if (lowered == "easy") return Difficulty::Easy;
    if (lowered == "hard") return Difficulty::Hard;
    return Difficulty::Normal;
}
}
bool SaveSystem::save(const Game& game, const string& path) {
    ofstream out(path.c_str());
    if (!out) return false;
    out << "difficulty=" << game.difficultyName() << '\n';
    out << "loop=" << game.loopNumber() << '\n';
    out << "time=" << game.currentMinutes() << '\n';
    out << "room=" << game.player().room() << '\n';
    out << "suspicion=" << game.player().suspicion() << '\n';
    out << "library=" << (game.libraryClue() ? 1 : 0) << '\n';
    out << "admin=" << (game.adminRoute() ? 1 : 0) << '\n';
    out << "folder=" << (game.folder() ? 1 : 0) << '\n';
    out << "codefragments=" << game.codeFragmentsNeeded() << '\n';
    out << "hidestreak=" << game.player().hideStreak() << '\n';
    out << "moves=" << game.player().totalMoves() << '\n';
    out << "searches=" << game.player().totalSearches() << '\n';
    out << "itemsused=" << game.player().itemsUsed() << '\n';
    out << "inventory=" << game.player().inventoryAsString() << '\n';
    return true;
}
bool SaveSystem::load(Game& game, const string& path) {
    ifstream in(path.c_str());
    if (!in) return false;
    string difficulty = "Normal";
    int loopNumber = 1;
    int time = 480;
    string room = "dorm";
    int suspicion = 0;
    bool library = false;
    bool admin = false;
    bool folder = false;
    int codeFragments = 1;
    int hideStreak = 0;
    int moves = 0;
    int searches = 0;
    int itemsUsed = 0;
    string inventory = "";
    string line;
    while (getline(in, line)) {
        if (line.rfind("difficulty=", 0) == 0) difficulty = line.substr(11);
        else if (line.rfind("loop=", 0) == 0) loopNumber = stoi(line.substr(5));
        else if (line.rfind("time=", 0) == 0) time = stoi(line.substr(5));
        else if (line.rfind("room=", 0) == 0) room = line.substr(5);
        else if (line.rfind("suspicion=", 0) == 0) suspicion = stoi(line.substr(10));
        else if (line.rfind("library=", 0) == 0) library = line.substr(8) == "1";
        else if (line.rfind("admin=", 0) == 0) admin = line.substr(6) == "1";
        else if (line.rfind("folder=", 0) == 0) folder = line.substr(7) == "1";
        else if (line.rfind("codefragments=", 0) == 0) codeFragments = stoi(line.substr(14));
        else if (line.rfind("hidestreak=", 0) == 0) hideStreak = stoi(line.substr(11));
        else if (line.rfind("moves=", 0) == 0) moves = stoi(line.substr(6));
        else if (line.rfind("searches=", 0) == 0) searches = stoi(line.substr(9));
        else if (line.rfind("itemsused=", 0) == 0) itemsUsed = stoi(line.substr(10));
        else if (line.rfind("inventory=", 0) == 0) inventory = line.substr(10);
    }
    game.loadState(time, parseDifficulty(difficulty), room, suspicion, library, admin, folder, loopNumber, inventory, hideStreak, moves, searches, itemsUsed, codeFragments);
    return true;
}
