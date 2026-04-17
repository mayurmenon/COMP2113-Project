#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include <vector>
#include <map>
#include "item.h"

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

    // --- Inventory System ---
    // Adds an item to the player's inventory. Input: item type. Output: none.
    void addItem(ItemType item);
    // Removes one instance of an item. Input: item type. Output: none.
    void removeItem(ItemType item);
    // Checks if the player has at least one of the item. Input: item type. Output: true/false.
    bool hasItem(ItemType item) const;
    // Returns the quantity of an item. Input: item type. Output: count.
    int itemCount(ItemType item) const;
    // Returns a list of all unique item types currently held. Input: none. Output: vector of item types.
    vector<ItemType> inventoryList() const;
    // Empties the inventory. Input: none. Output: none.
    void clearInventory();

    // --- Hiding System Improvements ---
    // Increases the consecutive hide streak and returns the current effective reduction. Output: none.
    void incrementHideStreak();
    // Resets the hide streak to 0. Output: none.
    void resetHideStreak();
    // Returns the suspicion reduction value based on current hide streak. Output: suspicion reduction amount.
    int hideEffectiveness() const;
    // Checks if hiding is allowed in a specific room type (e.g. not rooftop). Input: room id. Output: true/false.
    bool canHideInRoom(const string& roomId) const;

    // --- Suspicion Modifiers ---
    // Applies passive suspicion decay over time spent. Input: time passed in minutes. Output: none.
    void applySuspicionDecay(int minutesPassed);
    // Returns a risk categorization string based on suspicion. Output: string description.
    string suspicionRiskLevel() const;

    // --- Status Helpers ---
    void incrementMoves();
    void incrementSearches();
    void incrementItemsUsed();
    int totalMoves() const;
    int totalSearches() const;
    int itemsUsed() const;
    int hideStreak() const;
    // Returns a formatted vector of strings describing the player's performance. Output: status lines.
    vector<string> statusSummary() const;

    // --- Save/Load Helpers ---
    // Serializes inventory to a string format. Output: string.
    string inventoryAsString() const;
    // Deserializes inventory string format and populates inventory mapping. Input: string. Output: none.
    void loadInventoryFromString(const string& data);
private:
    string roomId_;
    int suspicion_;
    bool hidden_;

    map<ItemType, int> inventory_;
    int hideStreak_;
    int suspicionDecayTimer_;

    int totalMoves_;
    int totalSearches_;
    int itemsUsed_;
};
#endif
