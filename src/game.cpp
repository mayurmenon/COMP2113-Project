#include "game.h"
#include <algorithm>
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
const string cyanColor = "\033[38;5;81m";
const string blueColor = "\033[38;5;75m";
const string goldColor = "\033[38;5;221m";
const string amberColor = "\033[38;5;215m";
const string roseColor = "\033[38;5;204m";
const string redColor = "\033[38;5;203m";
const string silverColor = "\033[38;5;152m";
const string whiteColor = "\033[1;97m";
const int panelWidth = 78;
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
// Splits a block of text into lines. Input: text block. Output: vector of lines.
vector<string> splitLines(const string& text) {
    vector<string> lines;
    istringstream in(text);
    string line;
    while (getline(in, line)) lines.push_back(line);
    if (lines.empty()) lines.push_back("");
    return lines;
}
// Checks if an event with the given id is active. Input: event list and id. Output: true if active.
bool hasActiveEvent(const vector<game_event>& events, const string& id) {
    for (int index = 0; index < (int) events.size(); index++) {
        if (events[index].id == id && events[index].status == event_active) return true;
    }
    return false;
}
string tint(const string& text, const string& color) { return color + text + mainColor; }
string roomTone(const string& roomId) {
    if (roomId == "archive" || roomId == "security") return roseColor;
    if (roomId == "faculty" || roomId == "admin" || roomId == "clocktower") return amberColor;
    if (roomId == "tunnel" || roomId == "bridge") return blueColor;
    if (roomId == "library" || roomId == "commons" || roomId == "lecture") return cyanColor;
    if (roomId == "rooftop" || roomId == "quad") return whiteColor;
    return brightColor;
}
string panelTone(const string& title) {
    string key = lowerText(title);
    if (key == "loopbreak" || key == "main menu" || key == "difficulty") return goldColor;
    if (key == "status" || key == "map") return cyanColor;
    if (key == "current page" || key == "journal") return blueColor;
    if (key == "inventory" || key == "objective") return amberColor;
    if (key == "event" || key == "you win") return roseColor;
    if (key == "help" || key == "how to play" || key == "command deck") return whiteColor;
    return brightColor;
}
string itemTone(ItemType type) {
    if (type == ItemType::HallPass) return cyanColor;
    if (type == ItemType::Keycard) return blueColor;
    if (type == ItemType::Screwdriver) return amberColor;
    if (type == ItemType::CodeFragment) return goldColor;
    return roseColor;
}
string stripAnsi(const string& text) {
    string clean;
    for (int index = 0; index < (int) text.size(); index++) {
        char letter = text[index];
        if (letter == '\033') {
            if (index + 1 < (int) text.size() && text[index + 1] == '[') {
                index += 2;
                while (index < (int) text.size()) {
                    char code = text[index];
                    if (code >= '@' && code <= '~') break;
                    index++;
                }
            } else if (index + 1 < (int) text.size()) {
                index++;
            }
            continue;
        }
        clean += letter;
    }
    return clean;
}
int visibleLength(const string& text) { return (int) stripAnsi(text).size(); }
string padRight(const string& text, int width) {
    if (visibleLength(text) >= width) return text;
    return text + repeatChar(' ', width - visibleLength(text));
}
string centerText(const string& text, int width) {
    int gap = width - visibleLength(text);
    if (gap <= 0) return text;
    int left = gap / 2;
    int right = gap - left;
    return repeatChar(' ', left) + text + repeatChar(' ', right);
}
vector<string> wrapText(const string& text, int width) {
    vector<string> lines;
    if (text.empty()) {
        lines.push_back("");
        return lines;
    }
    istringstream in(text);
    string word;
    string line;
    while (in >> word) {
        if ((int) word.size() > width) {
            if (!line.empty()) {
                lines.push_back(line);
                line.clear();
            }
            string chunk = word;
            while ((int) chunk.size() > width) {
                lines.push_back(chunk.substr(0, width));
                chunk = chunk.substr(width);
            }
            line = chunk;
            continue;
        }
        if (line.empty()) {
            line = word;
        } else if ((int) line.size() + 1 + (int) word.size() <= width) {
            line += " " + word;
        } else {
            lines.push_back(line);
            line = word;
        }
    }
    if (!line.empty()) lines.push_back(line);
    if (lines.empty()) lines.push_back("");
    return lines;
}
vector<string> wrapLines(const vector<string>& lines, int width) {
    vector<string> wrapped;
    for (int index = 0; index < (int) lines.size(); index++) {
        if (lines[index].empty()) wrapped.push_back("");
        else if (visibleLength(lines[index]) <= width) wrapped.push_back(lines[index]);
        else {
            vector<string> part = wrapText(Utils::trim(lines[index]), width);
            wrapped.insert(wrapped.end(), part.begin(), part.end());
        }
    }
    if (wrapped.empty()) wrapped.push_back("");
    return wrapped;
}
vector<string> labelBlock(const string& label, const string& text, int width) {
    vector<string> block;
    int labelWidth = 10;
    int bodyWidth = width - labelWidth;
    if (bodyWidth < 8) bodyWidth = width;
    vector<string> wrapped = wrapText(text.empty() ? "-" : text, bodyWidth);
    for (int index = 0; index < (int) wrapped.size(); index++) {
        if (index == 0) block.push_back(padRight(label, labelWidth) + wrapped[index]);
        else block.push_back(repeatChar(' ', labelWidth) + wrapped[index]);
    }
    return block;
}
vector<string> mergeColumns(const vector<string>& left, const vector<string>& right, int leftWidth, int rightWidth) {
    vector<string> rows;
    int total = max((int) left.size(), (int) right.size());
    for (int index = 0; index < total; index++) {
        string leftText = index < (int) left.size() ? left[index] : "";
        string rightText = index < (int) right.size() ? right[index] : "";
        rows.push_back(padRight(leftText, leftWidth) + tint(" | ", dimColor) + padRight(rightText, rightWidth));
    }
    return rows;
}
vector<string> tintBlock(const vector<string>& lines, const string& color) {
    vector<string> painted;
    for (int index = 0; index < (int) lines.size(); index++) painted.push_back(tint(lines[index], color));
    return painted;
}
string tickBar(int value, int maxValue, int width, char filled, char empty) {
    if (maxValue <= 0) return repeatChar(empty, width);
    int amount = value * width / maxValue;
    if (amount < 0) amount = 0;
    if (amount > width) amount = width;
    return repeatChar(filled, amount) + repeatChar(empty, width - amount);
}
string timeRibbon(int currentMinutes, int endMinutes, int width) {
    int dayStart = 480;
    int total = endMinutes - dayStart;
    if (total <= 0) total = 1;
    int progress = currentMinutes - dayStart;
    if (progress < 0) progress = 0;
    if (progress > total) progress = total;
    string ribbon = tickBar(progress, total, width, '#', '.');
    int marker = progress * width / total;
    if (marker >= width) marker = width - 1;
    if (marker < 0) marker = 0;
    ribbon[marker] = '^';
    return "08:00 " + ribbon + " " + Utils::formatTime(endMinutes);
}
string minuteWindow(int currentMinutes, int endMinutes) {
    int left = endMinutes - currentMinutes;
    if (left < 0) left = 0;
    int hours = left / 60;
    int minutes = left % 60;
    return to_string(hours) + "h " + (minutes < 10 ? "0" : "") + to_string(minutes) + "m left";
}
string normalizeRoomTarget(const string& text) {
    string room = lowerText(text);
    if (room == "residence" || room == "room") return "dorm";
    if (room == "main corridor" || room == "hall") return "corridor";
    if (room == "lecture hall" || room == "classroom") return "lecture";
    if (room == "learning commons" || room == "common room") return "commons";
    if (room == "science lab") return "lab";
    if (room == "faculty corridor") return "faculty";
    if (room == "admin office" || room == "office") return "admin";
    if (room == "archive room") return "archive";
    if (room == "central quad") return "quad";
    if (room == "late-night canteen") return "canteen";
    if (room == "sports hall") return "gym";
    if (room == "security hub") return "security";
    if (room == "service tunnel") return "tunnel";
    if (room == "footbridge") return "bridge";
    if (room == "clocktower stairwell" || room == "tower") return "clocktower";
    if (room == "roof") return "rooftop";
    return room;
}
string nextObjectiveHint(const MovementContext& context, bool hasFolder) {
    if (!context.libraryClue && !context.hasScrewdriver) return "search the library for a clue or the lab for a tool";
    if (!context.libraryClue) return "search the library before pushing deeper";
    if (!context.adminRoute && !context.hasScrewdriver && !context.hasKeycard) return "use the clue to unlock admin, or prepare an alternate route";
    if (!hasFolder) return "reach the archive and secure the sealed folder";
    if (context.hasRooftopKey) return "head for the archive stair and extract";
    if (context.hasScrewdriver) return "head for the bridge shaft and extract";
    if (context.libraryClue && context.hasKeycard) return "head for the clocktower stair and extract";
    return "unlock any rooftop route before the loop collapses";
}
vector<string> roomArt(const string& roomId) {
    if (roomId == "dorm") return vector<string>{
        "   ____________       ",
        "  /  ____     /|      ",
        " /__/___/____/ |      ",
        " |  desk   | | |      ",
        " |_________|/ /       ",
        "    loop start        "};
    if (roomId == "library") return vector<string>{
        "   _________________  ",
        "  ||_|_|_|_|_|_|_|_|  ",
        "  ||_|_|_|_|_|_|_|_|  ",
        "  ||_|_|_|_|_|_|_|_|  ",
        "  || reserve shelf || ",
        "  '----------------'  "};
    if (roomId == "lab") return vector<string>{
        "      __              ",
        "     / /\\_            ",
        "    /_/ /_            ",
        "    \\ \\ \\_\\           ",
        "     \\_\\/ /           ",
        "   hatch + tools      "};
    if (roomId == "admin") return vector<string>{
        "   .------------.     ",
        "   | FILE | LED |     ",
        "   |------|-----|     ",
        "   |route |pass |     ",
        "   |______|_____|     ",
        "   records in play    "};
    if (roomId == "archive") return vector<string>{
        "   __________________ ",
        "  | [] [] [] [] []  | ",
        "  | [] [] [] [] []  | ",
        "  | [] sealed folder |",
        "  |__________________|",
        "   code lock nearby   "};
    if (roomId == "tunnel") return vector<string>{
        "   =================  ",
        "  //  pipe  pipe  \\\\ ",
        " ||   grate  hatch  ||",
        "  \\\\______________// ",
        "   damp echo below   ",
        "   hidden route      "};
    if (roomId == "bridge") return vector<string>{
        "      ______________  ",
        " ____/____________/|  ",
        "|____  ____  ____| |  ",
        "     |/    \\/    | |  ",
        "     / maintenance|/  ",
        "      exposed path    "};
    if (roomId == "clocktower") return vector<string>{
        "        /\\            ",
        "       /  \\           ",
        "      /_[]_\\          ",
        "      | [] |          ",
        "      | || |          ",
        "      tower stair     "};
    if (roomId == "security") return vector<string>{
        "   .----------------. ",
        "   | []  []  []  [] | ",
        "   | monitor wall   | ",
        "   | alert terminal | ",
        "   '----------------' ",
        "    full watch zone   "};
    if (roomId == "rooftop") return vector<string>{
        "        _^_           ",
        "   _   /___\\   _      ",
        " _|_|_|_|_|_|_|_|_    ",
        " |  skyline break  |  ",
        " |_________________|  ",
        "    final escape      "};
    return vector<string>{
        "    ______________    ",
        "   |   campus wing |  ",
        "   |  doors lights |  ",
        "   |  routes notes |  ",
        "   |_______________|  ",
        "    room signal       "};
}
vector<string> commandDeck() {
    vector<string> left;
    left.push_back("CORE");
    left.push_back("look        search      wait 10");
    left.push_back("move room   hide        use item");
    left.push_back("");
    left.push_back("INTEL");
    left.push_back("status      journal     objective");
    left.push_back("inventory   map         event");

    vector<string> right;
    right.push_back("SESSION");
    right.push_back("save        load        menu");
    right.push_back("help        quit");
    right.push_back("");
    right.push_back("SHORTCUTS");
    right.push_back("move admin   wait 20   use hall pass");
    right.push_back("inventory    event     look");
    return mergeColumns(left, right, 34, 37);
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
      folder_(false),
      lastEvent_("quiet campus") {
    srand(time(nullptr));
    itemTable_ = createItemTable();
    codeFragmentsNeeded_ = (rand() % 3) + 1;
    events_ = create_event_list(difficultyIndex());
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
    events_ = create_event_list(difficultyIndex());
    lastEvent_ = "save loaded";
    if (map_.get(roomId)) player_.moveTo(roomId);
    player_.setSuspicion(suspicion);
    player_.loadInventoryFromString(inventoryData);
    for (int i = 0; i < hideStreak; i++) player_.incrementHideStreak();
    if (hideStreak > 0) player_.setHidden(true);
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
    events_ = create_event_list(difficultyIndex());
    lastEvent_ = "quiet campus";
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
int Game::difficultyIndex() const {
    if (difficulty_ == Difficulty::Easy) return 0;
    if (difficulty_ == Difficulty::Hard) return 2;
    return 1;
}
void Game::clearScreen() const { cout << resetColor << "\033[40m" << mainColor << "\033[2J\033[H"; }
void Game::drawBox(const string& title, const vector<string>& lines) const {
    int innerWidth = panelWidth - 4;
    vector<string> body = wrapLines(lines, innerWidth);
    string tone = panelTone(title);
    string label = brightColor + "[ " + title + " ]" + mainColor;
    string top = tone + "+=" + label;
    int topFill = panelWidth - 1 - visibleLength(top);
    if (topFill < 0) topFill = 0;
    top += repeatChar('=', topFill) + "+" + mainColor;
    cout << top << "\n";
    for (int index = 0; index < (int) body.size(); index++) {
        cout << tone << "| " << mainColor << padRight(body[index], innerWidth) << tone << " |" << mainColor << "\n";
    }
    cout << tone << "+" << repeatChar('=', panelWidth - 2) << "+" << mainColor << "\n";
}
string Game::prompt(const string& area) const {
    string key = lowerText(area);
    string tone = roomTone(key);
    if (key == "menu" || key == "difficulty" || key == "continue") tone = goldColor;
    return tone + "student@" + area + brightColor + " $ " + mainColor;
}
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
    vector<string> lines;
    lines.push_back(centerText(tint(" _                               _                    _    ", cyanColor), 74));
    lines.push_back(centerText(tint("| |    ___   ___  _ __  _ __   | |__  _ __ ___  __ _| | __", blueColor), 74));
    lines.push_back(centerText(tint("| |   / _ \\ / _ \\| '_ \\| '_ \\  | '_ \\| '__/ _ \\/ _` | |/ /", whiteColor), 74));
    lines.push_back(centerText(tint("| |__| (_) | (_) | |_) | |_) | | |_) | | |  __/ (_| |   < ", goldColor), 74));
    lines.push_back(centerText(tint("|_____\\___/ \\___/| .__/| .__/  |_.__/|_|  \\___|\\__,_|_|\\_\\", roseColor), 74));
    lines.push_back(centerText(tint("                 |_|   |_|                                   ", amberColor), 74));
    lines.push_back("");
    lines.push_back(centerText(tint("campus loop console // stealth, route, reset, escape", silverColor), 74));
    lines.push_back(centerText(tint("every run starts in the dorm. one good run ends on the roof.", silverColor), 74));
    drawBox("loopbreak", lines);
}
void Game::showMenu() const {
    drawBox("main menu", vector<string>{
        "[1] new game      start a fresh loop from the dorm",
        "[2] load game     continue from savegame.txt",
        "[3] how to play   view the mission and command guide",
        "[4] quit          leave the terminal"});
}
void Game::showAbout() const {
    vector<string> lines;
    lines.push_back("mission");
    lines.push_back("find the library clue, confirm the archive route, take the sealed folder, and reach the rooftop before the day collapses.");
    lines.push_back("");
    lines.push_back("loop rules");
    lines.push_back("time and suspicion can reset the day. clue progress stays with you, but each run still needs a clean route.");
    lines.push_back("");
    lines.push_back("routes");
    lines.push_back("archive stair is direct, bridge shaft is fast, and clocktower stair is risky.");
    lines.push_back("");
    lines.push_back("tools");
    lines.push_back("hall pass helps cool suspicion. other key items work automatically when the route needs them.");
    lines.push_back("");
    lines.push_back("commands");
    lines.push_back("look, move, search, hide, wait, use, inventory, status, map, journal, objective, event, save, load, menu, quit");
    drawBox("how to play", lines);
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
        drawBox("command deck", commandDeck());
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
    vector<string> left;
    left.push_back("TELEMETRY");
    left.push_back("time      " + Utils::formatTime(currentMinutes_));
    left.push_back("loop      " + to_string(loopNumber_));
    left.push_back("mode      " + difficultyName());
    left.push_back("cover     " + string(player_.hidden() ? "hidden" : "open"));
    left.push_back("risk      " + player_.suspicionRiskLevel());
    left.push_back("pressure  " + bar(player_.suspicion(), 100, 20) + " " + to_string(player_.suspicion()) + "/100");

    vector<string> right;
    right.push_back("MISSION FEED");
    right.push_back("room      " + player_.room());
    right.push_back("clue      " + string(libraryClue_ ? "secured" : "missing"));
    right.push_back("route     " + string(adminRoute_ ? "mapped" : "unknown"));
    right.push_back("folder    " + string(folder_ ? "in hand" : "not taken"));
    right.push_back("progress  " + map_.progressionSummary(movementContext(), folder_));
    right.push_back("event     " + lastEvent_);

    vector<string> lines = mergeColumns(left, right, 34, 37);
    lines.push_back("");
    lines.push_back("dayline   " + timeRibbon(currentMinutes_, dayEnd(), 50));
    lines.push_back("window    " + minuteWindow(currentMinutes_, dayEnd()));
    drawBox("status", lines);
}
void Game::showRoom() const {
    const Room* room = map_.get(player_.room());
    if (!room) return;
    MovementContext context = movementContext();
    vector<string> left;
    left.push_back(tint(room->name, roomTone(room->id)));
    left.push_back("");
    vector<string> art = tintBlock(roomArt(room->id), roomTone(room->id));
    left.insert(left.end(), art.begin(), art.end());
    left.push_back("");
    left.push_back(tint("zone      ", silverColor) + tint(room->restricted ? "restricted" : "public", room->restricted ? amberColor : cyanColor));
    left.push_back(tint("threat    ", silverColor) + tint(room->threatProfile, room->restricted ? roseColor : whiteColor));
    left.push_back(tint("patrol    ", silverColor) + tint(room->patrolLabel() + " (" + to_string(room->patrolIntensity) + "/4)", room->patrolIntensity >= 3 ? redColor : room->patrolIntensity == 2 ? amberColor : cyanColor));

    vector<string> right;
    right.push_back(tint("SCENE FILE", blueColor));
    right.push_back("");
    vector<string> description = wrapText(room->description, 37);
    right.insert(right.end(), description.begin(), description.end());
    right.push_back("");
    string ambient = room->ambientFor(loopNumber_, currentMinutes_);
    if (!ambient.empty()) {
        vector<string> block = labelBlock("ambient", ambient, 47);
        right.insert(right.end(), block.begin(), block.end());
    }
    string explore = room->explorationFor(loopNumber_, currentMinutes_);
    if (!explore.empty()) {
        vector<string> block = labelBlock("intel", explore, 47);
        right.insert(right.end(), block.begin(), block.end());
    }
    string hint = room->progressionHintFor(loopNumber_, currentMinutes_);
    if (!hint.empty()) {
        vector<string> block = labelBlock("hint", hint, 47);
        right.insert(right.end(), block.begin(), block.end());
    }
    vector<string> dossier = map_.roomDossier(room->id);
    if (!dossier.empty()) {
        vector<string> block = labelBlock("brief", dossier[(loopNumber_ + currentMinutes_ / 25) % dossier.size()], 47);
        right.insert(right.end(), block.begin(), block.end());
    }
    vector<string> strategy = map_.routeStrategy(room->id, context);
    if (!strategy.empty()) {
        vector<string> block = labelBlock("route", strategy[(loopNumber_ + currentMinutes_ / 18) % strategy.size()], 47);
        right.insert(right.end(), block.begin(), block.end());
    }

    vector<string> lines = mergeColumns(left, right, 24, 47);
    lines.push_back("");
    vector<string> footer = labelBlock("paths", map_.exitsOf(room->id, context), 74);
    lines.insert(lines.end(), footer.begin(), footer.end());
    footer = labelBlock("details", Utils::join(room->searchables, ", "), 74);
    lines.insert(lines.end(), footer.begin(), footer.end());
    drawBox("current page", lines);
}
void Game::showHelp() const {
    vector<string> left;
    left.push_back("MOVEMENT + ACTION");
    left.push_back("look            redraw the current room page");
    left.push_back("move archive    travel to a connected room");
    left.push_back("search          check the room for clues or items");
    left.push_back("hide            reduce heat if the room allows it");
    left.push_back("wait 10         pass time and let the campus move");
    left.push_back("use hall pass   apply an item when needed");

    vector<string> right;
    right.push_back("INTEL + SESSION");
    right.push_back("status          show time, risk, and mission feed");
    right.push_back("inventory       list the items you are carrying");
    right.push_back("journal         show saved clue notes");
    right.push_back("objective       show current progression stage");
    right.push_back("event           check the latest disruption");
    right.push_back("save / load     keep or restore the current run");
    vector<string> lines = mergeColumns(left, right, 34, 37);
    lines.push_back("");
    lines.push_back("tip       commands accept natural input like 'move admin', 'wait 20', or 'use hall pass'.");
    drawBox("help", lines);
}
void Game::showMap() const {
    vector<string> lines = splitLines(map_.layout());
    lines.push_back("");
    lines.push_back("current room: " + player_.room());
    lines.push_back("objective   : archive -> rooftop");
    drawBox("map", lines);
}
void Game::showStatus() const {
    int minutesLeft = dayEnd() - currentMinutes_;
    if (minutesLeft < 0) minutesLeft = 0;
    vector<string> lines;
    lines.push_back("time        " + Utils::formatTime(currentMinutes_));
    lines.push_back("time left   " + to_string(minutesLeft) + " minutes");
    lines.push_back("difficulty  " + difficultyName());
    lines.push_back("suspicion   " + bar(player_.suspicion(), 100, 24) + " " + to_string(player_.suspicion()) + "/100");
    lines.push_back("state       " + string(player_.hidden() ? "hidden and still" : "visible and moving"));
    lines.push_back("dayline     " + timeRibbon(currentMinutes_, dayEnd(), 48));
    lines.push_back("");
    vector<string> summary = player_.statusSummary();
    lines.insert(lines.end(), summary.begin(), summary.end());
    drawBox("status", lines);
}
void Game::showJournal() const {
    vector<string> lines;
    if (libraryClue_) lines.push_back("library note  the reserve shelf hides the first clue");
    if (adminRoute_) lines.push_back("admin note    the archive path runs through the office wing");
    if (player_.hasItem(ItemType::Screwdriver)) lines.push_back("lab note      screwdriver opens the bridge panel and tunnel hatch");
    if (player_.hasItem(ItemType::Keycard)) lines.push_back("security note keycard softens heat in restricted corridors");
    if (folder_) lines.push_back("current loop  the sealed folder is already with you");
    if (lines.empty()) lines.push_back("no notes saved yet");
    drawBox("journal", lines);
}
void Game::showObjective() const {
    vector<string> lines;
    MovementContext context = movementContext();
    lines.push_back(string(libraryClue_ ? "[x] " : "[ ] ") + "find the library clue");
    lines.push_back(string(adminRoute_ ? "[x] " : "[ ] ") + "find the admin route");
    lines.push_back(string(player_.hasItem(ItemType::Screwdriver) ? "[x] " : "[ ] ") + "secure a tool for hidden routes");
    lines.push_back(string(folder_ ? "[x] " : "[ ] ") + "take the folder");
    lines.push_back(string(folder_ && player_.room() == "rooftop" ? "[x] " : "[ ] ") + "reach the rooftop");
    if (player_.itemCount(ItemType::CodeFragment) > 0 || player_.hasItem(ItemType::RooftopKey)) {
        int count = player_.itemCount(ItemType::CodeFragment);
        lines.push_back(string(count >= codeFragmentsNeeded_ ? "[x] " : "[ ] ") + "collect code fragments (" + to_string(count) + "/" + to_string(codeFragmentsNeeded_) + ")");
    }
    vector<string> routes = map_.availableEscapeRoutes(context);
    if (routes.empty()) lines.push_back("escape routes: none unlocked yet");
    else lines.push_back("escape routes: " + Utils::join(routes, ", "));
    lines.push_back("progress stage: " + map_.progressionSummary(context, folder_));
    lines.push_back("next step: " + nextObjectiveHint(context, folder_));
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
            lines.push_back(tint(item.name, itemTone(type)) + (count > 1 ? tint(" x" + to_string(count), silverColor) : ""));
            vector<string> wrapped = wrapText(item.description, 68);
            for (int line = 0; line < (int) wrapped.size(); line++) lines.push_back("  " + wrapped[line]);
            if (i + 1 < (int) items.size()) lines.push_back("");
        }
    }
    drawBox("inventory", lines);
}
void Game::showEvent() const {
    vector<string> lines;
    lines.push_back("latest event  " + lastEvent_);
    lines.push_back("");
    bool anyActive = false;
    for (int index = 0; index < (int) events_.size(); index++) {
        if (events_[index].status == event_active) {
            lines.push_back("active        " + events_[index].description);
            anyActive = true;
        }
    }
    if (!anyActive) lines.push_back("active        no disruptions in play");
    drawBox("event", lines);
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
    if (player_.hidden()) {
        player_.setHidden(false);
        player_.resetHideStreak();
        lines.push_back("you step out of cover to use the item.");
    }

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
        lines.push_back("the keycard works automatically in restricted routes.");
    } else if (type == ItemType::Screwdriver) {
        lines.push_back("the screwdriver works automatically on the bridge and tunnel routes.");
    } else if (type == ItemType::CodeFragment) {
        lines.push_back("keep collecting fragments and search the archive again when you have enough.");
    } else if (type == ItemType::RooftopKey) {
        lines.push_back("the rooftop key works automatically on the archive stair.");
    }

    if (success) {
        player_.incrementItemsUsed();
        if (item.consumable) {
            player_.removeItem(type);
            lines.push_back("(" + item.name + " consumed)");
        }
    }

    drawBox("use " + itemName, lines);
}

void Game::handle(const string& input) {
    string command = lowerText(input);
    string argument = argumentOf(input);
    if (command.empty()) return;
    bool pauseAfter = true;
    if (command == "1" || command == "look" || command == "l") showRoom();
    else if (command == "2" || command == "move" || command == "go" || command.rfind("move ", 0) == 0 || command.rfind("go ", 0) == 0) movePlayer(argument);
    else if (command == "3" || command == "search") searchRoom();
    else if (command == "4" || command == "hide") hidePlayer();
    else if (command == "5" || command == "wait" || command.rfind("wait ", 0) == 0) waitPlayer(argument.empty() ? 10 : parseMinutes(argument, 10));
    else if (command == "inventory" || command == "inv" || command == "i") showInventory();
    else if (command == "use" || command.rfind("use ", 0) == 0) useItem(argument);
    else if (command == "6" || command == "status") showStatus();
    else if (command == "7" || command == "map") showMap();
    else if (command == "8" || command == "journal") showJournal();
    else if (command == "9" || command == "objective" || command == "obj") showObjective();
    else if (command == "event" || command == "events") showEvent();
    else if (command == "0" || command == "help" || command == "?") showHelp();
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
    string destination = normalizeRoomTarget(target);
    if (destination.empty()) {
        drawBox("move", vector<string>{"type a connected room name after move", "from here: " + map_.exitsOf(player_.room(), movementContext())});
        return;
    }
    if (!map_.get(destination)) {
        drawBox("move", vector<string>{"that room does not exist", "from here: " + map_.exitsOf(player_.room(), movementContext())});
        return;
    }
    if (destination == "rooftop" && hasActiveEvent(events_, "rain")) {
        drawBox("move", vector<string>(1, "the rooftop exit is closed by heavy rain right now"));
        return;
    }
    MoveCheck validation = map_.validateMove(player_.room(), destination, movementContext());
    if (!validation.allowed) {
        drawBox("move", vector<string>(1, validation.reason));
        return;
    }
    bool wasHidden = player_.hidden();
    const Room* room = map_.get(destination);
    player_.moveTo(destination);
    player_.resetHideStreak();
    player_.incrementMoves();
    if (room && room->restricted) {
        int rise = difficulty_ == Difficulty::Hard ? 14 : difficulty_ == Difficulty::Easy ? 8 : 11;
        if (player_.hasItem(ItemType::Keycard)) rise -= 5;
        if (wasHidden) rise -= 3;
        if (hasActiveEvent(events_, "power_dip")) rise -= 2;
        if (hasActiveEvent(events_, "corridor_inspection")) rise += 2;
        if (rise < 1) rise = 1;
        player_.addSuspicion(rise);
    }
    if (!passTime(10)) return;
    vector<string> lines;
    lines.push_back("you moved to " + room->name);
    if (room->restricted) lines.push_back("restricted areas raise suspicion");
    if (validation.hiddenRoute) lines.push_back("you used a hidden route");
    if (validation.riskyRoute) lines.push_back("this route is high-risk and heavily watched");
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
    bool alternateArchiveRoute = player_.hasItem(ItemType::Screwdriver) || (libraryClue_ && player_.hasItem(ItemType::Keycard));
    bool archiveReady = adminRoute_ || alternateArchiveRoute;
    if (player_.hidden()) {
        player_.setHidden(false);
        player_.resetHideStreak();
        lines.push_back("you step out of cover to search the area.");
    }

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
        if (!archiveReady) lines.push_back("you are not ready to search the archive");
        else if (!folder_) {
            folder_ = true;
            if (adminRoute_) lines.push_back("you secure the sealed folder");
            else lines.push_back("your alternate route gets you inside and you secure the sealed folder");
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
    event_check_trigger(events_, currentMinutes_);
    for (int index = 0; index < (int) events_.size(); index++) {
        if (events_[index].status == event_active && events_[index].ticks_remaining == events_[index].duration) {
            lastEvent_ = events_[index].description;
            if (events_[index].suspiciousness_change != 0) player_.addSuspicion(events_[index].suspiciousness_change);
        }
    }
    events_tick(events_);
    return true;
}
void Game::checkReset() {
    if (player_.suspicion() >= 100) resetLoop(true, "security catches you before you can escape");
}
MovementContext Game::movementContext() const {
    MovementContext context;
    context.libraryClue = libraryClue_;
    context.adminRoute = adminRoute_;
    context.hasKeycard = player_.hasItem(ItemType::Keycard);
    context.hasScrewdriver = player_.hasItem(ItemType::Screwdriver);
    context.hasRooftopKey = player_.hasItem(ItemType::RooftopKey);
    context.hidden = player_.hidden();
    return context;
}
