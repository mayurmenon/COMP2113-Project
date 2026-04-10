#include "player.h"
using namespace std;
Player::Player() { reset(); }
void Player::reset() {
    roomId_ = "dorm";
    suspicion_ = 0;
    hidden_ = false;
}
const string& Player::room() const { return roomId_; }
int Player::suspicion() const { return suspicion_; }
bool Player::hidden() const { return hidden_; }
void Player::moveTo(const string& roomId) {
    roomId_ = roomId;
    hidden_ = false;
}
void Player::addSuspicion(int amount) { setSuspicion(suspicion_ + amount); }
void Player::setSuspicion(int amount) {
    if (amount < 0) amount = 0;
    if (amount > 100) amount = 100;
    suspicion_ = amount;
}
void Player::setHidden(bool hiddenValue) { hidden_ = hiddenValue; }
