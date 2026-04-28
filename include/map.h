#ifndef MAP_H
#define MAP_H
#include <map>
#include <string>
#include "room.h"
using namespace std;

struct MovementContext {
    bool libraryClue;
    bool adminRoute;
    bool hasKeycard;
    bool hasScrewdriver;
    bool hasRooftopKey;
    bool hidden;
};

struct MoveCheck {
    bool allowed;
    bool hiddenRoute;
    bool riskyRoute;
    string reason;
    // Builds the default failed movement result. Input: none. Output: none.
    MoveCheck() : allowed(false), hiddenRoute(false), riskyRoute(false) {}
    // Builds a movement result with full details. Input: move flags and message. Output: none.
    MoveCheck(bool isAllowed, bool isHiddenRoute, bool isRiskyRoute, const string& message)
        : allowed(isAllowed), hiddenRoute(isHiddenRoute), riskyRoute(isRiskyRoute), reason(message) {}
};

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
    // Validates movement with progression locks. Input: current room, target room, and player context. Output: move result.
    MoveCheck validateMove(const string& from, const string& to, const MovementContext& context) const;
    // Lists exits for one room. Input: room id. Output: exit text.
    string exitsOf(const string& roomId) const;
    // Lists exit labels with route state. Input: room id and player context. Output: exit text.
    string exitsOf(const string& roomId, const MovementContext& context) const;
    // Lists exits as stacked lines for the room panel. Input: room id. Output: multiline exit text.
    string exitListOf(const string& roomId) const;
    // Lists route states as stacked lines for the room panel. Input: room id and player context. Output: multiline exit text.
    string exitListOf(const string& roomId, const MovementContext& context) const;
    // Lists currently available rooftop escape routes. Input: player context. Output: route-name list.
    vector<string> availableEscapeRoutes(const MovementContext& context) const;
    // Returns a high-level progression summary. Input: player context and folder state. Output: summary text.
    string progressionSummary(const MovementContext& context, bool hasFolder) const;
    // Returns room-specific dossier notes. Input: room id. Output: note lines.
    vector<string> roomDossier(const string& roomId) const;
    // Returns room-specific strategy notes. Input: room id and player context. Output: note lines.
    vector<string> routeStrategy(const string& roomId, const MovementContext& context) const;
    // Returns a map-wide tactical briefing. Input: player context and folder state. Output: note lines.
    vector<string> sectorBriefing(const MovementContext& context, bool hasFolder) const;
    // Returns a dynamic heist checklist. Input: player context and folder state. Output: checklist lines.
    vector<string> heistChecklist(const MovementContext& context, bool hasFolder) const;
    // Returns the map layout as text. Input: none. Output: map string.
    string layout() const;
private:
    map<string, Room> rooms_;
    // Checks whether a route requirement is satisfied. Input: requirement tag and player context. Output: true or false.
    bool requirementMet(const string& requirement, const MovementContext& context) const;
};
#endif
