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
    MoveCheck() : allowed(false), hiddenRoute(false), riskyRoute(false) {}
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
    // Validates movement with progression state and lock checks.
    MoveCheck validateMove(const string& from, const string& to, const MovementContext& context) const;
    // Lists exits for one room. Input: room id. Output: exit text.
    string exitsOf(const string& roomId) const;
    // Lists route labels for one room using progression context.
    string exitsOf(const string& roomId, const MovementContext& context) const;
    // Lists currently available rooftop escape routes based on tools/progression.
    vector<string> availableEscapeRoutes(const MovementContext& context) const;
    // Returns high-level progression stage text from map-lock perspective.
    string progressionSummary(const MovementContext& context, bool hasFolder) const;
    // Returns room-specific progression dossier notes.
    vector<string> roomDossier(const string& roomId) const;
    // Returns room-specific route strategy notes based on progression state.
    vector<string> routeStrategy(const string& roomId, const MovementContext& context) const;
    // Returns a map-wide tactical briefing by sector.
    vector<string> sectorBriefing(const MovementContext& context, bool hasFolder) const;
    // Returns a dynamic heist checklist from map progression state.
    vector<string> heistChecklist(const MovementContext& context, bool hasFolder) const;
    // Returns the map layout as text. Input: none. Output: map string.
    string layout() const;
private:
    map<string, Room> rooms_;
    // Checks if one route requirement is satisfied.
    bool requirementMet(const string& requirement, const MovementContext& context) const;
};
#endif
