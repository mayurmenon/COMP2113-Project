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

    Route()
        : hidden(false), risky(false) {}
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

    // Adds a directed route and keeps simple exits in sync.
    void addRoute(const Route& route);
    // Finds a directed route by destination room id.
    const Route* routeTo(const string& destination) const;
    // Lists route targets with optional hidden route visibility.
    vector<string> routeTargets(bool includeHidden) const;
    // Returns true if a route exists to destination.
    bool hasRouteTo(const string& destination) const;
    // Adds one ambient detail line for room narration.
    void addAmbientDetail(const string& detail);
    // Adds one exploration detail line shown on room view.
    void addExplorationDetail(const string& detail);
    // Adds one progression hint line used by map progression.
    void addProgressionHint(const string& hint);
    // Selects one ambient detail for the current loop and time.
    string ambientFor(int loopNumber, int minute) const;
    // Selects one exploration detail for the current loop and time.
    string explorationFor(int loopNumber, int minute) const;
    // Selects one progression hint for the current loop and time.
    string progressionHintFor(int loopNumber, int minute) const;
    // Returns a textual patrol pressure label from patrol intensity.
    string patrolLabel() const;
};
#endif
