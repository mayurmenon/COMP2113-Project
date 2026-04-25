#include "map.h"
#include "utils.h"
using namespace std;
CampusMap::CampusMap() { reset(); }
void CampusMap::reset() {
    rooms_.clear();
    rooms_["dorm"] = Room("dorm", "Residence Room", "A cramped room where every loop begins. Your notes and gear are scattered from previous failed attempts.", vector<string>(), {"desk", "noticeboard", "half-packed bag", "sticky-note timeline"}, false);
    rooms_["corridor"] = Room("corridor", "Main Corridor", "The busiest path on campus. Students drift by, security cameras blink, and every doorway feels like a fork in the heist.", vector<string>(), {"posters", "door signs", "camera dome", "maintenance map"}, false);
    rooms_["lecture"] = Room("lecture", "Lecture Hall", "Rows of empty seats face a glowing projector. Whispering here carries farther than expected.", vector<string>(), {"podium", "front row", "side aisle", "under-seat vents"}, false);
    rooms_["library"] = Room("library", "Library", "Reserve shelves, terminal logs, and scribbled margin notes point to routes most students never notice.", vector<string>(), {"reserve shelf", "catalog terminal", "newspaper rack", "annotated map"}, false);
    rooms_["commons"] = Room("commons", "Learning Commons", "Open tables and group-study booths make this the easiest place to blend in and plan your next move.", vector<string>(), {"flyers", "whiteboard", "charging station", "projector cart"}, false);
    rooms_["lab"] = Room("lab", "Science Lab", "Equipment trays and locked cabinets line the walls. A maintenance hatch is half-hidden behind gas canisters.", vector<string>(), {"tool bench", "storage cabinet", "maintenance hatch", "safety locker"}, false);
    rooms_["faculty"] = Room("faculty", "Faculty Corridor", "A quiet corridor with frosted office doors and suspiciously frequent patrol routes.", vector<string>(), {"office plaques", "mail trays", "meeting board", "panic button panel"}, true);
    rooms_["admin"] = Room("admin", "Admin Office", "Forms, attendance ledgers, and archived movement records reveal how the secure wing is really connected.", vector<string>(), {"filing cabinet", "reception desk", "routing binder", "clearance ledger"}, true);
    rooms_["archive"] = Room("archive", "Archive Room", "Dusty cabinets hold the sealed folder. A coded lock and reinforced exit door protect the final objective.", vector<string>(), {"drawer bank", "coded lock", "sealed cabinet", "elevator override"}, true);
    rooms_["quad"] = Room("quad", "Central Quad", "The open campus square links major buildings. Good visibility, but little cover if guards sweep through.", vector<string>(), {"event banner", "campus map", "statue base", "service hatch"}, false);
    rooms_["canteen"] = Room("canteen", "Late-Night Canteen", "Lights hum over half-empty tables. Staff routes and delivery doors make this a useful mid-run detour.", vector<string>(), {"condiment counter", "tray return", "back noticeboard", "cold-room door"}, false);
    rooms_["gym"] = Room("gym", "Sports Hall", "Echoing courts and locker rows hide side passages toward utility areas few students use.", vector<string>(), {"equipment cage", "locker row", "maintenance stairs", "bleacher access"}, false);
    rooms_["security"] = Room("security", "Security Hub", "Monitor walls display camera feeds from restricted zones. One wrong move here ends runs quickly.", vector<string>(), {"monitor wall", "incident log", "key locker", "alert terminal"}, true);
    rooms_["tunnel"] = Room("tunnel", "Service Tunnel", "A narrow maintenance tunnel under campus. It is dim, risky, and almost never patrolled on schedule.", vector<string>(), {"pipe junction", "junction box", "service grate", "drainage ladder"}, true);
    rooms_["bridge"] = Room("bridge", "Footbridge", "A windy bridge between blocks. Beneath a bench, a bolted panel leads to a concealed ascent shaft.", vector<string>(), {"bench", "maintenance panel", "warning sign", "cable trench"}, false);
    rooms_["clocktower"] = Room("clocktower", "Clocktower Stairwell", "A spiraling emergency stair climbs behind the old library wing. It is silent except for the mechanical clock ticks.", vector<string>(), {"service ladder", "clock console", "narrow landing", "locked gate"}, true);
    rooms_["rooftop"] = Room("rooftop", "Rooftop Exit", "The skyline opens in every direction. Reach here with the folder before the loop collapses to break free.", vector<string>(), {"stair door", "radio mast", "skyline", "signal light"}, false);

    rooms_["dorm"].addRoute(Route("corridor", "residence door", false, false, "", ""));

    rooms_["corridor"].addRoute(Route("dorm", "residence hall return", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("library", "north reading wing", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("lecture", "lecture wing access", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("commons", "commons glass doors", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("quad", "central quad gate", false, false, "", ""));

    rooms_["lecture"].addRoute(Route("corridor", "main hall return", false, false, "", ""));
    rooms_["lecture"].addRoute(Route("lab", "lab side door", false, false, "", ""));

    rooms_["library"].addRoute(Route("corridor", "main stack exit", false, false, "", ""));
    rooms_["library"].addRoute(Route("faculty", "staff records wing", false, true, "", ""));
    rooms_["library"].addRoute(Route("clocktower", "clocktower service stair", true, true, "library_clue", "you have not decoded the library margin note to find that stairwell"));

    rooms_["commons"].addRoute(Route("corridor", "north doors", false, false, "", ""));
    rooms_["commons"].addRoute(Route("admin", "records office passage", false, true, "", ""));
    rooms_["commons"].addRoute(Route("canteen", "late canteen cutthrough", false, false, "", ""));

    rooms_["lab"].addRoute(Route("lecture", "lecture hall return", false, false, "", ""));
    rooms_["lab"].addRoute(Route("faculty", "faculty service door", false, true, "", ""));
    rooms_["lab"].addRoute(Route("tunnel", "maintenance hatch", true, true, "screwdriver", "the lab hatch is still bolted. you need a screwdriver"));

    rooms_["faculty"].addRoute(Route("library", "archive index wing", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("lab", "research wing return", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("admin", "admin side corridor", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("security", "security checkpoint", false, true, "keycard_or_hidden", "security hub requires cover. hide first or carry a keycard"));

    rooms_["admin"].addRoute(Route("commons", "commons return", false, true, "", ""));
    rooms_["admin"].addRoute(Route("faculty", "faculty return", false, true, "", ""));
    rooms_["admin"].addRoute(Route("archive", "secure archive door", false, true, "admin_route", "the archive wing is blocked until you confirm the route"));
    rooms_["admin"].addRoute(Route("security", "monitor room door", false, true, "keycard_or_hidden", "security hub requires cover. hide first or carry a keycard"));

    rooms_["archive"].addRoute(Route("admin", "admin corridor return", false, true, "", ""));
    rooms_["archive"].addRoute(Route("tunnel", "sublevel conduit", true, true, "screwdriver", "the tunnel grate is jammed. you need a screwdriver to force it"));
    rooms_["archive"].addRoute(Route("rooftop", "sealed stair door", false, true, "rooftop_key", "the door to the roof is locked. you need the rooftop key."));

    rooms_["quad"].addRoute(Route("corridor", "academic block entrance", false, false, "", ""));
    rooms_["quad"].addRoute(Route("bridge", "east footbridge", false, false, "", ""));
    rooms_["quad"].addRoute(Route("canteen", "canteen side lane", false, false, "", ""));
    rooms_["quad"].addRoute(Route("gym", "sports hall gate", false, false, "", ""));

    rooms_["canteen"].addRoute(Route("quad", "quad return", false, false, "", ""));
    rooms_["canteen"].addRoute(Route("commons", "study lounge passage", false, false, "", ""));

    rooms_["gym"].addRoute(Route("quad", "main court exit", false, false, "", ""));
    rooms_["gym"].addRoute(Route("security", "security side entrance", false, true, "keycard_or_hidden", "security hub requires cover. hide first or carry a keycard"));

    rooms_["security"].addRoute(Route("gym", "sports wing exit", false, true, "", ""));
    rooms_["security"].addRoute(Route("faculty", "faculty corridor door", false, true, "", ""));
    rooms_["security"].addRoute(Route("admin", "admin corridor door", false, true, "", ""));

    rooms_["tunnel"].addRoute(Route("lab", "maintenance hatch return", true, true, "screwdriver", "the lab hatch is still bolted. you need a screwdriver"));
    rooms_["tunnel"].addRoute(Route("archive", "archive grate bypass", true, true, "screwdriver", "the tunnel grate is jammed. you need a screwdriver to force it"));

    rooms_["bridge"].addRoute(Route("quad", "quad return", false, false, "", ""));
    rooms_["bridge"].addRoute(Route("rooftop", "maintenance shaft ascent", true, true, "screwdriver", "the maintenance panel is bolted shut. you need a screwdriver."));

    rooms_["clocktower"].addRoute(Route("library", "library clock stair return", true, true, "library_clue", "you have not decoded the library margin note to find that stairwell"));
    rooms_["clocktower"].addRoute(Route("rooftop", "clocktower gate", true, true, "keycard", "the clocktower gate is magnetically locked. a keycard is required."));

    rooms_["rooftop"].addRoute(Route("bridge", "bridge stair down", false, false, "", ""));
    rooms_["rooftop"].addRoute(Route("archive", "archive stairwell down", false, false, "", ""));
    rooms_["rooftop"].addRoute(Route("clocktower", "clocktower descent", false, false, "", ""));

    rooms_["dorm"].threatProfile = "safe";
    rooms_["dorm"].patrolIntensity = 0;
    rooms_["dorm"].addAmbientDetail("A muffled alarm from another room resets with each loop.");
    rooms_["dorm"].addAmbientDetail("Someone in the next room keeps practicing the same guitar riff.");
    rooms_["dorm"].addAmbientDetail("The corridor light leaks through the bottom of your door.");
    rooms_["dorm"].addAmbientDetail("Your own scribbled timeline is taped to the wardrobe.");
    rooms_["dorm"].addAmbientDetail("The campus app notification repeats yesterday's warning.");
    rooms_["dorm"].addExplorationDetail("The desk drawer has old security patrol timings from last loop.");
    rooms_["dorm"].addExplorationDetail("A folded hand sketch marks lab and archive utility lines.");
    rooms_["dorm"].addExplorationDetail("Your half-packed bag has room for one more key item.");
    rooms_["dorm"].addExplorationDetail("Sticky notes suggest the bridge path is faster but exposed.");
    rooms_["dorm"].addProgressionHint("Start with public rooms to gather low-risk items.");
    rooms_["dorm"].addProgressionHint("If runs fail, reset your route through corridor -> library first.");
    rooms_["dorm"].addProgressionHint("The heist is easiest when clue, route, and escape tool are staged early.");
    rooms_["dorm"].addProgressionHint("Track one primary path and one fallback path before leaving.");

    rooms_["corridor"].threatProfile = "watched";
    rooms_["corridor"].patrolIntensity = 1;
    rooms_["corridor"].addAmbientDetail("Camera motors click every few seconds above the noticeboard.");
    rooms_["corridor"].addAmbientDetail("Student chatter masks quiet footsteps if you move with the crowd.");
    rooms_["corridor"].addAmbientDetail("A custodian cart blocks one section before drifting away.");
    rooms_["corridor"].addAmbientDetail("The overhead lights flicker near the lecture wing.");
    rooms_["corridor"].addAmbientDetail("Patrol shoes echo louder after noon loops.");
    rooms_["corridor"].addExplorationDetail("Door sign updates reveal which wings become restricted later.");
    rooms_["corridor"].addExplorationDetail("A folded events map shows fastest access to the quad.");
    rooms_["corridor"].addExplorationDetail("One camera has a blind edge near a vending alcove.");
    rooms_["corridor"].addExplorationDetail("Public boards often hide forged hall passes.");
    rooms_["corridor"].addProgressionHint("This is your safest transit spine to branch into each objective.");
    rooms_["corridor"].addProgressionHint("Rotate between library and commons to avoid repeated patrol contact.");
    rooms_["corridor"].addProgressionHint("Use corridor to recover pace before entering restricted wings.");

    rooms_["lecture"].threatProfile = "low";
    rooms_["lecture"].patrolIntensity = 1;
    rooms_["lecture"].addAmbientDetail("The projector fan hums in an otherwise empty hall.");
    rooms_["lecture"].addAmbientDetail("Rows of chairs create cover lines from the side door.");
    rooms_["lecture"].addAmbientDetail("A cleaning staff schedule is still pinned near the podium.");
    rooms_["lecture"].addAmbientDetail("An emergency exit light glows toward the lab passage.");
    rooms_["lecture"].addAmbientDetail("Old lecture slides show facilities floor plans by accident.");
    rooms_["lecture"].addExplorationDetail("The podium drawer has outdated but useful room labels.");
    rooms_["lecture"].addExplorationDetail("Under-seat vents carry noise from nearby corridors.");
    rooms_["lecture"].addExplorationDetail("A side aisle lets you reposition without crossing open floor.");
    rooms_["lecture"].addExplorationDetail("The hall is good for regaining stealth rhythm before lab.");
    rooms_["lecture"].addProgressionHint("Use lecture as a low-risk transition into lab routes.");
    rooms_["lecture"].addProgressionHint("If suspicion is high, avoid lingering and move through fast.");
    rooms_["lecture"].addProgressionHint("Lab access from here supports both direct and hidden progression.");

    rooms_["library"].threatProfile = "research";
    rooms_["library"].patrolIntensity = 1;
    rooms_["library"].addAmbientDetail("The reference desk printer keeps spitting the same queue ticket.");
    rooms_["library"].addAmbientDetail("Floor lamps dim whenever staff pass through reserve stacks.");
    rooms_["library"].addAmbientDetail("Card scanners beep softly from the periodicals wing.");
    rooms_["library"].addAmbientDetail("Clocktower ticks are faint behind the old architecture wall.");
    rooms_["library"].addAmbientDetail("Catalog terminals auto-lock after short idle windows.");
    rooms_["library"].addExplorationDetail("Reserve shelf notes provide the first reliable clue path.");
    rooms_["library"].addExplorationDetail("Annotated margins mention the clocktower service stair.");
    rooms_["library"].addExplorationDetail("Terminal logs map admin references to archive paperwork.");
    rooms_["library"].addExplorationDetail("A map annotation hints faculty wing patrol timing.");
    rooms_["library"].addProgressionHint("Secure library clue before attempting clocktower or admin routes.");
    rooms_["library"].addProgressionHint("This room unlocks strategic routing knowledge for late game.");
    rooms_["library"].addProgressionHint("Re-searching library can still reveal timing-safe transitions.");

    rooms_["commons"].threatProfile = "public";
    rooms_["commons"].patrolIntensity = 1;
    rooms_["commons"].addAmbientDetail("Conversation noise gives you cover from isolated surveillance.");
    rooms_["commons"].addAmbientDetail("A digital noticeboard cycles society events and room bookings.");
    rooms_["commons"].addAmbientDetail("Power strips spark occasionally along the north tables.");
    rooms_["commons"].addAmbientDetail("Night staff cross here between canteen and admin.");
    rooms_["commons"].addAmbientDetail("Study booths create temporary blind spots at the edges.");
    rooms_["commons"].addExplorationDetail("A whiteboard sketch includes an outdated admin map.");
    rooms_["commons"].addExplorationDetail("Flyers sometimes conceal forged hall pass stashes.");
    rooms_["commons"].addExplorationDetail("Charging station logs show when staff devices leave admin.");
    rooms_["commons"].addExplorationDetail("The canteen cutthrough is a quiet detour when corridor heats up.");
    rooms_["commons"].addProgressionHint("Use commons to set up admin entry with lower initial suspicion.");
    rooms_["commons"].addProgressionHint("Canteen detours can reduce repeated restricted exposure.");
    rooms_["commons"].addProgressionHint("This room is ideal for pacing before archive attempts.");

    rooms_["lab"].threatProfile = "tooling";
    rooms_["lab"].patrolIntensity = 2;
    rooms_["lab"].addAmbientDetail("Gas canisters rattle whenever someone opens the hatch panel.");
    rooms_["lab"].addAmbientDetail("Safety posters partially cover an old maintenance route map.");
    rooms_["lab"].addAmbientDetail("A lab timer alarm repeats every hour in looped rhythm.");
    rooms_["lab"].addAmbientDetail("Storage cabinets slam shut from draft pressure.");
    rooms_["lab"].addAmbientDetail("A metallic smell gets stronger near the service hatch.");
    rooms_["lab"].addExplorationDetail("Tool benches are your best source for screwdriver upgrades.");
    rooms_["lab"].addExplorationDetail("The maintenance hatch opens hidden progression into tunnel.");
    rooms_["lab"].addExplorationDetail("Cabinet labels reference sublevel conduit IDs.");
    rooms_["lab"].addExplorationDetail("Research logs mention patrol checks near faculty doors.");
    rooms_["lab"].addProgressionHint("Acquire screwdriver here before attempting tunnel/bridge escapes.");
    rooms_["lab"].addProgressionHint("Lab gives alternate heist tempo if admin route is crowded.");
    rooms_["lab"].addProgressionHint("Tool acquisition here enables two independent escape branches.");

    rooms_["faculty"].threatProfile = "restricted";
    rooms_["faculty"].patrolIntensity = 3;
    rooms_["faculty"].addAmbientDetail("Footsteps slow noticeably near office doors after hours.");
    rooms_["faculty"].addAmbientDetail("Mail trays are sorted by departments tied to admin records.");
    rooms_["faculty"].addAmbientDetail("Patrol radios crackle near the end of each corridor cycle.");
    rooms_["faculty"].addAmbientDetail("Security mirrors give broad sight lines toward lab exits.");
    rooms_["faculty"].addAmbientDetail("Meeting boards show temporary access policy changes.");
    rooms_["faculty"].addExplorationDetail("Office plaques map which keys control security checkpoints.");
    rooms_["faculty"].addExplorationDetail("Mail routing reveals who signs archive transfer papers.");
    rooms_["faculty"].addExplorationDetail("A side corridor gives optional entry into security hub.");
    rooms_["faculty"].addExplorationDetail("Restricted exposure here can spike suspicion quickly.");
    rooms_["faculty"].addProgressionHint("Enter only with purpose: keycard lead, admin transit, or security check.");
    rooms_["faculty"].addProgressionHint("Hall pass or hiding helps offset restricted penalties.");
    rooms_["faculty"].addProgressionHint("This is the hinge between safe and high-stakes map sectors.");

    rooms_["admin"].threatProfile = "restricted";
    rooms_["admin"].patrolIntensity = 3;
    rooms_["admin"].addAmbientDetail("Printers periodically dump attendance sheets and route codes.");
    rooms_["admin"].addAmbientDetail("Reception cameras pivot toward archive wing doors.");
    rooms_["admin"].addAmbientDetail("A desk fan masks soft keypad beeps from secure terminals.");
    rooms_["admin"].addAmbientDetail("Clipboard stacks indicate active nightly security checks.");
    rooms_["admin"].addAmbientDetail("Clearance cards on lanyards disappear as loops progress.");
    rooms_["admin"].addExplorationDetail("Filing cabinets contain route clues to unlock archive access.");
    rooms_["admin"].addExplorationDetail("Routing binders validate your adminRoute progression flag.");
    rooms_["admin"].addExplorationDetail("Code fragments can be discovered through records cross-checks.");
    rooms_["admin"].addExplorationDetail("Security door here offers high-risk alternate branching.");
    rooms_["admin"].addProgressionHint("Confirm admin route before direct archive attempts.");
    rooms_["admin"].addProgressionHint("Use keycard to reduce suspicion pressure while gathering fragments.");
    rooms_["admin"].addProgressionHint("Admin is where the heist turns from setup into execution.");

    rooms_["archive"].threatProfile = "critical";
    rooms_["archive"].patrolIntensity = 4;
    rooms_["archive"].addAmbientDetail("Metal cabinets creak even when untouched.");
    rooms_["archive"].addAmbientDetail("The coded lock flashes warning patterns between attempts.");
    rooms_["archive"].addAmbientDetail("A reinforced stair door seals toward rooftop route.");
    rooms_["archive"].addAmbientDetail("Air vents carry tunnel drafts from beneath the floor.");
    rooms_["archive"].addAmbientDetail("A silent camera dome tracks movement around the cabinet row.");
    rooms_["archive"].addExplorationDetail("The sealed folder is hidden behind coded cabinet logic.");
    rooms_["archive"].addExplorationDetail("Code fragments combine here to produce rooftop key access.");
    rooms_["archive"].addExplorationDetail("Tunnel bypass provides alternate entry under high pressure.");
    rooms_["archive"].addExplorationDetail("Time spent here rapidly raises attention and risk.");
    rooms_["archive"].addProgressionHint("Archive is the final objective room: enter prepared, exit quickly.");
    rooms_["archive"].addProgressionHint("Bring route knowledge and key items before committing.");
    rooms_["archive"].addProgressionHint("Once folder is secured, prioritize nearest viable escape route.");
    rooms_["archive"].addProgressionHint("If risk spikes, tunnel or bridge exits can salvage the run.");

    rooms_["quad"].threatProfile = "open";
    rooms_["quad"].patrolIntensity = 1;
    rooms_["quad"].addAmbientDetail("Open air carries announcements from multiple buildings.");
    rooms_["quad"].addAmbientDetail("Guard routes cross here in visible but predictable patterns.");
    rooms_["quad"].addAmbientDetail("Student groups cluster near the central statue at dusk.");
    rooms_["quad"].addAmbientDetail("Delivery carts briefly block line-of-sight near canteen lane.");
    rooms_["quad"].addAmbientDetail("Gym floodlights brighten one side of the square.");
    rooms_["quad"].addExplorationDetail("Campus map signage helps optimize multi-room route loops.");
    rooms_["quad"].addExplorationDetail("Bridge access from here enables fast late-game exits.");
    rooms_["quad"].addExplorationDetail("Gym and canteen branches provide safer reroute options.");
    rooms_["quad"].addExplorationDetail("Open visibility makes it easier to avoid accidental patrol collision.");
    rooms_["quad"].addProgressionHint("Use quad as a route balancing hub between risk and speed.");
    rooms_["quad"].addProgressionHint("Bridge branch is fast; gym branch is safer for resets.");
    rooms_["quad"].addProgressionHint("A good quad transition preserves time for archive execution.");

    rooms_["canteen"].threatProfile = "low";
    rooms_["canteen"].patrolIntensity = 1;
    rooms_["canteen"].addAmbientDetail("Kitchen extractors hum above mostly empty tables.");
    rooms_["canteen"].addAmbientDetail("Tray carts shield movement near the service corridor.");
    rooms_["canteen"].addAmbientDetail("Staff shift logs are pinned near the freezer door.");
    rooms_["canteen"].addAmbientDetail("A back noticeboard tracks deliveries to restricted wings.");
    rooms_["canteen"].addAmbientDetail("Cleaning schedules repeat at nearly identical loop times.");
    rooms_["canteen"].addExplorationDetail("Hall passes can surface here among posted notices.");
    rooms_["canteen"].addExplorationDetail("Delivery timings hint when admin receives unattended parcels.");
    rooms_["canteen"].addExplorationDetail("Cutthrough route helps avoid heavily watched corridor moments.");
    rooms_["canteen"].addExplorationDetail("The room offers low-risk recovery after restricted runs.");
    rooms_["canteen"].addProgressionHint("Use canteen as a pressure-release node in long runs.");
    rooms_["canteen"].addProgressionHint("If suspicion spikes, regroup here before next restricted entry.");
    rooms_["canteen"].addProgressionHint("Canteen path pairs well with commons-led progression.");

    rooms_["gym"].threatProfile = "medium";
    rooms_["gym"].patrolIntensity = 2;
    rooms_["gym"].addAmbientDetail("Echoes make footsteps difficult to localize from distance.");
    rooms_["gym"].addAmbientDetail("Locker rows create short visual barriers along the walls.");
    rooms_["gym"].addAmbientDetail("A maintenance stairwell links toward security access points.");
    rooms_["gym"].addAmbientDetail("Floodlights switch modes at predictable intervals.");
    rooms_["gym"].addAmbientDetail("Equipment cages rattle when patrols check the area.");
    rooms_["gym"].addExplorationDetail("Gym branch offers an alternate approach into security hub.");
    rooms_["gym"].addExplorationDetail("Bleacher shadows can briefly help with stealth positioning.");
    rooms_["gym"].addExplorationDetail("This path is slower than corridor but can be safer.");
    rooms_["gym"].addExplorationDetail("Guard habits here are cyclic and learnable across loops.");
    rooms_["gym"].addProgressionHint("Gym route is useful when faculty corridor is too hot.");
    rooms_["gym"].addProgressionHint("Combine hiding and movement timing before security entry.");
    rooms_["gym"].addProgressionHint("This branch supports controlled, low-chaos progression.");

    rooms_["security"].threatProfile = "critical";
    rooms_["security"].patrolIntensity = 4;
    rooms_["security"].addAmbientDetail("Monitor walls show moving views of every restricted sector.");
    rooms_["security"].addAmbientDetail("Card readers flash red until valid credentials are swiped.");
    rooms_["security"].addAmbientDetail("Radio chatter increases with every suspicious movement spike.");
    rooms_["security"].addAmbientDetail("Incident logs are updated in strict ten-minute intervals.");
    rooms_["security"].addAmbientDetail("A locker unit stores emergency bypass cards.");
    rooms_["security"].addExplorationDetail("Searching here can reveal keycard-grade route control intel.");
    rooms_["security"].addExplorationDetail("Hub access improves pathing decisions in admin/faculty wings.");
    rooms_["security"].addExplorationDetail("Movement through this room is high-risk even when prepared.");
    rooms_["security"].addExplorationDetail("Hidden approach or keycard is strongly recommended.");
    rooms_["security"].addProgressionHint("Enter only with stealth state or keycard support.");
    rooms_["security"].addProgressionHint("Use hub information quickly; prolonged stay is dangerous.");
    rooms_["security"].addProgressionHint("Security can accelerate heist completion or trigger rapid failure.");

    rooms_["tunnel"].threatProfile = "hidden";
    rooms_["tunnel"].patrolIntensity = 3;
    rooms_["tunnel"].addAmbientDetail("Pipes knock in rhythm with distant boiler pulses.");
    rooms_["tunnel"].addAmbientDetail("Condensation drips make footing uneven and loud.");
    rooms_["tunnel"].addAmbientDetail("Airflow patterns indicate the nearest hatch exits.");
    rooms_["tunnel"].addAmbientDetail("Emergency strip lights fail intermittently.");
    rooms_["tunnel"].addAmbientDetail("Metal grates vibrate when someone crosses archive stairs above.");
    rooms_["tunnel"].addExplorationDetail("Tunnel bypass can skip standard admin-to-archive pressure.");
    rooms_["tunnel"].addExplorationDetail("Code fragments may appear in service boxes and utility logs.");
    rooms_["tunnel"].addExplorationDetail("Route requires screwdriver at both entry and archive grate.");
    rooms_["tunnel"].addExplorationDetail("This is a hidden, risky branch for flexible progression.");
    rooms_["tunnel"].addProgressionHint("Use tunnel when direct restricted corridors are too costly.");
    rooms_["tunnel"].addProgressionHint("Do not enter without tool readiness and exit plan.");
    rooms_["tunnel"].addProgressionHint("Tunnel path can salvage near-failed runs with fast repositioning.");

    rooms_["bridge"].threatProfile = "exposed";
    rooms_["bridge"].patrolIntensity = 2;
    rooms_["bridge"].addAmbientDetail("Wind noise masks short bursts of movement.");
    rooms_["bridge"].addAmbientDetail("Open sight lines make you easy to spot if you hesitate.");
    rooms_["bridge"].addAmbientDetail("A bolted panel sits under the bench near warning tape.");
    rooms_["bridge"].addAmbientDetail("Security lights from rooftops sweep this crossing intermittently.");
    rooms_["bridge"].addAmbientDetail("The metal railing hums from the city draft.");
    rooms_["bridge"].addExplorationDetail("Screwdriver unlocks hidden maintenance shaft to rooftop.");
    rooms_["bridge"].addExplorationDetail("Bridge route is one of the fastest endgame escapes.");
    rooms_["bridge"].addExplorationDetail("Without tool prep, this branch is a dead end.");
    rooms_["bridge"].addExplorationDetail("Exposure risk is high, so commit only when ready.");
    rooms_["bridge"].addProgressionHint("Bridge is your speed route once screwdriver is secured.");
    rooms_["bridge"].addProgressionHint("Time this branch for final extraction with folder in hand.");
    rooms_["bridge"].addProgressionHint("Fast but exposed: use when suspicion remains manageable.");
    rooms_["bridge"].addProgressionHint("Avoid bridge stalls; commit to movement once on the span.");

    rooms_["clocktower"].threatProfile = "hidden-critical";
    rooms_["clocktower"].patrolIntensity = 3;
    rooms_["clocktower"].addAmbientDetail("Clock gears grind behind stone walls in heavy pulses.");
    rooms_["clocktower"].addAmbientDetail("Narrow steps amplify every misplaced footstep.");
    rooms_["clocktower"].addAmbientDetail("A magnetic gate blocks rooftop ascent without credentials.");
    rooms_["clocktower"].addAmbientDetail("Dust trails reveal infrequent but possible patrol checks.");
    rooms_["clocktower"].addAmbientDetail("The stairwell is silent except for mechanical ticks.");
    rooms_["clocktower"].addExplorationDetail("Library clue reveals this hidden high-risk rooftop branch.");
    rooms_["clocktower"].addExplorationDetail("Keycard is required to pass the final gate.");
    rooms_["clocktower"].addExplorationDetail("Route is stealthy but unforgiving if unprepared.");
    rooms_["clocktower"].addExplorationDetail("Useful as alternate extraction when archive stair is blocked.");
    rooms_["clocktower"].addProgressionHint("Treat clocktower as backup escape for well-prepared runs.");
    rooms_["clocktower"].addProgressionHint("Do not route here until clue and keycard are secured.");
    rooms_["clocktower"].addProgressionHint("This branch increases heist depth and alternate completion paths.");
    rooms_["clocktower"].addProgressionHint("Clocktower is slower than bridge but safer from open sight lines.");

    rooms_["rooftop"].threatProfile = "escape";
    rooms_["rooftop"].patrolIntensity = 1;
    rooms_["rooftop"].addAmbientDetail("The skyline opens in every direction above the campus blocks.");
    rooms_["rooftop"].addAmbientDetail("Signal lights blink as if synced to the time loop itself.");
    rooms_["rooftop"].addAmbientDetail("Wind carries distant sirens from the city edge.");
    rooms_["rooftop"].addAmbientDetail("Metal doors from archive, bridge, and clocktower converge here.");
    rooms_["rooftop"].addAmbientDetail("Every completed route feels different, but ends in the same sky.");
    rooms_["rooftop"].addExplorationDetail("Arrival with folder immediately fulfills the loopbreak condition.");
    rooms_["rooftop"].addExplorationDetail("Multiple ingress points make this the core extraction node.");
    rooms_["rooftop"].addExplorationDetail("Reaching here proves progression and route planning worked.");
    rooms_["rooftop"].addExplorationDetail("The final sequence rewards whichever escape branch you prepared.");
    rooms_["rooftop"].addProgressionHint("All heist branches should converge here with the folder.");
    rooms_["rooftop"].addProgressionHint("Plan extraction before entering archive to avoid rushed failure.");
    rooms_["rooftop"].addProgressionHint("A complete run links clue, route, key items, folder, and rooftop.");
    rooms_["rooftop"].addProgressionHint("Successful progression is measured by both completion and control.");
}
const Room* CampusMap::get(const string& roomId) const {
    map<string, Room>::const_iterator found = rooms_.find(roomId);
    return found == rooms_.end() ? nullptr : &found->second;
}
bool CampusMap::canMove(const string& from, const string& to) const {
    const Room* room = get(from);
    if (!room) return false;
    return room->hasRouteTo(to);
}
bool CampusMap::requirementMet(const string& requirement, const MovementContext& context) const {
    if (requirement.empty()) return true;
    if (requirement == "library_clue") return context.libraryClue;
    if (requirement == "admin_route") return context.adminRoute;
    if (requirement == "keycard") return context.hasKeycard;
    if (requirement == "screwdriver") return context.hasScrewdriver;
    if (requirement == "rooftop_key") return context.hasRooftopKey;
    if (requirement == "keycard_or_hidden") return context.hasKeycard || context.hidden;
    return false;
}
MoveCheck CampusMap::validateMove(const string& from, const string& to, const MovementContext& context) const {
    const Room* room = get(from);
    if (!room) return MoveCheck(false, false, false, "your current room is invalid");
    const Route* route = room->routeTo(to);
    if (!route) return MoveCheck(false, false, false, "you cannot move there from here");
    if (!requirementMet(route->requirement, context)) {
        if (!route->blockedMessage.empty()) return MoveCheck(false, route->hidden, route->risky, route->blockedMessage);
        return MoveCheck(false, route->hidden, route->risky, "that route is locked");
    }
    return MoveCheck(true, route->hidden, route->risky, "");
}
string CampusMap::exitsOf(const string& roomId) const {
    const Room* room = get(roomId);
    if (!room) return "";
    return Utils::join(room->routeTargets(false), ", ");
}
string CampusMap::exitsOf(const string& roomId, const MovementContext& context) const {
    const Room* room = get(roomId);
    if (!room) return "";
    vector<string> labels;
    for (int i = 0; i < (int)room->routes.size(); i++) {
        const Route& route = room->routes[i];
        bool unlocked = requirementMet(route.requirement, context);
        string label = route.to;
        if (route.hidden) label += " [hidden]";
        if (route.risky) label += " [risky]";
        if (!unlocked) label += " [locked]";
        labels.push_back(label);
    }
    return Utils::join(labels, ", ");
}
vector<string> CampusMap::availableEscapeRoutes(const MovementContext& context) const {
    vector<string> routes;
    if (requirementMet("rooftop_key", context)) {
        routes.push_back("archive stair route");
    }
    if (requirementMet("screwdriver", context)) {
        routes.push_back("bridge maintenance shaft");
    }
    if (requirementMet("library_clue", context) && requirementMet("keycard", context)) {
        routes.push_back("clocktower emergency stair");
    }
    return routes;
}
string CampusMap::progressionSummary(const MovementContext& context, bool hasFolder) const {
    if (!context.libraryClue) return "stage 1: gather intelligence in public academic zones";
    if (!context.adminRoute) return "stage 2: decode admin route access to archive wing";
    if (!hasFolder) return "stage 3: penetrate archive and retrieve sealed objective";
    vector<string> routes = availableEscapeRoutes(context);
    if (routes.empty()) return "stage 4: objective secured, but no rooftop route unlocked";
    if ((int)routes.size() == 1) return "stage 4: objective secured, one extraction route available";
    if ((int)routes.size() == 2) return "stage 4: objective secured, two extraction routes available";
    return "stage 4: objective secured, all extraction routes available";
}
vector<string> CampusMap::roomDossier(const string& roomId) const {
    vector<string> notes;
    if (roomId == "dorm") {
        notes.push_back("safehouse: low patrol, best planning point");
        notes.push_back("priority: confirm next branch before stepping out");
        notes.push_back("risk: none, but no direct progression items");
        notes.push_back("route role: start/end reset anchor");
    } else if (roomId == "corridor") {
        notes.push_back("safe transit spine with light camera coverage");
        notes.push_back("priority: branch to library/commons with low risk");
        notes.push_back("risk: moderate if repeatedly crossed");
        notes.push_back("route role: central connector for all early game loops");
    } else if (roomId == "lecture") {
        notes.push_back("low-pressure staging node toward lab");
        notes.push_back("priority: use as calm transition into tool route");
        notes.push_back("risk: low, but sparse hiding cover");
        notes.push_back("route role: buffer between public and restricted sectors");
    } else if (roomId == "library") {
        notes.push_back("intel hub: unlocks clue and clocktower logic");
        notes.push_back("priority: secure clue before restricted route attempts");
        notes.push_back("risk: low to medium depending on loop timing");
        notes.push_back("route role: progression gate for hidden stair branch");
    } else if (roomId == "commons") {
        notes.push_back("public staging area with flexible detours");
        notes.push_back("priority: prep admin entry with lower suspicion");
        notes.push_back("risk: low, useful for reset pacing");
        notes.push_back("route role: canteen/admin connector");
    } else if (roomId == "lab") {
        notes.push_back("tool acquisition zone for hidden-route unlocks");
        notes.push_back("priority: find screwdriver early");
        notes.push_back("risk: medium due to semi-restricted access");
        notes.push_back("route role: gateway to tunnel branch");
    } else if (roomId == "faculty") {
        notes.push_back("restricted hinge with frequent patrol paths");
        notes.push_back("priority: keep movement efficient and purposeful");
        notes.push_back("risk: high without keycard support");
        notes.push_back("route role: bridge between library/lab/admin/security");
    } else if (roomId == "admin") {
        notes.push_back("restricted records node for archive route unlock");
        notes.push_back("priority: secure adminRoute state and fragments");
        notes.push_back("risk: high, surveillance concentrated");
        notes.push_back("route role: direct gateway into archive objective room");
    } else if (roomId == "archive") {
        notes.push_back("critical objective room with severe lock pressure");
        notes.push_back("priority: retrieve folder and exit immediately");
        notes.push_back("risk: very high, prolonged stay is dangerous");
        notes.push_back("route role: final heist objective before extraction");
    } else if (roomId == "quad") {
        notes.push_back("open-area routing hub between campus sectors");
        notes.push_back("priority: rotate branches without repeated corridor load");
        notes.push_back("risk: low to medium due to exposure");
        notes.push_back("route role: bridge/canteen/gym dispatcher");
    } else if (roomId == "canteen") {
        notes.push_back("low-risk recovery and detour room");
        notes.push_back("priority: regroup if suspicion starts climbing");
        notes.push_back("risk: low, occasional pass item opportunities");
        notes.push_back("route role: commons-to-quad safety branch");
    } else if (roomId == "gym") {
        notes.push_back("alternate approach corridor into security wing");
        notes.push_back("priority: use for route diversity under pressure");
        notes.push_back("risk: medium, echoes can reveal movement");
        notes.push_back("route role: secondary security approach");
    } else if (roomId == "security") {
        notes.push_back("critical surveillance core requiring stealth support");
        notes.push_back("priority: enter with keycard or hidden status");
        notes.push_back("risk: very high, mistakes escalate quickly");
        notes.push_back("route role: optional high-risk progression accelerator");
    } else if (roomId == "tunnel") {
        notes.push_back("hidden sublevel route for bypass strategies");
        notes.push_back("priority: only enter after screwdriver unlock");
        notes.push_back("risk: high due to constrained movement");
        notes.push_back("route role: alternate archive ingress/egress");
    } else if (roomId == "bridge") {
        notes.push_back("fast extraction branch with high visual exposure");
        notes.push_back("priority: commit only after screwdriver prep");
        notes.push_back("risk: medium-high from open sight lines");
        notes.push_back("route role: speed-focused rooftop path");
    } else if (roomId == "clocktower") {
        notes.push_back("hidden extraction branch requiring clue plus keycard");
        notes.push_back("priority: reserve as backup/alternate rooftop route");
        notes.push_back("risk: high due to narrow, locked ascent");
        notes.push_back("route role: deep progression reward path");
    } else if (roomId == "rooftop") {
        notes.push_back("final extraction node; all successful routes converge");
        notes.push_back("priority: arrive with folder before reset conditions");
        notes.push_back("risk: low once reached, objective determines success");
        notes.push_back("route role: completion trigger for loopbreak");
    }
    return notes;
}
string CampusMap::layout() const {
    return "Campus map\n"
           "[dorm]\n"
           "  |\n"
           "[corridor]--[lecture]--[lab]===([tunnel])===[archive]--[rooftop]\n"
           "  |            |         |         |          ^          ^\n"
           "[library]---[faculty]--[security]--[admin]---+          |\n"
           " ((clocktower))                                      ^\n"
           "  |                                                  |\n"
           " [hidden stair]                                      |\n"
           "  |                                                |\n"
           "[quad]----[gym]------------------------------[bridge]\n"
           "  |\n"
           "[canteen]----[commons]------------------------+";
}
