#include "map.h"
#include "utils.h"
using namespace std;
CampusMap::CampusMap() { reset(); }
void CampusMap::reset() {
    rooms_.clear();
    rooms_["dorm"] = Room("dorm", "Residence Room", "A cramped room where every loop begins. Your notes and gear are scattered from previous failed attempts.", {"corridor"}, {"desk", "noticeboard", "half-packed bag"}, false);
    rooms_["corridor"] = Room("corridor", "Main Corridor", "The busiest path on campus. Students drift by, security cameras blink, and every doorway feels like a fork in the heist.", {"dorm", "library", "lecture", "commons", "quad"}, {"posters", "door signs", "camera dome"}, false);
    rooms_["lecture"] = Room("lecture", "Lecture Hall", "Rows of empty seats face a glowing projector. Whispering here carries farther than expected.", {"corridor", "lab"}, {"podium", "front row", "side aisle"}, false);
    rooms_["library"] = Room("library", "Library", "Reserve shelves, terminal logs, and scribbled margin notes point to routes most students never notice.", {"corridor", "faculty", "clocktower"}, {"reserve shelf", "catalog terminal", "newspaper rack"}, false);
    rooms_["commons"] = Room("commons", "Learning Commons", "Open tables and group-study booths make this the easiest place to blend in and plan your next move.", {"corridor", "admin", "canteen"}, {"flyers", "whiteboard", "charging station"}, false);
    rooms_["lab"] = Room("lab", "Science Lab", "Equipment trays and locked cabinets line the walls. A maintenance hatch is half-hidden behind gas canisters.", {"lecture", "faculty", "tunnel"}, {"tool bench", "storage cabinet", "maintenance hatch"}, false);
    rooms_["faculty"] = Room("faculty", "Faculty Corridor", "A quiet corridor with frosted office doors and suspiciously frequent patrol routes.", {"library", "lab", "admin", "security"}, {"office plaques", "mail trays", "meeting board"}, true);
    rooms_["admin"] = Room("admin", "Admin Office", "Forms, attendance ledgers, and archived movement records reveal how the secure wing is really connected.", {"commons", "faculty", "archive", "security"}, {"filing cabinet", "reception desk", "routing binder"}, true);
    rooms_["archive"] = Room("archive", "Archive Room", "Dusty cabinets hold the sealed folder. A coded lock and reinforced exit door protect the final objective.", {"admin", "rooftop", "tunnel"}, {"drawer bank", "coded lock", "sealed cabinet"}, true);
    rooms_["quad"] = Room("quad", "Central Quad", "The open campus square links major buildings. Good visibility, but little cover if guards sweep through.", {"corridor", "bridge", "canteen", "gym"}, {"event banner", "campus map", "statue base"}, false);
    rooms_["canteen"] = Room("canteen", "Late-Night Canteen", "Lights hum over half-empty tables. Staff routes and delivery doors make this a useful mid-run detour.", {"quad", "commons"}, {"condiment counter", "tray return", "back noticeboard"}, false);
    rooms_["gym"] = Room("gym", "Sports Hall", "Echoing courts and locker rows hide side passages toward utility areas few students use.", {"quad", "security"}, {"equipment cage", "locker row", "maintenance stairs"}, false);
    rooms_["security"] = Room("security", "Security Hub", "Monitor walls display camera feeds from restricted zones. One wrong move here ends runs quickly.", {"gym", "faculty", "admin"}, {"monitor wall", "incident log", "key locker"}, true);
    rooms_["tunnel"] = Room("tunnel", "Service Tunnel", "A narrow maintenance tunnel under campus. It is dim, risky, and almost never patrolled on schedule.", {"lab", "archive"}, {"pipe junction", "junction box", "service grate"}, true);
    rooms_["bridge"] = Room("bridge", "Footbridge", "A windy bridge between blocks. Beneath a bench, a bolted panel leads to a concealed ascent shaft.", {"quad", "rooftop"}, {"bench", "maintenance panel", "warning sign"}, false);
    rooms_["clocktower"] = Room("clocktower", "Clocktower Stairwell", "A spiraling emergency stair climbs behind the old library wing. It is silent except for the mechanical clock ticks.", {"library", "rooftop"}, {"service ladder", "clock console", "narrow landing"}, true);
    rooms_["rooftop"] = Room("rooftop", "Rooftop Exit", "The skyline opens in every direction. Reach here with the folder before the loop collapses to break free.", {"bridge", "archive", "clocktower"}, {"stair door", "radio mast", "skyline"}, false);
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
    return "Campus map\n"
           "[dorm]\n"
           "  |\n"
           "[corridor]--[lecture]--[lab]--[tunnel]--[archive]--[rooftop]\n"
           "  |            |         |         |          ^          ^\n"
           "[library]---[faculty]--[security]--[admin]---+          |\n"
           "  |                                              [clocktower]\n"
           "[clocktower]                                         ^\n"
           "  |                                                |\n"
           "[quad]----[gym]------------------------------[bridge]\n"
           "  |\n"
           "[canteen]----[commons]------------------------+";
}
