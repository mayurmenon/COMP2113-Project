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
const string mainColor = "\033[38;5;253m";
const string brightColor = "\033[1;38;5;120m";
const string dividerColor = "\033[38;5;250m";
const string cyanColor = "\033[38;5;81m";
const string blueColor = "\033[38;5;75m";
const string goldColor = "\033[38;5;221m";
const string amberColor = "\033[38;5;215m";
const string roseColor = "\033[38;5;204m";
const string redColor = "\033[38;5;203m";
const string silverColor = "\033[38;5;188m";
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
// Keeps a number inside a valid range. Input: value and bounds. Output: clamped value.
int clampInt(int value, int low, int high) {
    if (value < low) return low;
    if (value > high) return high;
    return value;
}
// Reads an integer without letting corrupt save data throw. Input: text and fallback. Output: parsed value.
int parseSavedInt(const string& text, int fallback) {
    try {
        size_t used = 0;
        int value = stoi(text, &used);
        return used == text.size() ? value : fallback;
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
// Restores event status records from a save string. Input: event list and serialized state. Output: none.
void applyEventState(vector<game_event>& events, const string& data) {
    if (data.empty()) return;
    stringstream stream(data);
    string token;
    while (getline(stream, token, ',')) {
        size_t first = token.find(':');
        size_t second = first == string::npos ? string::npos : token.find(':', first + 1);
        if (first == string::npos || second == string::npos) continue;
        string id = token.substr(0, first);
        int status = clampInt(parseSavedInt(token.substr(first + 1, second - first - 1), event_inactive), event_inactive, event_done);
        int ticks = parseSavedInt(token.substr(second + 1), 0);
        for (int index = 0; index < (int) events.size(); index++) {
            if (events[index].id == id) {
                events[index].status = status;
                events[index].ticks_remaining = clampInt(ticks, 0, events[index].duration);
                if (events[index].status != event_active) events[index].ticks_remaining = 0;
                if (events[index].status == event_active && events[index].ticks_remaining == 0) events[index].status = event_done;
            }
        }
    }
}
string tint(const string& text, const string& color) { return color + text + mainColor; }
string pulse(const string& text, const string& color) { return color + "[" + text + "]" + mainColor; }
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
    if (key == "status" || key == "map" || key == "focus") return cyanColor;
    if (key == "current page" || key == "current room" || key == "night briefing" || key == "journal") return blueColor;
    if (key == "inventory" || key == "objective" || key == "easy guide") return amberColor;
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
// Adds wrapped text while keeping a panel concise. Input: output list, text, width, line limit. Output: none.
void appendWrappedLimited(vector<string>& lines, const string& text, int width, int maxLines) {
    vector<string> wrapped = wrapText(text, width);
    for (int index = 0; index < (int) wrapped.size() && index < maxLines; index++) {
        lines.push_back(wrapped[index]);
    }
}
// Builds a colored section heading. Input: title and color. Output: heading text.
string sectionHeading(const string& title, const string& color) {
    return tint(title, color);
}
vector<string> labelBlock(const string& label, const string& text, int width) {
    vector<string> block;
    int labelWidth = 10;
    int bodyWidth = width - labelWidth;
    if (bodyWidth < 8) bodyWidth = width;
    vector<string> rawLines = splitLines(text.empty() ? "-" : text);
    bool firstLine = true;
    for (int index = 0; index < (int) rawLines.size(); index++) {
        vector<string> wrapped;
        if (rawLines[index].empty()) wrapped.push_back("");
        else wrapped = wrapText(rawLines[index], bodyWidth);
        for (int part = 0; part < (int) wrapped.size(); part++) {
            if (firstLine) {
                block.push_back(padRight(label, labelWidth) + wrapped[part]);
                firstLine = false;
            } else {
                block.push_back(repeatChar(' ', labelWidth) + wrapped[part]);
            }
        }
    }
    return block;
}
vector<string> mergeColumns(const vector<string>& left, const vector<string>& right, int leftWidth, int rightWidth) {
    vector<string> rows;
    int total = max((int) left.size(), (int) right.size());
    for (int index = 0; index < total; index++) {
        string leftText = index < (int) left.size() ? left[index] : "";
        string rightText = index < (int) right.size() ? right[index] : "";
        rows.push_back(padRight(leftText, leftWidth) + tint(" | ", dividerColor) + padRight(rightText, rightWidth));
    }
    return rows;
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
    if (room == "residence" || room == "room" || room == "hall room" || room == "swire hall" || room == "swire hall room") return "dorm";
    if (room == "main corridor" || room == "hall" || room == "university street" || room == "street") return "corridor";
    if (room == "lecture hall" || room == "classroom" || room == "grand hall" || room == "grand hall foyer") return "lecture";
    if (room == "library" || room == "main library") return "library";
    if (room == "learning commons" || room == "common room" || room == "chi wah" || room == "chi wah learning commons") return "commons";
    if (room == "science lab" || room == "kadoorie" || room == "kadoorie biological sciences building" || room == "kadoorie lab") return "lab";
    if (room == "faculty corridor" || room == "main building corridor") return "faculty";
    if (room == "admin office" || room == "office" || room == "main building office" || room == "registry office") return "admin";
    if (room == "archive room" || room == "records room" || room == "main building records room") return "archive";
    if (room == "central quad" || room == "courtyard" || room == "centennial courtyard" || room == "centennial campus courtyard") return "quad";
    if (room == "late-night canteen" || room == "canteen" || room == "cafe 330" || room == "composite building cafe") return "canteen";
    if (room == "sports hall" || room == "sports centre" || room == "stanley ho sports centre") return "gym";
    if (room == "security hub" || room == "security office" || room == "east gate security office") return "security";
    if (room == "service tunnel" || room == "centennial service tunnel" || room == "maintenance tunnel") return "tunnel";
    if (room == "footbridge" || room == "university street footbridge" || room == "bridge") return "bridge";
    if (room == "clocktower stairwell" || room == "tower" || room == "clock tower" || room == "main building clock tower") return "clocktower";
    if (room == "roof" || room == "main building rooftop") return "rooftop";
    return room;
}
vector<string> commandDeck(Difficulty difficulty, int stage) {
    vector<string> lines;
    if (stage == 0) lines.push_back(tint("FOCUS", goldColor) + "   reach the Main Library and search for the first clue");
    else if (stage == 1) lines.push_back(tint("FOCUS", goldColor) + "   turn the clue into a Main Building route");
    else if (stage == 2) lines.push_back(tint("FOCUS", goldColor) + "   prepare one clean archive entry and exit");
    else lines.push_back(tint("FOCUS", goldColor) + "   finish the file -> roof escape chain");
    lines.push_back("");
    lines.push_back(tint("CORE", cyanColor) + "    look, move <room>, search");
    if (stage == 0) {
        lines.push_back(tint("SAVE", blueColor) + "    save, load");
        lines.push_back(tint("MORE", whiteColor) + "    objective, status, help, menu, quit");
    } else if (stage == 1) {
        lines.push_back(tint("STEALTH", roseColor) + " hide, wait 10");
        lines.push_back(tint("MORE", whiteColor) + "    inventory, map, objective, save, load");
    } else if (stage == 2) {
        lines.push_back(tint("STEALTH", roseColor) + " hide, wait 10, use hall pass");
        lines.push_back(tint("MORE", whiteColor) + "    journal, map, event, save, load");
    } else {
        lines.push_back(tint("TOOLS", amberColor) + "   use <item>, hide, wait 10");
        lines.push_back(tint("MORE", whiteColor) + "    journal, map, event, save, load");
    }
    if (difficulty == Difficulty::Hard) lines.push_back(tint("MODE", amberColor) + "    hard: fewer hints");
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
      folder_(false),
      showTutorialNext_(true),
      events_(new vector<game_event>),
      lastEvent_("quiet campus") {
    srand(time(nullptr));
    itemTable_ = createItemTable();
    codeFragmentsNeeded_ = (rand() % 3) + 1;
    *events_ = create_event_list(difficultyIndex());
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
void Game::loadState(int minutes, Difficulty difficulty, const string& roomId, int suspicion, bool libraryClue, bool adminRoute, bool folder, int loopNumber, const string& inventoryData, int hideStreak, int totalMoves, int totalSearches, int itemsUsed, int codeFragmentsNeeded, const string& eventData, const string& latestEvent) {
    difficulty_ = difficulty;
    currentMinutes_ = clampInt(minutes, 480, dayEnd() - 1);
    loopNumber_ = clampInt(loopNumber, 1, 9999);
    libraryClue_ = libraryClue;
    adminRoute_ = adminRoute;
    folder_ = folder;
    running_ = true;
    won_ = false;
    showTutorialNext_ = false;
    player_.reset();
    codeFragmentsNeeded_ = clampInt(codeFragmentsNeeded, 1, 3);
    *events_ = create_event_list(difficultyIndex());
    applyEventState(*events_, eventData);
    lastEvent_ = latestEvent.empty() ? "save loaded" : latestEvent;
    if (map_.get(roomId)) player_.moveTo(roomId);
    player_.setSuspicion(suspicion);
    player_.loadInventoryFromString(inventoryData);
    hideStreak = clampInt(hideStreak, 0, 3);
    for (int i = 0; i < hideStreak; i++) player_.incrementHideStreak();
    if (hideStreak > 0) player_.setHidden(true);
    for (int i = 0; i < clampInt(totalMoves, 0, 9999); i++) player_.incrementMoves();
    for (int i = 0; i < clampInt(totalSearches, 0, 9999); i++) player_.incrementSearches();
    for (int i = 0; i < clampInt(itemsUsed, 0, 9999); i++) player_.incrementItemsUsed();
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
string Game::eventState() const {
    ostringstream out;
    for (int index = 0; index < (int) events_->size(); index++) {
        if (index > 0) out << ",";
        out << (*events_)[index].id << ":" << (*events_)[index].status << ":" << (*events_)[index].ticks_remaining;
    }
    return out.str();
}
const string& Game::latestEvent() const { return lastEvent_; }
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
    *events_ = create_event_list(difficultyIndex());
    lastEvent_ = "quiet campus";
    running_ = true;
    won_ = false;
    showTutorialNext_ = true;
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
    lines.push_back(centerText(tint("HKU after hours // route, hide, learn, escape", silverColor), 74));
    lines.push_back(centerText(tint("start small, read the campus, and break the loop one layer at a time.", silverColor), 74));
    drawBox("loopbreak", lines);
}
void Game::showMenu() const {
    drawBox("main menu", vector<string>{
        "[1] new game      start a fresh HKU night run",
        "[2] load game     continue from savegame.txt",
        "[3] how to play   view the route and command guide",
        "[4] quit          leave the terminal"});
}
void Game::showAbout() const {
    vector<string> lines;
    lines.push_back("story");
    lines.push_back("You wake in your hall room after another failed night at HKU. A sealed file is still moving around campus, and this time you want to reach the roof with it before the loop snaps back.");
    lines.push_back("");
    lines.push_back("early game");
    lines.push_back("Start with the Main Library and Chi Wah Learning Commons. The game opens up after you learn the route and find your first useful item.");
    lines.push_back("");
    lines.push_back("routes");
    lines.push_back("Main Library -> Main Building office -> records room is the cleanest path. Kadoorie, the service tunnel, the clock tower, and the footbridge become important later.");
    lines.push_back("");
    lines.push_back("difficulty");
    lines.push_back("Easy gives clear early hints. Normal gives lighter nudges. Hard mostly leaves the reading of the campus to you.");
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
void Game::showTutorial() {
    int total = difficulty_ == Difficulty::Easy ? 4 : difficulty_ == Difficulty::Normal ? 3 : 2;
    for (int step = 1; step <= total; step++) {
        clearScreen();
        showHud();
        showRoom();
        drawBox("command deck", commandDeck(difficulty_, onboardingStage()));
        showTutorialStep(step, total);
        cout << prompt("tutorial");
        string input;
        getline(cin, input);
        string command = lowerText(input);
        if (command == "skip") break;
        if (command == "save") {
            drawBox("save", vector<string>(1, SaveSystem::save(*this) ? "game saved" : "save failed"));
            pause();
        } else if (command == "load") {
            bool loaded = SaveSystem::load(*this);
            drawBox("load", vector<string>(1, loaded ? "game loaded" : "savegame.txt was not found"));
            pause();
            if (loaded) return;
        } else if (command == "menu") {
            running_ = false;
            return;
        } else if (command == "quit" || command == "exit") {
            running_ = false;
            quit_ = true;
            return;
        }
    }
}
void Game::showTutorialStep(int step, int total) const {
    vector<string> lines;
    if (step == 1) {
        lines.push_back("This screen is your whole run.");
        lines.push_back("Left side: where you are, what the room feels like, where you can go, and what you can search.");
        lines.push_back("Right side: time, goals, items, and event notes.");
        if (difficulty_ == Difficulty::Easy) lines.push_back("The separate easy guide panel below gives the current best next step.");
        lines.push_back("");
        lines.push_back("Start with " + pulse("move <room>", goldColor) + ", " + pulse("search", cyanColor) + ", and " + pulse("look", whiteColor) + ".");
        lines.push_back("Press Enter to continue, or type " + pulse("skip", amberColor) + " if you already know the basics.");
    } else if (step == 2) {
        lines.push_back("To win a run, build the chain in order:");
        lines.push_back("Main Library clue -> Main Building route -> sealed file -> rooftop escape.");
        lines.push_back("");
        lines.push_back(pulse("objective", goldColor) + " shows the route checklist.");
        lines.push_back(pulse("status", cyanColor) + " shows time, suspicion, and your pace.");
        lines.push_back("If suspicion reaches 100 or the day ends, the loop resets.");
    } else if (step == 3) {
        lines.push_back("Later systems unlock after the run opens up.");
        lines.push_back("Do not worry about everything at once.");
        lines.push_back("");
        lines.push_back(pulse("inventory", whiteColor) + " shows what you carry.");
        lines.push_back(pulse("journal", cyanColor) + " keeps route notes.");
        lines.push_back(pulse("event", amberColor) + " shows active campus events.");
        lines.push_back(pulse("use <item>", goldColor) + " matters more in the mid and late game.");
    } else {
        lines.push_back("Hint style depends on difficulty.");
        if (difficulty_ == Difficulty::Easy) {
            lines.push_back("Easy mode gives exact early help like " + pulse("move corridor", goldColor) + ", " + pulse("move library", cyanColor) + ", and " + pulse("search", whiteColor) + ".");
            lines.push_back("After you understand the route, those hints fade and you take over.");
        } else if (difficulty_ == Difficulty::Normal) {
            lines.push_back("Normal mode points you in the right direction, but expects you to read the campus yourself.");
        } else {
            lines.push_back("Hard mode only gives a light opening nudge. After that, your own notes matter most.");
        }
        lines.push_back("When in doubt, return to public rooms, lower suspicion, and rebuild the route one step at a time.");
    }
    drawBox("tutorial " + to_string(step) + "/" + to_string(total), lines);
}
void Game::showGuide() const {
    vector<string> lines = easyGuideLines();
    if (lines.empty()) return;
    drawBox("easy guide", lines);
}
void Game::playLoop() {
    running_ = true;
    won_ = false;
    while (running_ && !won_ && !quit_) {
        if (showTutorialNext_) {
            showTutorial();
            showTutorialNext_ = false;
        }
        clearScreen();
        showHud();
        showRoom();
        if (difficulty_ == Difficulty::Easy) showGuide();
        drawBox("command deck", commandDeck(difficulty_, onboardingStage()));
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
    string hint = activeHint();
    if (hint.empty()) hint = map_.progressionSummary(movementContext(), folder_);
    lines.push_back(sectionHeading("NEXT FOCUS", goldColor));
    appendWrappedLimited(lines, hint, 72, 2);
    lines.push_back("");
    lines.push_back(sectionHeading("SNAPSHOT", cyanColor));
    lines.push_back("time " + Utils::formatTime(currentMinutes_) + " / risk " + player_.suspicionRiskLevel() + " / place " + player_.room());
    if (lastEvent_ != "quiet campus") {
        lines.push_back("");
        lines.push_back(sectionHeading("CAMPUS EVENT", roseColor));
        appendWrappedLimited(lines, lastEvent_, 72, 1);
    }
    drawBox("focus", lines);
}
void Game::showRoom() const {
    const Room* room = map_.get(player_.room());
    if (!room) return;
    MovementContext context = movementContext();
    vector<string> left;
    left.push_back(sectionHeading("LOCATION", roomTone(room->id)));
    left.push_back(tint(room->name, roomTone(room->id)) + " [" + room->id + "]");
    left.push_back("");
    appendWrappedLimited(left, room->description, 46, 2);
    left.push_back("");

    left.push_back(sectionHeading("WHAT MATTERS HERE", amberColor));
    string roomFocus = room->progressionHintFor(loopNumber_, currentMinutes_);
    if (roomFocus.empty()) roomFocus = room->explorationFor(loopNumber_, currentMinutes_);
    appendWrappedLimited(left, roomFocus.empty() ? "Use this room to keep the route moving." : roomFocus, 46, 2);
    if (room->restricted) left.push_back(tint("warning  watched room: search quickly", roseColor));
    else left.push_back(tint("safe     public room: good for planning", cyanColor));
    left.push_back("");

    left.push_back(sectionHeading("PATHS", blueColor));
    string pathText = onboardingStage() <= 1 ? map_.exitsOf(room->id) : map_.exitsOf(room->id, context);
    vector<string> block = labelBlock("move", pathText, 46);
    left.insert(left.end(), block.begin(), block.end());
    left.push_back("");
    left.push_back(sectionHeading("ACTION", goldColor));
    block = labelBlock("search", Utils::join(room->searchables, ", "), 46);
    left.insert(left.end(), block.begin(), block.end());

    vector<string> lines = mergeColumns(left, sidebarLines(), 46, 25);
    drawBox("current room", lines);
}
void Game::showHelp() const {
    vector<string> lines;
    lines.push_back("start with public rooms and short actions.");
    lines.push_back("");
    lines.push_back("look            redraw the main screen");
    lines.push_back("move library    travel to a connected room");
    lines.push_back("search          check the room for clues or items");
    lines.push_back("objective       see the current route checklist");
    lines.push_back("status          check time, risk, and your run stats");
    lines.push_back("save / load     keep or restore a run");
    if (onboardingStage() >= 1) {
        lines.push_back("inventory       show all items in detail");
        lines.push_back("map             show the campus graph");
        lines.push_back("hide / wait 10  manage suspicion");
    }
    if (onboardingStage() >= 2) {
        lines.push_back("use hall pass   spend your pass to cool suspicion");
        lines.push_back("journal         check saved route notes");
        lines.push_back("event           review active campus events");
    }
    lines.push_back("");
    lines.push_back("You can type short room ids like 'move admin' or full names like 'move Main Library'.");
    drawBox("help", lines);
}
void Game::showMap() const {
    vector<string> lines = splitLines(map_.layout());
    const Room* room = map_.get(player_.room());
    lines.push_back("");
    lines.push_back("you are here : " + string(room ? room->name + " [" + room->id + "]" : player_.room()));
    lines.push_back("main route  : Main Library -> Main Building Office -> Main Building Records Room -> Main Building Rooftop");
    if (onboardingStage() <= 1) lines.push_back("late routes : more routes open after your first clue and first useful item");
    else lines.push_back("late routes : Kadoorie tunnel, footbridge shaft, and clock tower stair");
    drawBox("map", lines);
}
void Game::showStatus() const {
    int minutesLeft = dayEnd() - currentMinutes_;
    if (minutesLeft < 0) minutesLeft = 0;
    vector<string> lines;
    const Room* room = map_.get(player_.room());
    lines.push_back("location    " + string(room ? room->name : player_.room()));
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
    if (libraryClue_) lines.push_back("main library   the margin note points away from the stacks and toward the Main Building records.");
    if (adminRoute_) lines.push_back("main building  the office ledger maps the route into the records room.");
    if (player_.hasItem(ItemType::Screwdriver)) lines.push_back("kadoorie       the screwdriver opens the service hatch and the footbridge panel.");
    if (player_.hasItem(ItemType::Keycard)) lines.push_back("security       the keycard softens the pressure in watched rooms.");
    if (player_.itemCount(ItemType::CodeFragment) > 0) lines.push_back("code trail     you have " + to_string(player_.itemCount(ItemType::CodeFragment)) + " code fragment(s) for the roof lock.");
    if (folder_) lines.push_back("current loop   the sealed file is already with you.");
    if (lines.empty()) lines.push_back("no notes saved yet");
    drawBox("journal", lines);
}
void Game::showObjective() const {
    vector<string> lines;
    MovementContext context = movementContext();
    lines.push_back(string(libraryClue_ ? "[x] " : "[ ] ") + "read the Main Library clue");
    lines.push_back(string(adminRoute_ ? "[x] " : "[ ] ") + "map the Main Building records route");
    lines.push_back(string(player_.hasItem(ItemType::Screwdriver) ? "[x] " : "[ ] ") + "secure a tool for hidden HKU routes");
    lines.push_back(string(folder_ ? "[x] " : "[ ] ") + "take the sealed file");
    lines.push_back(string(folder_ && player_.room() == "rooftop" ? "[x] " : "[ ] ") + "reach the rooftop");
    if (player_.itemCount(ItemType::CodeFragment) > 0 || player_.hasItem(ItemType::RooftopKey)) {
        int count = player_.itemCount(ItemType::CodeFragment);
        lines.push_back(string(count >= codeFragmentsNeeded_ ? "[x] " : "[ ] ") + "collect roof code fragments (" + to_string(count) + "/" + to_string(codeFragmentsNeeded_) + ")");
    }
    vector<string> routes = map_.availableEscapeRoutes(context);
    if (routes.empty()) lines.push_back("escape routes: none unlocked yet");
    else lines.push_back("escape routes: " + Utils::join(routes, ", "));
    lines.push_back("progress stage: " + map_.progressionSummary(context, folder_));
    string hint = activeHint();
    if (!hint.empty()) lines.push_back("hint: " + hint);
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
    for (int index = 0; index < (int) events_->size(); index++) {
        if ((*events_)[index].status == event_active) {
            lines.push_back("active        " + (*events_)[index].description);
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
    string normalizedItem = lowerText(itemName);
    ItemType type;
    if (!itemNameToType(normalizedItem, type)) {
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
    if (destination == "rooftop" && hasActiveEvent(*events_, "rain")) {
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
        if (hasActiveEvent(*events_, "power_dip")) rise -= 2;
        if (hasActiveEvent(*events_, "corridor_inspection")) rise += 2;
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
            lines.push_back("you spot a late-study pass tucked into a notice sleeve.");
        }
    } else if (room->id == "faculty" || room->id == "security") {
        if (libraryClue_ && !player_.hasItem(ItemType::Keycard)) {
            player_.addItem(ItemType::Keycard);
            lines.push_back("you match the library note to staff records and pocket a Main Building keycard.");
        }
    } else if (room->id == "lab") {
        if (!player_.hasItem(ItemType::Screwdriver)) {
            player_.addItem(ItemType::Screwdriver);
            lines.push_back("you borrow a flathead screwdriver from the Kadoorie tool bench.");
        }
    } else if (room->id == "admin" || room->id == "archive" || room->id == "tunnel") {
        if (player_.itemCount(ItemType::CodeFragment) < codeFragmentsNeeded_) {
            player_.addItem(ItemType::CodeFragment);
            lines.push_back("you copy down part of the Main Building roof code.");
        } else if (player_.room() == "archive" && !player_.hasItem(ItemType::RooftopKey)) {
            player_.addItem(ItemType::RooftopKey);
            lines.push_back("the full code lines up and releases the rooftop key from the locked tray.");
        }
    }

    if (room->id == "library") {
        if (!libraryClue_) {
            libraryClue_ = true;
            lines.push_back("a note in the Main Library links the sealed file to the Main Building records room.");
        } else lines.push_back("the shelves and terminals only confirm the clue you already have.");
    } else if (room->id == "admin") {
        if (!libraryClue_) lines.push_back("the office papers make little sense without the Main Library clue.");
        else if (!adminRoute_) {
            adminRoute_ = true;
            lines.push_back("the office ledger maps the way into the records room.");
        } else lines.push_back("the office records only repeat the route you already mapped.");
    } else if (room->id == "tunnel") {
        if (player_.hasItem(ItemType::Screwdriver)) lines.push_back("you trace the service line and learn where it rises into the records side.");
        else lines.push_back("you hear airflow behind the sealed hatch, but it is still bolted shut.");
    } else if (room->id == "clocktower") {
        if (libraryClue_) lines.push_back("the clue and the tower markings line up. This stair can reach the roof with the right access.");
        else lines.push_back("the tower markings only make sense after the Main Library clue.");
    } else if (room->id == "archive") {
        if (!archiveReady) lines.push_back("you are in the records room, but you still do not understand the route well enough to find the file.");
        else if (!folder_) {
            folder_ = true;
            if (adminRoute_) lines.push_back("you pull the sealed file from the records tray and tuck it away.");
            else lines.push_back("the hidden route gets you in, and you secure the sealed file before anyone comes through.");
        } else lines.push_back("the sealed file is already with you.");
    } else lines.push_back("nothing useful turns up here.");
    if (room->restricted) {
        player_.addSuspicion(4);
        lines.push_back("searching in a watched room raises suspicion.");
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
int Game::onboardingStage() const {
    if (folder_ || player_.itemCount(ItemType::CodeFragment) > 0 || player_.hasItem(ItemType::RooftopKey) || player_.totalMoves() >= 14) return 3;
    if (adminRoute_ || player_.hasItem(ItemType::Screwdriver) || player_.hasItem(ItemType::Keycard) || player_.totalSearches() >= 3) return 2;
    if (libraryClue_ || player_.totalMoves() >= 3 || player_.totalSearches() >= 1) return 1;
    return 0;
}
string Game::activeHint() const {
    string room = player_.room();
    int stage = onboardingStage();
    if (difficulty_ != Difficulty::Hard && player_.suspicion() >= 75) {
        if (difficulty_ == Difficulty::Easy) return "Your suspicion is high. Slow down, hide, wait, or use the hall pass before entering another watched room.";
        return "Suspicion is climbing fast. Reset your pace before the next restricted room.";
    }

    if (difficulty_ == Difficulty::Hard) {
        if (stage == 0 && player_.totalMoves() == 0) return "Start with the Main Library or Chi Wah. After that, read the campus for yourself.";
        return "";
    }

    if (difficulty_ == Difficulty::Normal) {
        if (stage == 0) return "The Main Library is the safest place to understand the route.";
        if (stage == 1) return "The clue now points toward the Main Building office and the routes around University Street.";
        if (stage == 2 && !folder_) return "You know the basics now. Prepare access and one exit before forcing the records room.";
        return "";
    }

    if (stage == 0) {
        if (room == "dorm") return "Try `move corridor`. University Street connects the early safe rooms.";
        if (room == "corridor") return "Try `move library`. The Main Library starts the clean route.";
        if (room == "library") return "Try `search`. The first clue is hidden in the Main Library.";
        return "Stay in public rooms first. The Main Library is the cleanest opening move.";
    }

    if (stage == 1) {
        if (room == "library") return "Try `move corridor`, then work toward Chi Wah Learning Commons.";
        if (room == "corridor") return "Try `move commons`. Chi Wah is a calmer step before the Main Building office.";
        if (room == "commons") return "Try `move admin`. The office records are the next key layer.";
        if (room == "admin") return "Try `search`. The office ledger should map the records route.";
        return "Follow the clue from the Main Library into Chi Wah and then the Main Building office.";
    }

    if (!folder_) {
        if (room == "archive") return "You are at the records room. Search only when you are ready to leave quickly.";
        if (player_.hasItem(ItemType::Screwdriver) && (room == "lab" || room == "tunnel")) return "The Kadoorie service route is ready if you want a cleaner way around the Main Building.";
        if (player_.totalMoves() >= 10 || player_.totalSearches() >= 4) return "";
        return "You know the shape of the run now. Focus on one way in and one way out.";
    }

    return "";
}
vector<string> Game::easyGuideLines() const {
    vector<string> lines;
    if (difficulty_ != Difficulty::Easy) return lines;

    string room = player_.room();
    int stage = onboardingStage();

    if (player_.suspicion() >= 75) {
        lines.push_back("next step   lower suspicion before you push deeper");
        lines.push_back("try         hide, wait 10, or use hall pass");
        lines.push_back("reason      one bad restricted move can reset the loop now");
        return lines;
    }

    if (stage == 0) {
        if (room == "dorm") {
            lines.push_back("next command  move corridor");
            lines.push_back("reason        University Street connects the safe opening rooms");
            lines.push_back("after that    move library, then search");
            return lines;
        }
        if (room == "corridor") {
            lines.push_back("next command  move library");
            lines.push_back("reason        the Main Library gives the first clue");
            lines.push_back("after that    search");
            return lines;
        }
        if (room == "library") {
            lines.push_back("next command  search");
            lines.push_back("reason        the Main Library clue starts the real route");
            lines.push_back("after that    move corridor and head toward Chi Wah");
            return lines;
        }
        lines.push_back("next focus   stay in public rooms and reach the Main Library");
        return lines;
    }

    if (stage == 1) {
        if (room == "library") {
            lines.push_back("next command  move corridor");
            lines.push_back("reason        you already have the clue, so it is time to move on");
            lines.push_back("after that    move commons");
            return lines;
        }
        if (room == "corridor") {
            lines.push_back("next command  move commons");
            lines.push_back("reason        Chi Wah is the calmer setup room before admin");
            lines.push_back("after that    move admin");
            return lines;
        }
        if (room == "commons") {
            lines.push_back("next command  move admin");
            lines.push_back("reason        the Main Building office maps the records-room route");
            lines.push_back("after that    search");
            return lines;
        }
        if (room == "admin") {
            lines.push_back("next command  search");
            lines.push_back("reason        the office ledger unlocks the records-room route");
            lines.push_back("after that    prepare for archive");
            return lines;
        }
        lines.push_back("next focus   follow the clue into Chi Wah and then the Main Building office");
        return lines;
    }

    if (!folder_) {
        if (room == "archive") {
            lines.push_back("next command  search");
            lines.push_back("reason        the sealed file and roof-code progress both come from here");
            lines.push_back("warning       leave quickly after you get what you need");
            return lines;
        }
        if (player_.hasItem(ItemType::Screwdriver) && room == "lab") {
            lines.push_back("option        move tunnel");
            lines.push_back("reason        the Kadoorie route now gives you a hidden way around the Main Building");
            lines.push_back("or            keep using the Main Building office route if it feels safer");
            return lines;
        }
        lines.push_back("focus        get into the records room with one clean exit in mind");
        lines.push_back("good exits   roof stair, footbridge shaft, or clock tower");
        return lines;
    }

    lines.push_back("focus        you have the file");
    lines.push_back("next step    use the safest roof route you already unlocked");
    lines.push_back("warning      do not stop to collect extra things now");
    return lines;
}
vector<string> Game::sidebarLines() const {
    vector<string> lines;
    lines.push_back(sectionHeading("STATUS", cyanColor));
    lines.push_back("time   " + Utils::formatTime(currentMinutes_));
    lines.push_back("left   " + minuteWindow(currentMinutes_, dayEnd()));
    lines.push_back("loop   " + to_string(loopNumber_) + " / " + difficultyName());
    lines.push_back("risk   " + player_.suspicionRiskLevel());
    lines.push_back("cover  " + string(player_.hidden() ? "hidden" : "open"));
    lines.push_back("");

    lines.push_back(sectionHeading("OBJECTIVE", goldColor));
    if (!libraryClue_) lines.push_back("1 search library");
    else if (!adminRoute_) lines.push_back("2 map admin route");
    else if (!folder_) lines.push_back("3 take sealed file");
    else lines.push_back("4 reach rooftop");
    lines.push_back(string(libraryClue_ ? "[x] " : "[ ] ") + "clue");
    lines.push_back(string(adminRoute_ ? "[x] " : "[ ] ") + "route");
    lines.push_back(string(folder_ ? "[x] " : "[ ] ") + "file");
    if (player_.itemCount(ItemType::CodeFragment) > 0 || player_.hasItem(ItemType::RooftopKey)) lines.push_back("code   " + to_string(player_.itemCount(ItemType::CodeFragment)) + "/" + to_string(codeFragmentsNeeded_));

    vector<ItemType> items = player_.inventoryList();
    if (!items.empty()) {
        lines.push_back("");
        lines.push_back(sectionHeading("ITEMS", amberColor));
        for (int i = 0; i < (int) items.size(); i++) {
            map<ItemType, Item>::const_iterator it = itemTable_.find(items[i]);
            if (it != itemTable_.end()) {
                string itemLine = it->second.name;
                int count = player_.itemCount(items[i]);
                if (count > 1) itemLine += " x" + to_string(count);
                lines.push_back(itemLine);
            }
        }
    }
    if (lastEvent_ != "quiet campus") {
        lines.push_back("");
        lines.push_back(sectionHeading("EVENT", roseColor));
        vector<string> eventBlock = wrapText(lastEvent_, 25);
        lines.insert(lines.end(), eventBlock.begin(), eventBlock.end());
    }
    return lines;
}
bool Game::passTime(int minutes) {
    currentMinutes_ += minutes;
    player_.applySuspicionDecay(minutes);
    if (currentMinutes_ >= dayEnd()) {
        resetLoop(true, "the day ends and the loop starts again");
        return false;
    }
    events_tick(*events_, minutes);
    event_check_trigger(*events_, currentMinutes_ - 480);
    for (int index = 0; index < (int) events_->size(); index++) {
        if ((*events_)[index].status == event_active && (*events_)[index].ticks_remaining == (*events_)[index].duration) {
            lastEvent_ = (*events_)[index].description;
            if ((*events_)[index].suspiciousness_change != 0) player_.addSuspicion((*events_)[index].suspiciousness_change);
        }
    }
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
