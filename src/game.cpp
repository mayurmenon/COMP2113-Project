#include "game.h"
#include <cctype>
#include <iostream>
#include <sstream>
#include "save.h"
#include "utils.h"
#include "item.h"
#include <cstdlib>
#include <ctime>
using namespace std;
namespace {
// Stores the terminal color codes used by the UI. Input: none. Output: none.
const string resetColor = "\033[0m";
const string mainColor = "\033[38;5;120m";
const string brightColor = "\033[1;92m";
const string dimColor = "\033[2;32m";
// Repeats one character many times. Input: character and count. Output: repeated text.
string repeatChar(char letter, int count) { return count > 0 ? string(count, letter) : ""; }
// Normalizes text for command matching. Input: raw text. Output: trimmed lowercase text.
string lowerText(const string& text) { return Utils::lowerCopy(Utils::trim(text)); }
// Gets the part after the command word. Input: raw command. Output: argument text.
string argumentOf(const string& text) {
    size_t firstSpace = text.find(' ');
    return firstSpace == string::npos ? "" : Utils::trim(text.substr(firstSpace + 1));
}
// Reads a positive number from text. Input: text and fallback value. Output: parsed minutes.
int parseMinutes(const string& text, int fallback) {
    try {
        int value = stoi(text);
        return value > 0 ? value : fallback;
    } catch (...) {
        return fallback;
    }
}
// Converts a bool into yes or no. Input: bool value. Output: yes/no text.
string yesNo(bool value) { return value ? "yes" : "no"; }
// Splits a block of text into lines. Input: text block. Output: vector of lines.
vector<string> splitLines(const string& text) {
    vector<string> lines;
    istringstream in(text);
    string line;
    while (getline(in, line)) lines.push_back(line);
    if (lines.empty()) lines.push_back("");
    return lines;
}
}
Game::Game()
    : difficulty_(Difficulty::Normal),
      currentMinutes_(480),
      loopNumber_(1),
      running_(false),
      won_(false),
      quit_(false),
      libraryClue_(false),
      adminRoute_(false),
      folder_(false) {
    srand(time(nullptr));
    itemTable_ = createItemTable();
    codeFragmentsNeeded_ = (rand() % 3) + 1;
}
void Game::start() {
    while (!quit_) {
        clearScreen();
        showTitle();
        showMenu();
        cout << "\n" << prompt("menu");
        string choice;
        getline(cin, choice);
        choice = lowerText(choice);
        if (choice == "1" || choice == "new") {
            chooseDifficulty();
            resetLoop(false, "");
            playLoop();
        } else if (choice == "2" || choice == "load") {
            if (SaveSystem::load(*this)) playLoop();
            else {
                drawBox("load", vector<string>(1, "savegame.txt was not found"));
                pause();
            }
        } else if (choice == "3" || choice == "help") {
            showAbout();
            pause();
        } else if (choice == "4" || choice == "quit" || choice == "exit") {
            quit_ = true;
        }
    }
}
void Game::loadState(int minutes, Difficulty difficulty, const string& roomId, int suspicion, bool libraryClue, bool adminRoute, bool folder, int loopNumber, const string& inventoryData, int hideStreak, int totalMoves, int totalSearches, int itemsUsed, int codeFragmentsNeeded) {
    difficulty_ = difficulty;
    currentMinutes_ = minutes;
    loopNumber_ = loopNumber;
    libraryClue_ = libraryClue;
    adminRoute_ = adminRoute;
    folder_ = folder;
    running_ = true;
    won_ = false;
    player_.reset();
    codeFragmentsNeeded_ = codeFragmentsNeeded;
    if (map_.get(roomId)) player_.moveTo(roomId);
    player_.setSuspicion(suspicion);
    player_.loadInventoryFromString(inventoryData);
    for (int i = 0; i < hideStreak; i++) player_.incrementHideStreak();
    for (int i = 0; i < totalMoves; i++) player_.incrementMoves();
    for (int i = 0; i < totalSearches; i++) player_.incrementSearches();
    for (int i = 0; i < itemsUsed; i++) player_.incrementItemsUsed();
}
int Game::currentMinutes() const { return currentMinutes_; }
int Game::loopNumber() const { return loopNumber_; }
Difficulty Game::difficulty() const { return difficulty_; }
string Game::difficultyName() const {
    if (difficulty_ == Difficulty::Easy) return "Easy";
    if (difficulty_ == Difficulty::Hard) return "Hard";
    return "Normal";
}
const Player& Game::player() const { return player_; }
bool Game::libraryClue() const { return libraryClue_; }
bool Game::adminRoute() const { return adminRoute_; }
bool Game::folder() const { return folder_; }
int Game::codeFragmentsNeeded() const { return codeFragmentsNeeded_; }
void Game::resetLoop(bool keepProgress, const string& reason) {
    if (keepProgress) loopNumber_++;
    else {
        loopNumber_ = 1;
        libraryClue_ = false;
        adminRoute_ = false;
    }
    folder_ = false;
    currentMinutes_ = 480;
    player_.reset();
    codeFragmentsNeeded_ = (rand() % 3) + 1;
    running_ = true;
    won_ = false;
    if (!reason.empty()) {
        clearScreen();
        drawBox("loop reset", vector<string>{reason, "time returns to 08:00", "loop " + to_string(loopNumber_)});
    }
}
int Game::dayEnd() const {
    if (difficulty_ == Difficulty::Easy) return 1260;
    if (difficulty_ == Difficulty::Hard) return 1140;
    return 1200;
}
void Game::clearScreen() const { cout << resetColor << "\033[40m" << mainColor << "\033[2J\033[H"; }
void Game::drawBox(const string& title, const vector<string>& lines) const {
    int width = 70;
    if ((int) title.size() + 8 > width) width = (int) title.size() + 8;
    for (int index = 0; index < (int) lines.size(); index++) if ((int) lines[index].size() + 4 > width) width = (int) lines[index].size() + 4;
    cout << "/" << repeatChar('=', width - 2) << "\\\n";
    string header = " " + title + " ";
    int left = (width - 2 - (int) header.size()) / 2;
    int right = width - 2 - (int) header.size() - left;
    cout << "|" << repeatChar(' ', left) << brightColor << header << mainColor << repeatChar(' ', right) << "|\n";
    cout << "|" << repeatChar('-', width - 2) << "|\n";
    cout << "|" << repeatChar(' ', width - 2) << "|\n";
    for (int index = 0; index < (int) lines.size(); index++) cout << "| " << lines[index] << repeatChar(' ', width - 4 - (int) lines[index].size()) << " |\n";
    cout << "|" << repeatChar(' ', width - 2) << "|\n";
    cout << "\\" << repeatChar('=', width - 2) << "/\n";
}
string Game::prompt(const string& area) const { return brightColor + "student@" + area + mainColor + " $ "; }
string Game::bar(int value, int maxValue, int width) const {
    if (maxValue <= 0) return "[" + repeatChar('.', width) + "]";
    int filled = value * width / maxValue;
    if (filled < 0) filled = 0;
    if (filled > width) filled = width;
    return "[" + repeatChar('#', filled) + repeatChar('.', width - filled) + "]";
}
void Game::pause() const {
    cout << "\n" << prompt("continue");
    string line;
    getline(cin, line);
}
void Game::showTitle() const {
    cout << brightColor;
    cout << "====================================================================\n";
    cout << " LOOPBREAK // HKU CAMPUS TERMINAL\n";
    cout << " a looping campus story told in the dark\n";
    cout << "====================================================================\n";
    cout << dimColor << " compact prototype build\n\n" << mainColor;
}
void Game::showMenu() const {
    drawBox("main menu", vector<string>{"[1] new game", "[2] load game", "[3] how to play", "[4] quit"});
}
void Game::showAbout() const {
    drawBox("how to play", vector<string>{
        "find the clue in the library",
        "find the route in the admin office",
        "take the folder from the archive",
        "escape via archive door, bridge shaft, or clocktower stair",
        "",
        "commands: look move search hide wait use inventory status map journal objective save load menu quit"});
}
void Game::chooseDifficulty() {
    clearScreen();
    showTitle();
    drawBox("difficulty", vector<string>{"1. easy", "2. normal", "3. hard"});
    cout << "\n" << prompt("difficulty");
    string choice;
    getline(cin, choice);
    if (Utils::trim(choice) == "1") difficulty_ = Difficulty::Easy;
    else if (Utils::trim(choice) == "3") difficulty_ = Difficulty::Hard;
    else difficulty_ = Difficulty::Normal;
}
void Game::playLoop() {
    running_ = true;
    won_ = false;
    while (running_ && !won_ && !quit_) {
        clearScreen();
        showHud();
        showRoom();
        cout << dimColor << "\n[look] [move] [search] [hide] [wait] [status] [map] [journal] [objective] [help] [save] [load] [menu] [quit]\n" << mainColor;
        cout << prompt(player_.room());
        string input;
        if (!getline(cin, input)) {
            quit_ = true;
            return;
        }
        handle(Utils::trim(input));
    }
    if (won_) {
        clearScreen();
        drawBox("you win", vector<string>{"you reach the rooftop with the sealed folder", "the loop finally stops"});
        pause();
        won_ = false;
    }
}
void Game::showHud() const {
    vector<string> lines;
    lines.push_back("time " + Utils::formatTime(currentMinutes_) + " | loop " + to_string(loopNumber_) + " | difficulty " + difficultyName());
    lines.push_back("room " + player_.room() + " | hidden " + yesNo(player_.hidden()) + " | risk " + player_.suspicionRiskLevel());
    lines.push_back("suspicion " + bar(player_.suspicion(), 100, 20) + " " + to_string(player_.suspicion()) + "/100");
    lines.push_back("clue " + yesNo(libraryClue_) + " | route " + yesNo(adminRoute_) + " | folder " + yesNo(folder_));
    drawBox("status", lines);
}
void Game::showRoom() const {
    const Room* room = map_.get(player_.room());
    if (!room) return;
    vector<string> lines;
    lines.push_back("scene  : " + room->name);
    lines.push_back("");
    lines.push_back(room->description);
    lines.push_back("");
    lines.push_back("paths  : " + map_.exitsOf(room->id));
    lines.push_back("details: " + Utils::join(room->searchables, ", "));
    lines.push_back(string("tone   : ") + (room->restricted ? "restricted" : "public"));
    drawBox("current page", lines);
}
void Game::showHelp() const {
    drawBox("help", vector<string>{
        "look       show the current room again",
        "move       travel to a connected room",
        "search     look for progress clues",
        "hide       lower suspicion",
        "wait 10    pass time",
        "use [item] use an item from inventory",
        "inventory  show your items",
        "status map journal objective save load menu quit"});
}
void Game::showMap() const { drawBox("map", splitLines(map_.layout())); }
void Game::showStatus() const {
    int minutesLeft = dayEnd() - currentMinutes_;
    if (minutesLeft < 0) minutesLeft = 0;
    vector<string> lines;
    lines.push_back("time " + Utils::formatTime(currentMinutes_));
    lines.push_back("time left " + to_string(minutesLeft) + " minutes");
    lines.push_back("difficulty " + difficultyName());
    lines.push_back("suspicion " + to_string(player_.suspicion()) + "/100");
    lines.push_back("hidden " + yesNo(player_.hidden()));
    vector<string> summary = player_.statusSummary();
    lines.insert(lines.end(), summary.begin(), summary.end());
    drawBox("status", lines);
}
void Game::showJournal() const {
    vector<string> lines;
    if (libraryClue_) lines.push_back("library note: the reserve shelf hides the first clue");
    if (adminRoute_) lines.push_back("admin note: the archive path runs through the office wing");
    if (player_.hasItem(ItemType::Screwdriver)) lines.push_back("lab note: screwdriver can open the bridge panel and tunnel hatch");
    if (player_.hasItem(ItemType::Keycard)) lines.push_back("security note: keycard lowers heat in restricted corridors");
    if (folder_) lines.push_back("current loop: you already have the sealed folder");
    if (lines.empty()) lines.push_back("no notes saved yet");
    drawBox("journal", lines);
}
void Game::showObjective() const {
    vector<string> lines;
    lines.push_back(string(libraryClue_ ? "[x] " : "[ ] ") + "find the library clue");
    lines.push_back(string(adminRoute_ ? "[x] " : "[ ] ") + "find the admin route");
    lines.push_back(string(player_.hasItem(ItemType::Screwdriver) ? "[x] " : "[ ] ") + "secure a tool for hidden routes");
    lines.push_back(string(folder_ ? "[x] " : "[ ] ") + "take the folder");
    lines.push_back(string(folder_ && player_.room() == "rooftop" ? "[x] " : "[ ] ") + "reach the rooftop");
    if (player_.itemCount(ItemType::CodeFragment) > 0 || player_.hasItem(ItemType::RooftopKey)) {
        int count = player_.itemCount(ItemType::CodeFragment);
        lines.push_back(string(count >= codeFragmentsNeeded_ ? "[x] " : "[ ] ") + "collect code fragments (" + to_string(count) + "/" + to_string(codeFragmentsNeeded_) + ")");
    }
    drawBox("objective", lines);
}
void Game::showInventory() const {
    vector<ItemType> items = player_.inventoryList();
    if (items.empty()) {
        drawBox("inventory", vector<string>{"your inventory is empty"});
        return;
    }
    vector<string> lines;
    for (int i = 0; i < (int)items.size(); i++) {
        ItemType type = items[i];
        map<ItemType, Item>::const_iterator it = itemTable_.find(type);
        if (it != itemTable_.end()) {
            const Item& item = it->second;
            int count = player_.itemCount(type);
            lines.push_back(item.name + (count > 1 ? " (x" + to_string(count) + ")" : ""));
            lines.push_back("  " + item.description);
        }
    }
    drawBox("inventory", lines);
}

void Game::useItem(const string& itemName) {
    if (itemName.empty()) {
        drawBox("use", vector<string>{"what do you want to use?"});
        return;
    }
    ItemType type;
    if (!itemNameToType(itemName, type)) {
        drawBox("use", vector<string>{"unknown item: " + itemName});
        return;
    }
    if (!player_.hasItem(type)) {
        drawBox("use", vector<string>{"you do not have a " + itemName});
        return;
    }
    
    map<ItemType, Item>::const_iterator it = itemTable_.find(type);
    if (it == itemTable_.end()) return;
    const Item& item = it->second;

    vector<string> lines;
    bool success = false;

    if (type == ItemType::HallPass) {
        if (player_.suspicion() > 0) {
            lines.push_back(item.useMessage);
            player_.addSuspicion(item.suspicionEffect);
            lines.push_back("suspicion dropped by 15");
            success = true;
        } else {
            lines.push_back(item.failMessage);
            lines.push_back("your suspicion is already 0.");
        }
    } else if (type == ItemType::Keycard) {
        lines.push_back(item.failMessage);
        lines.push_back("the keycard is used automatically when entering restricted areas.");
    } else if (type == ItemType::Screwdriver) {
        if (player_.room() == "bridge") {
            lines.push_back(item.useMessage);
            lines.push_back("you can now move directly to the rooftop via the maintenance shaft.");
        } else {
            lines.push_back(item.failMessage);
        }
    } else if (type == ItemType::CodeFragment) {
        if (player_.room() == "archive" || player_.room() == "admin") {
            lines.push_back(item.useMessage);
            lines.push_back("the fragments form the sequence you need.");
        } else {
            lines.push_back(item.failMessage);
        }
    } else if (type == ItemType::RooftopKey) {
        if (player_.room() == "archive") {
            lines.push_back(item.useMessage);
            lines.push_back("rooftop door unlocked. you can now move explicitly to the rooftop cleanly.");
            success = true; 
        } else {
            lines.push_back(item.failMessage);
        }
    }

    if (success) {
        player_.incrementItemsUsed();
        if (item.consumable) {
            player_.removeItem(type);
            lines.push_back("(" + item.name + " consumed)");
        }
    }

    if (type == ItemType::RooftopKey && success) {
        player_.moveTo("rooftop");
        lines.push_back("you moved to rooftop.");
        if (folder_) won_ = true;
        checkReset();
    }

    drawBox("use " + itemName, lines);
}

void Game::handle(const string& input) {
    string command = lowerText(input);
    string argument = argumentOf(input);
    if (command.empty()) return;
    bool pauseAfter = true;
    if (command == "1" || command == "look") showRoom();
    else if (command == "2" || command == "move" || command == "go" || command.rfind("move ", 0) == 0 || command.rfind("go ", 0) == 0) movePlayer(argument);
    else if (command == "3" || command == "search") searchRoom();
    else if (command == "4" || command == "hide") hidePlayer();
    else if (command == "5" || command == "wait" || command.rfind("wait ", 0) == 0) waitPlayer(argument.empty() ? 10 : parseMinutes(argument, 10));
    else if (command == "inventory" || command == "inv" || command == "i") showInventory();
    else if (command == "use" || command.rfind("use ", 0) == 0) useItem(argument);
    else if (command == "6" || command == "status") showStatus();
    else if (command == "7" || command == "map") showMap();
    else if (command == "8" || command == "journal") showJournal();
    else if (command == "9" || command == "objective") showObjective();
    else if (command == "0" || command == "help") showHelp();
    else if (command == "save") drawBox("save", vector<string>(1, SaveSystem::save(*this) ? "game saved" : "save failed"));
    else if (command == "load") drawBox("load", vector<string>(1, SaveSystem::load(*this) ? "game loaded" : "savegame.txt was not found"));
    else if (command == "menu") {
        running_ = false;
        pauseAfter = false;
    } else if (command == "quit" || command == "exit") {
        running_ = false;
        quit_ = true;
        pauseAfter = false;
    } else drawBox("input", vector<string>(1, "unknown command"));
    if (pauseAfter && running_ && !won_ && !quit_) pause();
}
void Game::movePlayer(const string& target) {
    string destination = lowerText(target);
    if (destination.empty()) {
        drawBox("move", vector<string>(1, "type a connected room name after move"));
        return;
    }
    if (!map_.get(destination)) {
        drawBox("move", vector<string>(1, "that room does not exist"));
        return;
    }
    if (!map_.canMove(player_.room(), destination)) {
        drawBox("move", vector<string>(1, "you cannot move there from here"));
        return;
    }
    if (destination == "archive" && player_.room() == "admin" && !adminRoute_) {
        drawBox("move", vector<string>(1, "the archive wing is blocked until you confirm the route"));
        return;
    }
    if (destination == "archive" && player_.room() == "tunnel" && !player_.hasItem(ItemType::Screwdriver)) {
        drawBox("move", vector<string>(1, "the tunnel grate is jammed. you need a screwdriver to force it"));
        return;
    }
    if (destination == "archive" && !adminRoute_ && player_.room() != "tunnel") {
        drawBox("move", vector<string>(1, "the archive route is still unclear"));
        return;
    }
    if (destination == "tunnel" && player_.room() == "lab" && !player_.hasItem(ItemType::Screwdriver)) {
        drawBox("move", vector<string>(1, "the lab hatch is still bolted. you need a screwdriver"));
        return;
    }
    if (destination == "security" && !player_.hasItem(ItemType::Keycard) && !player_.hidden()) {
        drawBox("move", vector<string>(1, "security hub requires cover. hide first or carry a keycard"));
        return;
    }
    if (destination == "clocktower" && player_.room() == "library" && !libraryClue_) {
        drawBox("move", vector<string>(1, "you have not decoded the library margin note to find that stairwell"));
        return;
    }
    if (destination == "rooftop" && player_.room() == "archive" && !player_.hasItem(ItemType::RooftopKey)) {
        drawBox("move", vector<string>(1, "the door to the roof is locked. you need the rooftop key."));
        return;
    }
    if (destination == "rooftop" && player_.room() == "bridge" && !player_.hasItem(ItemType::Screwdriver)) {
        drawBox("move", vector<string>(1, "the maintenance panel is bolted shut. you need a screwdriver."));
        return;
    }
    if (destination == "rooftop" && player_.room() == "clocktower" && !player_.hasItem(ItemType::Keycard)) {
        drawBox("move", vector<string>(1, "the clocktower gate is magnetically locked. a keycard is required."));
        return;
    }
    bool wasHidden = player_.hidden();
    const Room* room = map_.get(destination);
    player_.moveTo(destination);
    player_.resetHideStreak();
    player_.incrementMoves();
    if (room && room->restricted) {
        if (player_.hasItem(ItemType::Keycard)) {
            // keycard avoids penalty
        } else {
            int rise = difficulty_ == Difficulty::Hard ? 14 : difficulty_ == Difficulty::Easy ? 8 : 11;
            if (wasHidden) rise -= 3;
            player_.addSuspicion(rise);
        }
    }
    if (!passTime(10)) return;
    vector<string> lines;
    lines.push_back("you moved to " + room->name);
    if (room->restricted) lines.push_back("restricted areas raise suspicion");
    drawBox("move", lines);
    if (destination == "rooftop" && folder_) won_ = true;
    checkReset();
}
void Game::searchRoom() {
    const Room* room = map_.get(player_.room());
    if (!room) return;
    if (!passTime(8)) return;
    vector<string> lines;
    player_.incrementSearches();

    if (room->id == "corridor" || room->id == "commons" || room->id == "canteen") {
        if (!player_.hasItem(ItemType::HallPass) && rand() % 2 == 0) {
            player_.addItem(ItemType::HallPass);
            lines.push_back("you found a forged hall pass.");
        }
    } else if (room->id == "faculty" || room->id == "security") {
        if (libraryClue_ && !player_.hasItem(ItemType::Keycard)) {
            player_.addItem(ItemType::Keycard);
            lines.push_back("you found an admin keycard after matching the library clue with staff records.");
        }
    } else if (room->id == "lab") {
        if (!player_.hasItem(ItemType::Screwdriver)) {
            player_.addItem(ItemType::Screwdriver);
            lines.push_back("you picked up a flathead screwdriver.");
        }
    } else if (room->id == "admin" || room->id == "archive" || room->id == "tunnel") {
        if (player_.itemCount(ItemType::CodeFragment) < codeFragmentsNeeded_) {
            player_.addItem(ItemType::CodeFragment);
            lines.push_back("you uncovered a code fragment.");
        } else if (player_.room() == "archive" && player_.itemCount(ItemType::CodeFragment) >= codeFragmentsNeeded_ && !player_.hasItem(ItemType::RooftopKey)) {
            player_.addItem(ItemType::RooftopKey);
            lines.push_back("you cracked the code and obtained the rooftop key!");
        }
    }

    if (room->id == "library") {
        if (!libraryClue_) {
            libraryClue_ = true;
            lines.push_back("you find the first clue in the reserve shelf");
        } else lines.push_back("the library confirms what you already know");
    } else if (room->id == "admin") {
        if (!libraryClue_) lines.push_back("the files mean little without the library clue");
        else if (!adminRoute_) {
            adminRoute_ = true;
            lines.push_back("you trace the route into the archive");
        } else lines.push_back("you already know the archive route");
    } else if (room->id == "tunnel") {
        if (player_.hasItem(ItemType::Screwdriver)) lines.push_back("you map a hidden tunnel bypass into the archive wing");
        else lines.push_back("you can hear airflow behind a sealed hatch, but cannot open it yet");
    } else if (room->id == "clocktower") {
        if (libraryClue_) lines.push_back("the tower stair can serve as a high-risk rooftop route");
        else lines.push_back("the stair controls are encoded in a library catalog reference");
    } else if (room->id == "archive") {
        if (!adminRoute_) lines.push_back("you are not ready to search the archive");
        else if (!folder_) {
            folder_ = true;
            lines.push_back("you secure the sealed folder");
        } else lines.push_back("the folder is already with you");
    } else lines.push_back("nothing important turns up here");
    if (room->restricted) {
        player_.addSuspicion(4);
        lines.push_back("staying here too long raises suspicion");
    }
    drawBox("search", lines);
    checkReset();
}
void Game::hidePlayer() {
    if (player_.hidden()) {
        drawBox("hide", vector<string>(1, "you are already hidden"));
        return;
    }
    if (!player_.canHideInRoom(player_.room())) {
        drawBox("hide", vector<string>(1, "there is nowhere useful to hide here"));
        return;
    }
    if (!passTime(5)) return;
    player_.setHidden(true);
    player_.incrementHideStreak();
    int eff = player_.hideEffectiveness();
    player_.addSuspicion(eff);
    drawBox("hide", vector<string>{"you keep a low profile for a while", "suspicion drops by " + to_string(-eff)});
}
void Game::waitPlayer(int minutes) {
    if (!passTime(minutes)) return;
    vector<string> lines;
    lines.push_back("you wait for " + to_string(minutes) + " minutes");
    if (player_.hidden()) {
        player_.addSuspicion(-4);
        lines.push_back("keeping still lowers suspicion");
    }
    player_.setHidden(false);
    player_.resetHideStreak();
    drawBox("wait", lines);
    checkReset();
}
bool Game::passTime(int minutes) {
    currentMinutes_ += minutes;
    player_.applySuspicionDecay(minutes);
    if (currentMinutes_ >= dayEnd()) {
        resetLoop(true, "the day ends and the loop starts again");
        return false;
    }
    return true;
}
void Game::checkReset() {
    if (player_.suspicion() >= 100) resetLoop(true, "security catches you before you can escape");
}
