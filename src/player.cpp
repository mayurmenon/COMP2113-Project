#include "player.h"
#include <sstream>
using namespace std;
namespace {
// Parses an inventory count from save data. Input: text. Output: positive count or 0.
int parseInventoryCount(const string& text) {
    try {
        size_t used = 0;
        int value = stoi(text, &used);
        if (used != text.size() || value < 1) return 0;
        if (value > 99) return 99;
        return value;
    } catch (...) {
        return 0;
    }
}
}
Player::Player() { reset(); }
void Player::reset() {
    roomId_ = "dorm";
    suspicion_ = 0;
    hidden_ = false;

    clearInventory();
    hideStreak_ = 0;
    suspicionDecayTimer_ = 0;

    totalMoves_ = 0;
    totalSearches_ = 0;
    itemsUsed_ = 0;
    // Reset peak suspicion and room visit history for the new run.
    peakSuspicion_ = 0;
    visitedRooms_.clear();
    visitedRooms_.insert("dorm");
}
const string& Player::room() const { return roomId_; }
int Player::suspicion() const { return suspicion_; }
bool Player::hidden() const { return hidden_; }
void Player::moveTo(const string& roomId) {
    roomId_ = roomId;
    hidden_ = false;
    // Record each distinct room the player enters for the end summary.
    visitedRooms_.insert(roomId);
}
void Player::addSuspicion(int amount) { setSuspicion(suspicion_ + amount); }
void Player::setSuspicion(int amount) {
    if (amount < 0) amount = 0;
    if (amount > 100) amount = 100;
    suspicion_ = amount;
    // Update peak suspicion whenever it is raised above the previous high.
    if (suspicion_ > peakSuspicion_) peakSuspicion_ = suspicion_;
}
void Player::setHidden(bool hiddenValue) { hidden_ = hiddenValue; }

void Player::addItem(ItemType item) {
    inventory_[item]++;
}

void Player::removeItem(ItemType item) {
    if (inventory_.count(item) && inventory_[item] > 0) {
        inventory_[item]--;
        if (inventory_[item] == 0) {
            inventory_.erase(item);
        }
    }
}

bool Player::hasItem(ItemType item) const {
    map<ItemType, int>::const_iterator it = inventory_.find(item);
    return it != inventory_.end() && it->second > 0;
}

int Player::itemCount(ItemType item) const {
    map<ItemType, int>::const_iterator it = inventory_.find(item);
    return it != inventory_.end() ? it->second : 0;
}

vector<ItemType> Player::inventoryList() const {
    vector<ItemType> items;
    for (map<ItemType, int>::const_iterator it = inventory_.begin(); it != inventory_.end(); ++it) {
        if (it->second > 0) items.push_back(it->first);
    }
    return items;
}

void Player::clearInventory() {
    inventory_.clear();
}

void Player::incrementHideStreak() {
    hideStreak_++;
}

void Player::resetHideStreak() {
    hideStreak_ = 0;
}

int Player::hideEffectiveness() const {
    if (hideStreak_ <= 1) return -10;
    if (hideStreak_ == 2) return -6;
    return -2;
}

bool Player::canHideInRoom(const string& roomId) const {
    return roomId != "rooftop" && roomId != "bridge" && roomId != "corridor";
}

void Player::applySuspicionDecay(int minutesPassed) {
    suspicionDecayTimer_ += minutesPassed;
    if (suspicionDecayTimer_ >= 10) {
        int decayAmount = suspicionDecayTimer_ / 10;
        suspicionDecayTimer_ %= 10;
        addSuspicion(-decayAmount);
    }
}

string Player::suspicionRiskLevel() const {
    if (suspicion_ >= 80) return "critical";
    if (suspicion_ >= 60) return "danger";
    if (suspicion_ >= 40) return "risky";
    if (suspicion_ >= 20) return "cautious";
    return "safe";
}

void Player::incrementMoves() { totalMoves_++; }
void Player::incrementSearches() { totalSearches_++; }
void Player::incrementItemsUsed() { itemsUsed_++; }
int Player::totalMoves() const { return totalMoves_; }
int Player::totalSearches() const { return totalSearches_; }
int Player::itemsUsed() const { return itemsUsed_; }
int Player::hideStreak() const { return hideStreak_; }
// Returns the highest suspicion value recorded during the run. Input: none. Output: peak suspicion.
int Player::peakSuspicion() const { return peakSuspicion_; }
// Returns the count of distinct rooms visited during the run. Input: none. Output: unique room count.
int Player::uniqueRoomsVisited() const { return (int) visitedRooms_.size(); }

vector<string> Player::statusSummary() const {
    vector<string> lines;
    lines.push_back("--- Player Performance ---");
    lines.push_back("Total moves    : " + to_string(totalMoves_));
    lines.push_back("Rooms searched : " + to_string(totalSearches_));
    lines.push_back("Items used     : " + to_string(itemsUsed_));
    lines.push_back("Risk assessment: " + suspicionRiskLevel());
    return lines;
}

string Player::inventoryAsString() const {
    ostringstream out;
    bool first = true;
    for (map<ItemType, int>::const_iterator it = inventory_.begin(); it != inventory_.end(); ++it) {
        if (it->second > 0) {
            if (!first) out << ",";
            out << itemTypeToTag(it->first) << ":" << it->second;
            first = false;
        }
    }
    return out.str();
}

void Player::loadInventoryFromString(const string& data) {
    clearInventory();
    if (data.empty()) return;
    stringstream ss(data);
    string token;
    while (getline(ss, token, ',')) {
        size_t colon = token.find(':');
        if (colon != string::npos) {
            string tag = token.substr(0, colon);
            int count = parseInventoryCount(token.substr(colon + 1));
            ItemType type;
            if (count > 0 && tagToItemType(tag, type)) {
                inventory_[type] = count;
            }
        }
    }
}
