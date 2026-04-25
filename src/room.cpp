#include "room.h"

namespace {
int wrappedIndex(int value, int size) {
    if (size <= 0) return 0;
    int index = value % size;
    if (index < 0) index += size;
    return index;
}
}

void Room::addRoute(const Route& route) {
    routes.push_back(route);
    bool inExits = false;
    for (int i = 0; i < (int)exits.size(); i++) {
        if (exits[i] == route.to) {
            inExits = true;
            break;
        }
    }
    if (!inExits) exits.push_back(route.to);
}

const Route* Room::routeTo(const string& destination) const {
    for (int i = 0; i < (int)routes.size(); i++) {
        if (routes[i].to == destination) return &routes[i];
    }
    return nullptr;
}

vector<string> Room::routeTargets(bool includeHidden) const {
    vector<string> targets;
    for (int i = 0; i < (int)routes.size(); i++) {
        if (!includeHidden && routes[i].hidden) continue;
        targets.push_back(routes[i].to);
    }
    return targets;
}

bool Room::hasRouteTo(const string& destination) const {
    return routeTo(destination) != nullptr;
}

void Room::addAmbientDetail(const string& detail) {
    ambientDetails.push_back(detail);
}

void Room::addExplorationDetail(const string& detail) {
    explorationDetails.push_back(detail);
}

void Room::addProgressionHint(const string& hint) {
    progressionHints.push_back(hint);
}

string Room::ambientFor(int loopNumber, int minute) const {
    if (ambientDetails.empty()) return "";
    int index = wrappedIndex(loopNumber + (minute / 30), (int)ambientDetails.size());
    return ambientDetails[index];
}

string Room::explorationFor(int loopNumber, int minute) const {
    if (explorationDetails.empty()) return "";
    int index = wrappedIndex(loopNumber * 2 + (minute / 20), (int)explorationDetails.size());
    return explorationDetails[index];
}

string Room::progressionHintFor(int loopNumber, int minute) const {
    if (progressionHints.empty()) return "";
    int index = wrappedIndex(loopNumber * 3 + (minute / 15), (int)progressionHints.size());
    return progressionHints[index];
}

string Room::patrolLabel() const {
    if (patrolIntensity <= 0) return "clear";
    if (patrolIntensity == 1) return "light";
    if (patrolIntensity == 2) return "moderate";
    if (patrolIntensity == 3) return "heavy";
    return "critical";
}
