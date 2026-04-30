#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include "event.h"
#include "map.h"
#include "player.h"
#include "item.h"
#include <memory>
#include <map>
#include <ctime>
using namespace std;
enum class Difficulty { Easy, Normal, Hard };
class Game {
public:
    // Sets up the first game state. Input: none. Output: none.
    Game();
    // Starts the menu and gameplay flow. Input: none. Output: none.
    void start();
    // Restores a saved run. Input: saved game values. Output: none.
    void loadState(int minutes, Difficulty difficulty, const string& roomId, int suspicion, bool libraryClue, bool adminRoute, bool folder, int loopNumber, const string& inventoryData, int hideStreak, int totalMoves, int totalSearches, int itemsUsed, int codeFragmentsNeeded, const string& eventData = "", const string& latestEvent = "save loaded", int peakSuspicion = 0, const string& visitedRoomsData = "", int realElapsedSeconds = 0);
    // Returns the current in-game time in minutes. Input: none. Output: total minutes.
    int currentMinutes() const;
    // Returns the current loop count. Input: none. Output: loop number.
    int loopNumber() const;
    // Returns the chosen difficulty enum. Input: none. Output: difficulty.
    Difficulty difficulty() const;
    // Returns the difficulty as text. Input: none. Output: difficulty name.
    string difficultyName() const;
    // Gives read-only access to player data. Input: none. Output: player reference.
    const Player& player() const;
    // Tells whether the library clue is known. Input: none. Output: true or false.
    bool libraryClue() const;
    // Tells whether the admin route is known. Input: none. Output: true or false.
    bool adminRoute() const;
    // Tells whether the folder has been collected. Input: none. Output: true or false.
    bool folder() const;
    // Returns the number of code fragments needed for the run. Input: none. Output: count.
    int codeFragmentsNeeded() const;
    // Serializes random event state for saving. Input: none. Output: event save text.
    string eventState() const;
    // Returns the latest event message. Input: none. Output: event text.
    const string& latestEvent() const;
    // Returns real wall-clock seconds elapsed in this run. Input: none. Output: elapsed seconds.
    int realElapsedSeconds() const;
private:
    CampusMap map_;
    Player player_;
    Difficulty difficulty_;
    int currentMinutes_;
    int loopNumber_;
    bool running_;
    bool won_;
    bool quit_;
    bool libraryClue_;
    bool adminRoute_;
    bool folder_;
    bool showTutorialNext_;

    map<ItemType, Item> itemTable_;
    int codeFragmentsNeeded_;
    unique_ptr<vector<game_event> > events_;
    string lastEvent_;
    // Wall-clock time when the current run started, used to compute real play time.
    time_t realStartTime_;

    // Resets the loop state. Input: keepProgress flag and reset reason. Output: none.
    void resetLoop(bool keepProgress, const string& reason);
    // Returns the time limit for the current difficulty. Input: none. Output: end time in minutes.
    int dayEnd() const;
    // Returns the helper difficulty index for side systems. Input: none. Output: 0, 1, or 2.
    int difficultyIndex() const;
    // Clears the terminal screen. Input: none. Output: none.
    void clearScreen() const;
    // Draws a framed panel of text. Input: title and body lines. Output: none.
    void drawBox(const string& title, const vector<string>& lines) const;
    // Draws a framed panel of text without wrapping the content.
    void drawBoxRaw(const string& title, const vector<string>& lines) const;
    // Builds the input prompt text. Input: current area name. Output: prompt string.
    string prompt(const string& area) const;
    // Builds a simple progress bar. Input: value, max value, width. Output: bar string.
    string bar(int value, int maxValue, int width) const;
    // Waits for the player to press enter. Input: none. Output: none.
    void pause() const;
    // Prints the game title screen. Input: none. Output: none.
    void showTitle() const;
    // Prints the main menu. Input: none. Output: none.
    void showMenu() const;
    // Prints the basic instructions. Input: none. Output: none.
    void showAbout() const;
    // Lets the player choose a difficulty. Input: none. Output: none.
    void chooseDifficulty();
    // Runs one full loop of gameplay. Input: none. Output: none.
    void playLoop();
    // Shows the guided start-of-run tutorial. Input: none. Output: none.
    void showTutorial();
    // Draws one tutorial page over the current game screen. Input: step and total pages. Output: none.
    void showTutorialStep(int step, int total) const;
    // Shows the separate easy-mode route guide panel. Input: none. Output: none.
    void showGuide() const;
    // Shows the top status panel. Input: none. Output: none.
    void showHud() const;
    // Shows the current room page. Input: none. Output: none.
    void showRoom() const;
    // Shows the command help panel. Input: none. Output: none.
    void showHelp() const;
    // Shows the campus map. Input: none. Output: none.
    void showMap() const;
    // Shows detailed player status. Input: none. Output: none.
    void showStatus() const;
    // Shows saved clue notes. Input: none. Output: none.
    void showJournal() const;
    // Shows current objectives. Input: none. Output: none.
    void showObjective() const;
    // Shows current inventory items and descriptions. Input: none. Output: none.
    void showInventory() const;
    // Shows the latest random event and active disruptions. Input: none. Output: none.
    void showEvent() const;
    // Shows the end-of-run achievement summary after a successful escape. Returns true if player wants to play again. Input: none. Output: play-again flag.
    bool showEndSummary() const;
    // Parses and runs one command. Input: raw user command. Output: none.
    void handle(const string& input);
    // Moves the player to a target room if valid. Input: target room text. Output: none.
    void movePlayer(const string& target);
    // Searches the current room for clues or items. Input: none. Output: none.
    void searchRoom();
    // Tries to hide the player in the current room. Input: none. Output: none.
    void hidePlayer();
    // Advances time without moving. Input: wait minutes. Output: none.
    void waitPlayer(int minutes);
    // Use item logic. Input: item name. Output: none.
    void useItem(const string& itemName);
    // Returns the current onboarding stage used for UI and hints. Input: none. Output: stage number.
    int onboardingStage() const;
    // Returns the current story beat shown at the top of the screen. Input: none. Output: story text.
    string storyBeat() const;
    // Returns the current hint text based on difficulty and progress. Input: none. Output: hint text.
    string activeHint() const;
    // Builds the current easy-mode guide lines. Input: none. Output: guide lines.
    vector<string> easyGuideLines() const;
    // Builds the right-side sidebar lines for the current room view. Input: none. Output: sidebar lines.
    vector<string> sidebarLines() const;
    // Adds time and checks for day end. Input: number of minutes. Output: true if loop continues.
    bool passTime(int minutes);
    // Checks whether suspicion causes a reset. Input: none. Output: none.
    void checkReset();
    // Builds map movement context from current player state. Input: none. Output: movement context.
    MovementContext movementContext() const;
    // Returns high-fidelity ASCII art for the current location.
    vector<string> getAsciiArt(const string& roomId) const;
};
#endif
