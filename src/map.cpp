#include "map.h"
#include "utils.h"
using namespace std;
CampusMap::CampusMap() { reset(); }
void CampusMap::reset() {
    rooms_.clear();
    rooms_["dorm"] = Room("dorm", "Residence Room", "A cramped room where every loop begins.", {"corridor"}, {"desk", "noticeboard"}, false);
    rooms_["corridor"] = Room("corridor", "Main Corridor", "The busiest path on campus.", {"dorm", "library", "lecture", "commons", "bridge"}, {"posters", "door signs"}, false);
    rooms_["lecture"] = Room("lecture", "Lecture Hall", "Rows of seats and a quiet back corner.", {"corridor", "lab"}, {"podium", "front row"}, false);
    rooms_["library"] = Room("library", "Library", "Reserve shelves and study desks hide useful clues.", {"corridor", "faculty"}, {"reserve shelf", "catalog terminal"}, false);
    rooms_["commons"] = Room("commons", "Learning Commons", "Open tables make it easy to blend in.", {"corridor", "admin"}, {"flyers", "whiteboard"}, false);
    rooms_["lab"] = Room("lab", "Science Lab", "Equipment trays and cabinets fill the room.", {"lecture", "faculty"}, {"tool bench", "storage cabinet"}, false);
    rooms_["faculty"] = Room("faculty", "Faculty Corridor", "A quieter corridor where you should not stay long.", {"library", "lab", "admin"}, {"office plaques", "mail trays"}, true);
    rooms_["admin"] = Room("admin", "Admin Office", "Forms and records point toward the archive.", {"commons", "faculty", "archive"}, {"filing cabinet", "reception desk"}, true);
    rooms_["archive"] = Room("archive", "Archive Room", "The sealed folder is stored here.", {"admin", "rooftop"}, {"drawer bank", "coded lock"}, true);
    rooms_["bridge"] = Room("bridge", "Footbridge", "A fast route between buildings.", {"corridor", "rooftop"}, {"bench", "maintenance panel"}, false);
    rooms_["rooftop"] = Room("rooftop", "Rooftop Exit", "If you reach this point with the folder, the loop ends.", {"bridge", "archive"}, {"stair door", "skyline"}, false);
}
const Room* CampusMap::get(const string& roomId) const {
    map<string, Room>::const_iterator found = rooms_.find(roomId);
    return found == rooms_.end() ? nullptr : &found->second;
}
bool CampusMap::canMove(const string& from, const string& to) const {
    const Room* room = get(from);
    if (!room) return false;
    for (int index = 0; index < (int) room->exits.size(); index++) if (room->exits[index] == to) return true;
    return false;
}
string CampusMap::exitsOf(const string& roomId) const {
    const Room* room = get(roomId);
    return room ? Utils::join(room->exits, ", ") : "";
}
string CampusMap::layout() const {
    return "Campus map\n[dorm]\n   |\n[corridor]--[lecture]--[lab]\n   |           |\n[library]    [commons]\n   |           |\n[faculty]---[admin]---[archive]---[rooftop]\n   |\n[bridge]-----------------------------+";
}
