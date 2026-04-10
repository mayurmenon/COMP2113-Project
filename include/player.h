#ifndef PLAYER_H
#define PLAYER_H
#include <string>
using namespace std;
class Player {
public:
    // Creates a fresh player state. Input: none. Output: none.
    Player();
    // Restores the default player values. Input: none. Output: none.
    void reset();
    // Returns the current room id. Input: none. Output: room id.
    const string& room() const;
    // Returns the suspicion value. Input: none. Output: suspicion from 0 to 100.
    int suspicion() const;
    // Returns whether the player is hidden. Input: none. Output: true or false.
    bool hidden() const;
    // Moves the player to another room. Input: target room id. Output: none.
    void moveTo(const string& roomId);
    // Changes suspicion by a delta. Input: amount to add. Output: none.
    void addSuspicion(int amount);
    // Sets suspicion with bounds. Input: target amount. Output: none.
    void setSuspicion(int amount);
    // Sets the hidden flag. Input: hidden state. Output: none.
    void setHidden(bool hiddenValue);
private:
    string roomId_;
    int suspicion_;
    bool hidden_;
};
#endif
