#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include "map.h"
#include "player.h"
using namespace std;
enum class Difficulty { Easy, Normal, Hard };
class Game {
public:
    // Sets up the first game state. Input: none. Output: none.
    Game();
    // Starts the menu and gameplay flow. Input: none. Output: none.
    void start();
    // Restores a saved run. Input: saved game values. Output: none.
    void loadState(int minutes, Difficulty difficulty, const string& roomId, int suspicion, bool libraryClue, bool adminRoute, bool folder, int loopNumber);
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
    // Resets the loop state. Input: keepProgress flag and reset reason. Output: none.
    void resetLoop(bool keepProgress, const string& reason);
    // Returns the time limit for the current difficulty. Input: none. Output: end time in minutes.
    int dayEnd() const;
    // Clears the terminal screen. Input: none. Output: none.
    void clearScreen() const;
    // Draws a framed panel of text. Input: title and body lines. Output: none.
    void drawBox(const string& title, const vector<string>& lines) const;
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
    // Parses and runs one command. Input: raw user command. Output: none.
    void handle(const string& input);
    // Moves the player if the route is valid. Input: target room text. Output: none.
    void movePlayer(const string& target);
    // Searches the current room for progress. Input: none. Output: none.
    void searchRoom();
    // Puts the player in a hidden state. Input: none. Output: none.
    void hidePlayer();
    // Advances time by waiting. Input: number of minutes. Output: none.
    void waitPlayer(int minutes);
    // Adds time and checks for day end. Input: number of minutes. Output: true if loop continues.
    bool passTime(int minutes);
    // Checks whether suspicion causes a reset. Input: none. Output: none.
    void checkReset();
};
#endif
