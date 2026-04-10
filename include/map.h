#ifndef MAP_H
#define MAP_H
#include <map>
#include <string>
#include "room.h"
using namespace std;
class CampusMap {
public:
    // Creates the map and fills the default rooms. Input: none. Output: none.
    CampusMap();
    // Rebuilds the default room graph. Input: none. Output: none.
    void reset();
    // Finds a room by id. Input: room id. Output: room pointer or null.
    const Room* get(const string& roomId) const;
    // Checks whether two rooms are directly connected. Input: current room and target room. Output: true or false.
    bool canMove(const string& from, const string& to) const;
    // Lists exits for one room. Input: room id. Output: exit text.
    string exitsOf(const string& roomId) const;
    // Returns the map layout as text. Input: none. Output: map string.
    string layout() const;
private:
    map<string, Room> rooms_;
};
#endif
