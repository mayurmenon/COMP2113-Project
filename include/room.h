#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <vector>
using namespace std;
struct Room {
    string id;
    string name;
    string description;
    vector<string> exits;
    vector<string> searchables;
    bool restricted;
    // Builds an empty room. Input: none. Output: none.
    Room() : restricted(false) {}
    // Builds a room with all fields filled. Input: room values. Output: none.
    Room(const string& roomId, const string& roomName, const string& roomDescription, const vector<string>& roomExits, const vector<string>& roomSearchables, bool roomRestricted)
        : id(roomId), name(roomName), description(roomDescription), exits(roomExits), searchables(roomSearchables), restricted(roomRestricted) {}
};
#endif
