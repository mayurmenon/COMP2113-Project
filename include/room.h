#ifndef ROOM_H
#define ROOM_H
#include <string>
#include <vector>
using namespace std;

// Stores one directed route from a room to another room.
struct Route {
    string to;
    string label;
    bool hidden;
    bool risky;
    string requirement;
    string blockedMessage;

    // Builds an empty route. Input: none. Output: none.
    Route()
        : hidden(false), risky(false) {}
    // Builds one full route record. Input: route fields. Output: none.
    Route(const string& routeTo,
          const string& routeLabel,
          bool routeHidden,
          bool routeRisky,
          const string& routeRequirement,
          const string& routeBlockedMessage)
        : to(routeTo),
          label(routeLabel),
          hidden(routeHidden),
          risky(routeRisky),
          requirement(routeRequirement),
          blockedMessage(routeBlockedMessage) {}
};

struct Room {
    string id;
    string name;
    string description;
    vector<string> exits;
    vector<string> searchables;
    vector<string> ambientDetails;
    vector<string> explorationDetails;
    vector<string> progressionHints;
    bool restricted;
    int patrolIntensity;
    string threatProfile;
    vector<Route> routes;

    // Builds an empty room. Input: none. Output: none.
    Room() : restricted(false), patrolIntensity(0), threatProfile("neutral") {}
    // Builds a room with all fields filled. Input: room values. Output: none.
    Room(const string& roomId, const string& roomName, const string& roomDescription, const vector<string>& roomExits, const vector<string>& roomSearchables, bool roomRestricted)
        : id(roomId), name(roomName), description(roomDescription), exits(roomExits), searchables(roomSearchables), restricted(roomRestricted), patrolIntensity(roomRestricted ? 2 : 0), threatProfile(roomRestricted ? "controlled" : "open") {}

    // Adds one directed route and updates simple exits. Input: route data. Output: none.
    void addRoute(const Route& route);
    // Finds a route by destination room id. Input: destination room id. Output: route pointer or null.
    const Route* routeTo(const string& destination) const;
    // Lists route targets with optional hidden routes. Input: includeHidden flag. Output: room-id list.
    vector<string> routeTargets(bool includeHidden) const;
    // Checks whether a route exists. Input: destination room id. Output: true or false.
    bool hasRouteTo(const string& destination) const;
    // Adds one ambient detail line. Input: detail text. Output: none.
    void addAmbientDetail(const string& detail);
    // Adds one exploration detail line. Input: detail text. Output: none.
    void addExplorationDetail(const string& detail);
    // Adds one progression hint line. Input: hint text. Output: none.
    void addProgressionHint(const string& hint);
    // Returns one ambient detail for the current loop and time. Input: loop number and minute. Output: detail text.
    string ambientFor(int loopNumber, int minute) const;
    // Returns one exploration detail for the current loop and time. Input: loop number and minute. Output: detail text.
    string explorationFor(int loopNumber, int minute) const;
    // Returns one progression hint for the current loop and time. Input: loop number and minute. Output: hint text.
    string progressionHintFor(int loopNumber, int minute) const;
    // Returns a patrol-pressure label. Input: none. Output: pressure text.
    string patrolLabel() const;
};
#endif
