#include "map.h"
#include "utils.h"
using namespace std;
CampusMap::CampusMap() { reset(); }
void CampusMap::reset() {
    rooms_.clear();
    rooms_["dorm"] = Room("dorm", "Swire Hall Room", "A small hall room above the Main Campus slope. Your desk is covered with notes from earlier loops that all ended too late.", vector<string>(), {"desk lamp", "hall notice", "half-packed bag", "folded route sketch"}, false);
    rooms_["corridor"] = Room("corridor", "University Street", "The long HKU spine between Main Campus and Centennial Campus. Cameras, students, and late walkers make it busy, but not impossible to blend into.", vector<string>(), {"direction sign", "campus noticeboard", "camera dome", "route map"}, false);
    rooms_["lecture"] = Room("lecture", "Grand Hall", "The Grand Hall foyer sits quiet after the day's events. Wide steps and side doors make it a useful early crossing point.", vector<string>(), {"usher desk", "seat row", "side stair", "backstage door"}, false);
    rooms_["library"] = Room("library", "Main Library", "HKU's Main Library sits between the old and new parts of campus life. The clue you need is buried somewhere among its shelves and terminals.", vector<string>(), {"reserve shelf", "old wing terminal", "book return", "level 3 notice"}, false);
    rooms_["commons"] = Room("commons", "Chi Wah Learning Commons", "Chi Wah is bright, open, and easy to disappear into. Booth seats, printers, and the spiral stair make it a good place to plan the next move.", vector<string>(), {"spiral stair", "group booth", "printer corner", "booking screen"}, false);
    rooms_["lab"] = Room("lab", "Kadoorie Biological Sciences Building", "The Kadoorie labs still smell faintly of solvents and steel. A service hatch near the back wall matters more tonight than any experiment bench.", vector<string>(), {"tool bench", "specimen cabinet", "service hatch", "lab locker"}, false);
    rooms_["faculty"] = Room("faculty", "Main Building Corridor", "The old Main Building corridor is lined with doors, plaques, and corners that carry sound. It feels historic, watched, and harder to cross cleanly.", vector<string>(), {"office plaques", "courtyard shutters", "heritage board", "bust niche"}, true);
    rooms_["admin"] = Room("admin", "Main Building Office", "Forms, ledgers, and routing sheets from the Main Building office point toward the records room and the roof route beyond it.", vector<string>(), {"routing binder", "staff pigeonholes", "reception desk", "access forms"}, true);
    rooms_["archive"] = Room("archive", "Main Building Records Room", "Old HKU records, sealed trays, and a locked stair door sit under a layer of dust. The file you need is here, but this is where the run turns dangerous.", vector<string>(), {"old wing shelves", "transfer box", "sealed file tray", "roof keypad"}, true);
    rooms_["quad"] = Room("quad", "Centennial Campus Courtyard", "The Centennial Campus courtyard is open, bright, and exposed. It links Chi Wah, the footbridge, and the longer branches of the run.", vector<string>(), {"courtyard bench", "campus map", "lily pond sign", "event poster"}, false);
    rooms_["canteen"] = Room("canteen", "Cafe 330", "Cafe 330 is quieter after the rush. It is a good place to breathe, check your notes, and move without too much attention.", vector<string>(), {"tray rack", "closing board", "receipt bin", "vending shelf"}, false);
    rooms_["gym"] = Room("gym", "Sports Centre Hall", "The sports hall is large, echoing, and harder to read than it first looks. It is not the fastest branch, but it opens another way toward security.", vector<string>(), {"equipment cage", "locker row", "side stair", "bleacher gap"}, false);
    rooms_["security"] = Room("security", "East Gate Security Office", "Monitors, incident logs, and access readers make this one of the most dangerous rooms on campus. It can help a run, but it can end one just as fast.", vector<string>(), {"monitor wall", "incident log", "key locker", "access reader"}, true);
    rooms_["tunnel"] = Room("tunnel", "Centennial Service Tunnel", "A narrow service tunnel runs below the campus edge. It is dim and ugly, but it can cut around the cleaner public route when you are prepared.", vector<string>(), {"pipe junction", "service grate", "junction box", "ladder rung"}, true);
    rooms_["bridge"] = Room("bridge", "University Street Footbridge", "The footbridge is open to wind and easy to watch from a distance. Beneath one bench, a bolted panel hides a much faster late-game route.", vector<string>(), {"bench", "bolted panel", "warning strip", "railing gap"}, false);
    rooms_["clocktower"] = Room("clocktower", "Main Building Clock Tower", "The Main Building clock tower stair is narrow, old, and easy to misread. It is a strong backup route, but only if you are ready for it.", vector<string>(), {"old steps", "gate reader", "bell housing", "narrow landing"}, true);
    rooms_["rooftop"] = Room("rooftop", "Main Building Rooftop", "The roof above the Main Building opens over Pokfulam and the rest of campus. Reach it with the file and the loop finally gives way.", vector<string>(), {"roof door", "signal light", "low wall", "city skyline"}, false);

    rooms_["dorm"].addRoute(Route("corridor", "hall door to University Street", false, false, "", ""));

    rooms_["corridor"].addRoute(Route("dorm", "return to Swire Hall", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("library", "Main Library entrance", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("lecture", "Grand Hall approach", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("commons", "Chi Wah entrance", false, false, "", ""));
    rooms_["corridor"].addRoute(Route("quad", "Centennial courtyard gate", false, false, "", ""));

    rooms_["lecture"].addRoute(Route("corridor", "back to University Street", false, false, "", ""));
    rooms_["lecture"].addRoute(Route("lab", "Kadoorie side door", false, false, "", ""));

    rooms_["library"].addRoute(Route("corridor", "back to University Street", false, false, "", ""));
    rooms_["library"].addRoute(Route("faculty", "Main Building side corridor", false, true, "", ""));
    rooms_["library"].addRoute(Route("clocktower", "clock tower service stair", true, true, "library_clue", "you still have not read enough of the Main Library clue to find that stair"));

    rooms_["commons"].addRoute(Route("corridor", "University Street doors", false, false, "", ""));
    rooms_["commons"].addRoute(Route("admin", "Main Building office passage", false, true, "", ""));
    rooms_["commons"].addRoute(Route("canteen", "Cafe 330 cutthrough", false, false, "", ""));

    rooms_["lab"].addRoute(Route("lecture", "Grand Hall return", false, false, "", ""));
    rooms_["lab"].addRoute(Route("faculty", "Main Building service door", false, true, "", ""));
    rooms_["lab"].addRoute(Route("tunnel", "Kadoorie service hatch", true, true, "screwdriver", "the Kadoorie hatch is still bolted. you need a screwdriver"));

    rooms_["faculty"].addRoute(Route("library", "library side return", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("lab", "Kadoorie return", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("admin", "office side corridor", false, true, "", ""));
    rooms_["faculty"].addRoute(Route("security", "East Gate checkpoint", false, true, "keycard_or_hidden", "East Gate security needs cover. hide first or carry a keycard"));

    rooms_["admin"].addRoute(Route("commons", "Chi Wah return", false, true, "", ""));
    rooms_["admin"].addRoute(Route("faculty", "Main Building return", false, true, "", ""));
    rooms_["admin"].addRoute(Route("archive", "records room door", false, true, "admin_route", "the records room is still locked until you map the route"));
    rooms_["admin"].addRoute(Route("security", "security side door", false, true, "keycard_or_hidden", "East Gate security needs cover. hide first or carry a keycard"));

    rooms_["archive"].addRoute(Route("admin", "office corridor return", false, true, "", ""));
    rooms_["archive"].addRoute(Route("tunnel", "service conduit", true, true, "screwdriver", "the service grate is still jammed. you need a screwdriver"));
    rooms_["archive"].addRoute(Route("rooftop", "Main Building roof stair", false, true, "rooftop_key", "the roof stair is still locked. you need the rooftop key"));

    rooms_["quad"].addRoute(Route("corridor", "University Street return", false, false, "", ""));
    rooms_["quad"].addRoute(Route("bridge", "footbridge access", false, false, "", ""));
    rooms_["quad"].addRoute(Route("canteen", "Cafe 330 lane", false, false, "", ""));
    rooms_["quad"].addRoute(Route("gym", "sports hall gate", false, false, "", ""));

    rooms_["canteen"].addRoute(Route("quad", "courtyard return", false, false, "", ""));
    rooms_["canteen"].addRoute(Route("commons", "Chi Wah passage", false, false, "", ""));

    rooms_["gym"].addRoute(Route("quad", "courtyard exit", false, false, "", ""));
    rooms_["gym"].addRoute(Route("security", "security side entrance", false, true, "keycard_or_hidden", "East Gate security needs cover. hide first or carry a keycard"));

    rooms_["security"].addRoute(Route("gym", "sports hall exit", false, true, "", ""));
    rooms_["security"].addRoute(Route("faculty", "Main Building door", false, true, "", ""));
    rooms_["security"].addRoute(Route("admin", "office door", false, true, "", ""));

    rooms_["tunnel"].addRoute(Route("lab", "Kadoorie hatch return", true, true, "screwdriver", "the Kadoorie hatch is still bolted. you need a screwdriver"));
    rooms_["tunnel"].addRoute(Route("archive", "records room grate", true, true, "screwdriver", "the records-room grate is still jammed. you need a screwdriver"));

    rooms_["bridge"].addRoute(Route("quad", "courtyard return", false, false, "", ""));
    rooms_["bridge"].addRoute(Route("rooftop", "maintenance shaft ascent", true, true, "screwdriver", "the footbridge panel is still bolted shut. you need a screwdriver"));

    rooms_["clocktower"].addRoute(Route("library", "Main Library stair return", true, true, "library_clue", "you still have not read enough of the Main Library clue to find that stair"));
    rooms_["clocktower"].addRoute(Route("rooftop", "clock tower gate", true, true, "keycard", "the clock tower gate is locked. a keycard is required"));

    rooms_["rooftop"].addRoute(Route("bridge", "footbridge stairs down", false, false, "", ""));
    rooms_["rooftop"].addRoute(Route("archive", "records stairwell down", false, false, "", ""));
    rooms_["rooftop"].addRoute(Route("clocktower", "clock tower descent", false, false, "", ""));

    rooms_["dorm"].threatProfile = "safe";
    rooms_["dorm"].patrolIntensity = 0;
    rooms_["dorm"].addAmbientDetail("A late-night hall noise drifts in from Swire Hall and then goes quiet again.");
    rooms_["dorm"].addAmbientDetail("The corridor light under the door makes it feel like you never really slept.");
    rooms_["dorm"].addAmbientDetail("Your phone still shows an old HKU notice you remember reading in the last loop.");
    rooms_["dorm"].addAmbientDetail("The room smells faintly of paper, rain, and cold instant coffee.");
    rooms_["dorm"].addAmbientDetail("Your desk is still set up exactly the way you left it in the last failed run.");
    rooms_["dorm"].addExplorationDetail("A sketch on the desk links the Main Library, the Main Building, and the roof.");
    rooms_["dorm"].addExplorationDetail("Old notes suggest the footbridge route is fast but exposed.");
    rooms_["dorm"].addExplorationDetail("You kept one page just for security timings near University Street.");
    rooms_["dorm"].addExplorationDetail("Your bag is empty enough to carry a few key items without slowing you down.");
    rooms_["dorm"].addProgressionHint("Start with public rooms to gather low-risk items.");
    rooms_["dorm"].addProgressionHint("If runs fail, reset your route through corridor -> library first.");
    rooms_["dorm"].addProgressionHint("The heist is easiest when clue, route, and escape tool are staged early.");
    rooms_["dorm"].addProgressionHint("Track one primary path and one fallback path before leaving.");

    rooms_["corridor"].threatProfile = "watched";
    rooms_["corridor"].patrolIntensity = 1;
    rooms_["corridor"].addAmbientDetail("Cameras watch the flow along University Street in a slow fixed rhythm.");
    rooms_["corridor"].addAmbientDetail("Students crossing between Main Campus and Centennial Campus make good moving cover.");
    rooms_["corridor"].addAmbientDetail("A cleaning cart sits under a sign for the Main Library and Grand Hall.");
    rooms_["corridor"].addAmbientDetail("You can hear footsteps from several HKU buildings at once from here.");
    rooms_["corridor"].addAmbientDetail("The slope and long sightlines make hesitation feel more dangerous than movement.");
    rooms_["corridor"].addExplorationDetail("The signs here tell you how close the Main Library and Chi Wah really are.");
    rooms_["corridor"].addExplorationDetail("A campus board shows which branches stay public later into the evening.");
    rooms_["corridor"].addExplorationDetail("One camera misses a thin slice of space near a vending alcove.");
    rooms_["corridor"].addExplorationDetail("Loose notices near the board are a good place to find a hall pass.");
    rooms_["corridor"].addProgressionHint("This is your safest transit spine to branch into each objective.");
    rooms_["corridor"].addProgressionHint("Rotate between library and commons to avoid repeated patrol contact.");
    rooms_["corridor"].addProgressionHint("Use corridor to recover pace before entering restricted wings.");

    rooms_["lecture"].threatProfile = "low";
    rooms_["lecture"].patrolIntensity = 1;
    rooms_["lecture"].addAmbientDetail("The Grand Hall foyer feels too large after the tight quiet of the hall room.");
    rooms_["lecture"].addAmbientDetail("Event signs and folded chairs make the space feel between uses.");
    rooms_["lecture"].addAmbientDetail("The side stair toward Kadoorie stays dimmer than the main entrance.");
    rooms_["lecture"].addAmbientDetail("Your footsteps bounce off the hall and come back a half-second later.");
    rooms_["lecture"].addAmbientDetail("A projector glow still leaks from deeper inside the lecture space.");
    rooms_["lecture"].addExplorationDetail("Backstage notices still show room names that help you read the west side of campus.");
    rooms_["lecture"].addExplorationDetail("The side aisle is better than the open middle if you need to move quietly.");
    rooms_["lecture"].addExplorationDetail("The hall gives you a calm buffer before pushing into Kadoorie.");
    rooms_["lecture"].addExplorationDetail("From here, the route to Kadoorie feels shorter than it looked on paper.");
    rooms_["lecture"].addProgressionHint("Use lecture as a low-risk transition into lab routes.");
    rooms_["lecture"].addProgressionHint("If suspicion is high, avoid lingering and move through fast.");
    rooms_["lecture"].addProgressionHint("Lab access from here supports both direct and hidden progression.");

    rooms_["library"].threatProfile = "research";
    rooms_["library"].patrolIntensity = 1;
    rooms_["library"].addAmbientDetail("The Main Library feels calmer than the rest of campus, but never truly empty.");
    rooms_["library"].addAmbientDetail("A scanner beeps somewhere between the old and new wings.");
    rooms_["library"].addAmbientDetail("Reserve shelves make narrow lines of cover if you keep moving.");
    rooms_["library"].addAmbientDetail("The quiet here is broken by keyboards, printers, and the clock tower beyond the wall.");
    rooms_["library"].addAmbientDetail("A self-check machine throws a dull blue light over the nearby shelves.");
    rooms_["library"].addExplorationDetail("A margin note near the reserve shelf starts the first real route clue.");
    rooms_["library"].addExplorationDetail("The clue mentions the Main Building and hints at the clock tower stair.");
    rooms_["library"].addExplorationDetail("Terminal logs link office records to older parts of the Main Building.");
    rooms_["library"].addExplorationDetail("The map near the shelf gives just enough to trust the next step.");
    rooms_["library"].addProgressionHint("Secure library clue before attempting clocktower or admin routes.");
    rooms_["library"].addProgressionHint("This room unlocks strategic routing knowledge for late game.");
    rooms_["library"].addProgressionHint("Re-searching library can still reveal timing-safe transitions.");

    rooms_["commons"].threatProfile = "public";
    rooms_["commons"].patrolIntensity = 1;
    rooms_["commons"].addAmbientDetail("Chi Wah still hums softly with printers, screens, and small study groups.");
    rooms_["commons"].addAmbientDetail("The spiral stair makes it easy to hear people before you see them.");
    rooms_["commons"].addAmbientDetail("Booth seats and open tables break sightlines better than the corridor outside.");
    rooms_["commons"].addAmbientDetail("A booking screen flips between study rooms and student spaces.");
    rooms_["commons"].addAmbientDetail("The courtyard entrance lets cooler air drift in from Centennial Campus.");
    rooms_["commons"].addExplorationDetail("A sketch on a whiteboard lines up with the Main Building office branch.");
    rooms_["commons"].addExplorationDetail("Loose paper near the booths is a believable place for a hall pass to surface.");
    rooms_["commons"].addExplorationDetail("Printer traffic here can hide a quick move better than silence can.");
    rooms_["commons"].addExplorationDetail("The Cafe 330 cutthrough is useful when University Street feels too watched.");
    rooms_["commons"].addProgressionHint("Use commons to set up admin entry with lower initial suspicion.");
    rooms_["commons"].addProgressionHint("Canteen detours can reduce repeated restricted exposure.");
    rooms_["commons"].addProgressionHint("This room is ideal for pacing before archive attempts.");

    rooms_["lab"].threatProfile = "tooling";
    rooms_["lab"].patrolIntensity = 2;
    rooms_["lab"].addAmbientDetail("The Kadoorie lab is quiet enough that every metal sound feels too loud.");
    rooms_["lab"].addAmbientDetail("A service hatch near the back wall breaks the clean line of the room.");
    rooms_["lab"].addAmbientDetail("Labels on cabinets and benches make the room feel more practical than academic tonight.");
    rooms_["lab"].addAmbientDetail("The smell of metal and cleaning fluid gets stronger near the hatch.");
    rooms_["lab"].addAmbientDetail("A timer somewhere deeper in the lab keeps repeating the same cycle.");
    rooms_["lab"].addExplorationDetail("The Kadoorie tool bench is the best place to find a screwdriver.");
    rooms_["lab"].addExplorationDetail("The hatch here leads into a hidden route below Centennial Campus.");
    rooms_["lab"].addExplorationDetail("Cabinet codes suggest the service line connects back toward the Main Building.");
    rooms_["lab"].addExplorationDetail("Research notes mention checks near the Main Building service door.");
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
    rooms_["admin"].addAmbientDetail("The Main Building office still holds the tired noise of printers and fans.");
    rooms_["admin"].addAmbientDetail("Paper trays, ledgers, and access forms sit in careful late-night order.");
    rooms_["admin"].addAmbientDetail("A camera keeps sweeping the door toward the records room.");
    rooms_["admin"].addAmbientDetail("The office is orderly enough that any delay feels suspicious.");
    rooms_["admin"].addAmbientDetail("A lanyard hook hangs empty, then full, depending on the loop.");
    rooms_["admin"].addExplorationDetail("The routing binder is the cleanest way to map the records-room path.");
    rooms_["admin"].addExplorationDetail("Cross-checking ledgers here can uncover part of the roof code.");
    rooms_["admin"].addExplorationDetail("The side door to security is useful, but only if you are ready for the risk.");
    rooms_["admin"].addExplorationDetail("The Main Building office is where the route stops being theory.");
    rooms_["admin"].addProgressionHint("Confirm admin route before direct archive attempts.");
    rooms_["admin"].addProgressionHint("Use keycard to reduce suspicion pressure while gathering fragments.");
    rooms_["admin"].addProgressionHint("Admin is where the heist turns from setup into execution.");

    rooms_["archive"].threatProfile = "critical";
    rooms_["archive"].patrolIntensity = 4;
    rooms_["archive"].addAmbientDetail("Old cabinets in the records room creak even when you barely touch them.");
    rooms_["archive"].addAmbientDetail("The roof keypad blinks in a steady pattern near the stair door.");
    rooms_["archive"].addAmbientDetail("Air from the service line rises through the floor near the back grate.");
    rooms_["archive"].addAmbientDetail("This room feels older than the rest of the run, and less forgiving.");
    rooms_["archive"].addAmbientDetail("The silence here is sharp enough to make every mistake feel louder.");
    rooms_["archive"].addExplorationDetail("The sealed file is hidden among old Main Building transfer records.");
    rooms_["archive"].addExplorationDetail("This room is also where the loose roof code starts to make sense.");
    rooms_["archive"].addExplorationDetail("The tunnel is ugly, but it does give you another way in and out.");
    rooms_["archive"].addExplorationDetail("Every extra second in the records room pushes the run closer to failure.");
    rooms_["archive"].addProgressionHint("Archive is the final objective room: enter prepared, exit quickly.");
    rooms_["archive"].addProgressionHint("Bring route knowledge and key items before committing.");
    rooms_["archive"].addProgressionHint("Once folder is secured, prioritize nearest viable escape route.");
    rooms_["archive"].addProgressionHint("If risk spikes, tunnel or bridge exits can salvage the run.");

    rooms_["quad"].threatProfile = "open";
    rooms_["quad"].patrolIntensity = 1;
    rooms_["quad"].addAmbientDetail("The Centennial Campus courtyard feels wide and bright even after most classes are done.");
    rooms_["quad"].addAmbientDetail("Light spills out from Chi Wah and the Grand Hall across the open podium.");
    rooms_["quad"].addAmbientDetail("A small sign points toward the lily pond and the upper level of University Street.");
    rooms_["quad"].addAmbientDetail("People crossing the courtyard give you cover if you keep your pace natural.");
    rooms_["quad"].addAmbientDetail("The open space makes it easy to see trouble coming from far away.");
    rooms_["quad"].addExplorationDetail("This is a good place to slow down and choose between the footbridge, Cafe 330, and the sports side.");
    rooms_["quad"].addExplorationDetail("The courtyard is safer than the Main Building, but too open for long delays.");
    rooms_["quad"].addExplorationDetail("A campus sign here helps you keep the Centennial side straight in your head.");
    rooms_["quad"].addExplorationDetail("If the Main Building feels too hot, this branch gives you room to reset the run.");
    rooms_["quad"].addProgressionHint("Use the courtyard to change branches without adding too much risk.");
    rooms_["quad"].addProgressionHint("This is where faster and safer routes start to split apart.");
    rooms_["quad"].addProgressionHint("The footbridge is the quick route. Cafe 330 and the sports side are calmer.");

    rooms_["canteen"].threatProfile = "low";
    rooms_["canteen"].patrolIntensity = 1;
    rooms_["canteen"].addAmbientDetail("Cafe 330 is almost empty now, with just a few trays left near the self-service counter.");
    rooms_["canteen"].addAmbientDetail("The late kitchen noise is soft enough to cover a short stop.");
    rooms_["canteen"].addAmbientDetail("A closing board and drink fridge glow near the edge of the room.");
    rooms_["canteen"].addAmbientDetail("Most people here are focused on coffee, food, or getting back out.");
    rooms_["canteen"].addAmbientDetail("It is one of the easiest rooms on campus to look normal in.");
    rooms_["canteen"].addExplorationDetail("Loose notices and receipts here are a believable place to notice a spare hall pass.");
    rooms_["canteen"].addExplorationDetail("This cutthrough is useful when University Street feels too exposed.");
    rooms_["canteen"].addExplorationDetail("Cafe 330 is better for breathing space than for major progress.");
    rooms_["canteen"].addExplorationDetail("A short stop here can steady the run before you go back into watched rooms.");
    rooms_["canteen"].addProgressionHint("Use Cafe 330 as a calm detour, not as your main plan.");
    rooms_["canteen"].addProgressionHint("If suspicion rises, this is one of the safer places to recover.");
    rooms_["canteen"].addProgressionHint("This room fits well between Chi Wah and the courtyard.");

    rooms_["gym"].threatProfile = "medium";
    rooms_["gym"].patrolIntensity = 2;
    rooms_["gym"].addAmbientDetail("The sports hall is big enough that sound moves strangely under the lights.");
    rooms_["gym"].addAmbientDetail("Locker rows and side doors break up the room more than you expect at first glance.");
    rooms_["gym"].addAmbientDetail("A notice near the entrance still lists the long opening hours of the sports centre.");
    rooms_["gym"].addAmbientDetail("The far end of the hall is darker, but the open floor still makes careless movement obvious.");
    rooms_["gym"].addAmbientDetail("This side of campus feels separate from the library route, which can be useful.");
    rooms_["gym"].addExplorationDetail("The sports side gives you another way to approach security when the Main Building route is rough.");
    rooms_["gym"].addExplorationDetail("You can learn the rhythm here, but the room is slower than the central route.");
    rooms_["gym"].addExplorationDetail("The gym is a backup branch, not the fastest branch.");
    rooms_["gym"].addExplorationDetail("If you need a less direct route into a watched area, this is often the better choice.");
    rooms_["gym"].addProgressionHint("Use the sports side when the centre of campus feels too busy.");
    rooms_["gym"].addProgressionHint("This branch is slower, but it can save a shaky run.");
    rooms_["gym"].addProgressionHint("You do not need this route every loop, but it matters when pressure rises.");

    rooms_["security"].threatProfile = "critical";
    rooms_["security"].patrolIntensity = 4;
    rooms_["security"].addAmbientDetail("The East Gate security office is full of monitor glow, radio noise, and too many lines of sight.");
    rooms_["security"].addAmbientDetail("The newer East Gate access feels clean and bright in a way that makes you more visible, not less.");
    rooms_["security"].addAmbientDetail("Reader lights flash near the desk while camera feeds cycle across the wall.");
    rooms_["security"].addAmbientDetail("This is the kind of room where even standing still can feel wrong.");
    rooms_["security"].addAmbientDetail("If the run goes bad here, it goes bad fast.");
    rooms_["security"].addExplorationDetail("A careful search here can turn up the keycard that makes the Main Building safer.");
    rooms_["security"].addExplorationDetail("This room gives strong control over later routes, but it is never low-risk.");
    rooms_["security"].addExplorationDetail("Come in hidden or already prepared, then leave quickly.");
    rooms_["security"].addExplorationDetail("The East Gate side rewards confidence more than hesitation.");
    rooms_["security"].addProgressionHint("Treat security as a deliberate choice, not a casual stop.");
    rooms_["security"].addProgressionHint("If you enter, know what you want before you search.");
    rooms_["security"].addProgressionHint("This room can make later runs easier, but only if you survive it.");

    rooms_["tunnel"].threatProfile = "hidden";
    rooms_["tunnel"].patrolIntensity = 3;
    rooms_["tunnel"].addAmbientDetail("The service tunnel under the Centennial side is wet, close, and louder than it should be.");
    rooms_["tunnel"].addAmbientDetail("Pipes and old wiring run beside the narrow path toward the Main Building side.");
    rooms_["tunnel"].addAmbientDetail("Every drip sounds bigger here because there is nowhere for the noise to go.");
    rooms_["tunnel"].addAmbientDetail("It is ugly, but it gives you something the public route does not: privacy.");
    rooms_["tunnel"].addAmbientDetail("The tunnel feels like a place students were never meant to stay in for long.");
    rooms_["tunnel"].addExplorationDetail("The Kadoorie hatch and the records-room grate turn this into a real alternate route.");
    rooms_["tunnel"].addExplorationDetail("If you know where to come up, the tunnel can cut past the most annoying parts of the Main Building.");
    rooms_["tunnel"].addExplorationDetail("This route is for commitment, not drifting.");
    rooms_["tunnel"].addExplorationDetail("The hidden path matters most when the normal route is already slipping away.");
    rooms_["tunnel"].addProgressionHint("Use the tunnel after you have a screwdriver and a reason to be here.");
    rooms_["tunnel"].addProgressionHint("This branch is strong because it is direct, not because it is safe.");
    rooms_["tunnel"].addProgressionHint("A tunnel run works best when you already know your exit.");

    rooms_["bridge"].threatProfile = "exposed";
    rooms_["bridge"].patrolIntensity = 2;
    rooms_["bridge"].addAmbientDetail("The University Street footbridge is open to wind, light, and anyone looking up from below.");
    rooms_["bridge"].addAmbientDetail("The metal bench and warning strip look ordinary until you know what is hidden there.");
    rooms_["bridge"].addAmbientDetail("The bridge gives a strong view back toward Centennial Campus and across the slope.");
    rooms_["bridge"].addAmbientDetail("It is easy to cross quickly and much harder to linger without drawing attention.");
    rooms_["bridge"].addAmbientDetail("This is one of the cleanest places to feel how exposed the late game really is.");
    rooms_["bridge"].addExplorationDetail("With the screwdriver, the panel below the bench opens a faster rooftop route.");
    rooms_["bridge"].addExplorationDetail("This branch is quick enough to save a good run and exposed enough to ruin a messy one.");
    rooms_["bridge"].addExplorationDetail("Do not come here early unless you already know why.");
    rooms_["bridge"].addExplorationDetail("When the bridge works, it works because you commit and move.");
    rooms_["bridge"].addProgressionHint("The footbridge is the fast escape path once your tool route is ready.");
    rooms_["bridge"].addProgressionHint("Use this branch late, not as a general-purpose detour.");
    rooms_["bridge"].addProgressionHint("Speed is the reward here. Exposure is the cost.");

    rooms_["clocktower"].threatProfile = "hidden-critical";
    rooms_["clocktower"].patrolIntensity = 3;
    rooms_["clocktower"].addAmbientDetail("The Main Building clock tower stair is narrow, dusty, and much older than the rest of the run.");
    rooms_["clocktower"].addAmbientDetail("The sound of the clock carries through the stone and makes the whole stair feel alive.");
    rooms_["clocktower"].addAmbientDetail("A locked gate near the top turns the tower from a secret into a real route problem.");
    rooms_["clocktower"].addAmbientDetail("This branch feels hidden, but never relaxed.");
    rooms_["clocktower"].addAmbientDetail("Every step here reminds you that the Main Building is older than your plan.");
    rooms_["clocktower"].addExplorationDetail("The Main Library clue is what teaches you this stair exists at all.");
    rooms_["clocktower"].addExplorationDetail("With a keycard, the tower becomes a slower but controlled roof route.");
    rooms_["clocktower"].addExplorationDetail("The clock tower is a backup path, not the easiest first win.");
    rooms_["clocktower"].addExplorationDetail("This is the route for runs where observation matters more than speed.");
    rooms_["clocktower"].addProgressionHint("Only come here after the library clue and a keycard are ready.");
    rooms_["clocktower"].addProgressionHint("The tower rewards patience more than improvisation.");
    rooms_["clocktower"].addProgressionHint("This is the clean backup route when the bridge feels too exposed.");

    rooms_["rooftop"].threatProfile = "escape";
    rooms_["rooftop"].patrolIntensity = 1;
    rooms_["rooftop"].addAmbientDetail("The Main Building rooftop opens over Pokfulam and the rest of the campus below.");
    rooms_["rooftop"].addAmbientDetail("Wind pulls at the door behind you while the lights of HKU spread out under the edge.");
    rooms_["rooftop"].addAmbientDetail("From up here, the whole run finally starts to make sense as one connected place.");
    rooms_["rooftop"].addAmbientDetail("Archive stairs, the footbridge shaft, and the clock tower all end in the same cold air.");
    rooms_["rooftop"].addAmbientDetail("This is the first room that feels outside the loop.");
    rooms_["rooftop"].addExplorationDetail("Reaching the rooftop with the sealed file is the one thing that actually ends the night.");
    rooms_["rooftop"].addExplorationDetail("The roof is the payoff for every clue, route, and item you prepared below.");
    rooms_["rooftop"].addExplorationDetail("It does not matter which branch got you here. It matters that you got here with the file.");
    rooms_["rooftop"].addExplorationDetail("The run ends above campus, not inside it.");
    rooms_["rooftop"].addProgressionHint("Every route in the game is really an argument about how you want to reach this roof.");
    rooms_["rooftop"].addProgressionHint("Do the dangerous work below. Keep the roof for the finish.");
    rooms_["rooftop"].addProgressionHint("A clean win means arriving here with the file before the loop closes.");
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
namespace {
string quickExitLabel(const Room* target, bool locked) {
    string label = target ? target->name + " [" + target->id + "]" : "unknown";
    if (locked) label += " (locked)";
    return label;
}
string detailedExitLabel(const Room* target, const Route& route, bool unlocked) {
    string label = "- ";
    label += target ? target->name + " [" + target->id + "]" : route.to;
    vector<string> flags;
    if (route.hidden) flags.push_back("hidden");
    if (route.risky) flags.push_back("risky");
    if (!unlocked) flags.push_back("locked");
    if (!flags.empty()) label += " : " + Utils::join(flags, ", ");
    return label;
}
}
string CampusMap::exitsOf(const string& roomId) const {
    const Room* room = get(roomId);
    if (!room) return "";
    vector<string> labels;
    vector<string> targets = room->routeTargets(false);
    for (int i = 0; i < (int) targets.size(); i++) {
        const Room* target = get(targets[i]);
        labels.push_back(quickExitLabel(target, false));
    }
    return Utils::join(labels, ", ");
}
string CampusMap::exitsOf(const string& roomId, const MovementContext& context) const {
    const Room* room = get(roomId);
    if (!room) return "";
    vector<string> labels;
    for (int i = 0; i < (int)room->routes.size(); i++) {
        const Route& route = room->routes[i];
        bool unlocked = requirementMet(route.requirement, context);
        const Room* target = get(route.to);
        labels.push_back(quickExitLabel(target, !unlocked));
    }
    return Utils::join(labels, ", ");
}
string CampusMap::exitListOf(const string& roomId) const {
    const Room* room = get(roomId);
    if (!room) return "";
    vector<string> labels;
    vector<string> targets = room->routeTargets(false);
    for (int i = 0; i < (int) targets.size(); i++) {
        const Room* target = get(targets[i]);
        labels.push_back("- " + quickExitLabel(target, false));
    }
    return Utils::join(labels, "\n");
}
string CampusMap::exitListOf(const string& roomId, const MovementContext& context) const {
    const Room* room = get(roomId);
    if (!room) return "";
    vector<string> labels;
    for (int i = 0; i < (int)room->routes.size(); i++) {
        const Route& route = room->routes[i];
        bool unlocked = requirementMet(route.requirement, context);
        const Room* target = get(route.to);
        labels.push_back(detailedExitLabel(target, route, unlocked));
    }
    return Utils::join(labels, "\n");
}
vector<string> CampusMap::availableEscapeRoutes(const MovementContext& context) const {
    vector<string> routes;
    if (requirementMet("rooftop_key", context)) {
        routes.push_back("Main Building roof stair");
    }
    if (requirementMet("screwdriver", context)) {
        routes.push_back("University Street footbridge shaft");
    }
    if (requirementMet("library_clue", context) && requirementMet("keycard", context)) {
        routes.push_back("Main Building clock tower stair");
    }
    return routes;
}
string CampusMap::progressionSummary(const MovementContext& context, bool hasFolder) const {
    if (!context.libraryClue && !context.hasScrewdriver) return "stage 1: learn the safe public side of campus";
    if (!context.adminRoute && !context.hasScrewdriver) return "stage 2: turn the Main Library clue into a records-room route";
    if (!context.adminRoute && context.hasScrewdriver && !hasFolder) return "stage 2: the Kadoorie tool route can bypass the normal records-room entry";
    if (!hasFolder) return "stage 3: enter the records room and take the sealed file";
    vector<string> routes = availableEscapeRoutes(context);
    if (routes.empty()) return "stage 4: you have the file, but you still need a roof route";
    if ((int)routes.size() == 1) return "stage 4: you have the file and one roof route";
    if ((int)routes.size() == 2) return "stage 4: you have the file and two roof routes";
    return "stage 4: you have the file and every roof route is open";
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
vector<string> CampusMap::routeStrategy(const string& roomId, const MovementContext& context) const {
    vector<string> notes;
    if (roomId == "library") {
        notes.push_back(context.libraryClue ? "clue confirmed; clocktower route is now theoretically reachable" : "priority: search here until clue is secured");
        notes.push_back(context.adminRoute ? "admin route already unlocked; library now serves as fallback transit" : "without clue, admin records are difficult to interpret");
        notes.push_back(context.hasKeycard ? "keycard in hand; faculty/security transitions are safer" : "no keycard yet; avoid overcommitting to restricted loops");
    } else if (roomId == "lab") {
        notes.push_back(context.hasScrewdriver ? "tool acquired; hidden tunnel and bridge shaft become viable" : "screwdriver missing; hidden routes remain locked");
        notes.push_back(context.adminRoute ? "direct objective route available, but tunnel still offers pressure relief" : "use lab as setup while admin route is unresolved");
        notes.push_back("lab is best used as a short stop, not a long loiter point");
    } else if (roomId == "admin") {
        notes.push_back(context.adminRoute ? "archive door protocol decoded; direct approach now permitted" : "search records to unlock archive path");
        notes.push_back(context.hasKeycard ? "restricted penalties reduced by keycard support" : "no keycard; suspicion can spike quickly here");
        notes.push_back(context.hasScrewdriver ? "tunnel fallback available if front route gets hot" : "tunnel fallback unavailable without screwdriver");
    } else if (roomId == "archive") {
        notes.push_back(context.hasRooftopKey ? "archive stair to rooftop is unlocked" : "need code fragments to convert objective into clean rooftop route");
        notes.push_back(context.hasScrewdriver ? "bridge/tunnel alternates can support emergency extraction" : "no tool-based alternate routes from archive pressure");
        notes.push_back(context.hasKeycard ? "clocktower backup can complement archive routeing" : "clocktower backup still locked by credential requirement");
    } else if (roomId == "bridge") {
        notes.push_back(context.hasScrewdriver ? "maintenance shaft route online for fast extraction" : "maintenance shaft locked; bridge is currently transit only");
        notes.push_back(context.hasRooftopKey ? "archive and bridge both offer rooftop convergence options" : "consider bridge as primary only if archive key is not ready");
        notes.push_back("bridge is speed-oriented and should be used decisively");
    } else if (roomId == "clocktower") {
        notes.push_back(context.libraryClue ? "service stair located from library intel" : "clocktower entry remains hidden until clue discovery");
        notes.push_back(context.hasKeycard ? "gate can be opened; route is viable" : "magnetic gate remains sealed without keycard");
        notes.push_back("clocktower route trades speed for controlled exposure");
    } else if (roomId == "security") {
        notes.push_back((context.hasKeycard || context.hidden) ? "entry condition met; use this access quickly" : "entry denied unless hidden or carrying keycard");
        notes.push_back(context.hasKeycard ? "keycard grants cleaner navigation through hub connectors" : "without keycard, security stay should be minimal");
        notes.push_back("security is optional but can rebalance difficult mid-run routes");
    } else if (roomId == "tunnel") {
        notes.push_back(context.hasScrewdriver ? "all tunnel hatches can be traversed" : "hatches remain locked without screwdriver");
        notes.push_back(context.adminRoute ? "tunnel now functions as alternate archive ingress/egress" : "tunnel is useful prep, but admin objective still unresolved");
        notes.push_back("tunnel rewards planning and punishes indecision");
    } else {
        notes.push_back("maintain low suspicion while chaining objective dependencies");
        notes.push_back("prefer routes with clear unlock state before committing");
        notes.push_back("keep at least one alternate extraction branch in reserve");
    }
    return notes;
}
vector<string> CampusMap::sectorBriefing(const MovementContext& context, bool hasFolder) const {
    vector<string> lines;
    lines.push_back("public sector: corridor/library/commons/quad/canteen remain best for low-risk setup");
    lines.push_back(string("restricted sector: ") + (context.hasKeycard ? "keycard-assisted traversal available" : "high suspicion exposure expected"));
    lines.push_back(string("hidden sector: ") + (context.hasScrewdriver ? "tunnel and bridge shaft unlocked" : "tool-lock active on hidden routes"));
    lines.push_back(string("tower sector: ") + (context.libraryClue ? "clocktower location known" : "clocktower location still obscured"));
    lines.push_back(string("archive sector: ") + (context.adminRoute ? "route protocol unlocked" : "admin route gate still active"));
    vector<string> escape = availableEscapeRoutes(context);
    if (escape.empty()) lines.push_back("extraction sector: no rooftop route currently unlocked");
    else lines.push_back("extraction sector: " + Utils::join(escape, ", "));
    lines.push_back(string("objective state: ") + (hasFolder ? "folder secured, prioritize extraction" : "folder not secured yet"));
    return lines;
}
vector<string> CampusMap::heistChecklist(const MovementContext& context, bool hasFolder) const {
    vector<string> list;
    list.push_back(string(context.libraryClue ? "[x] " : "[ ] ") + "obtain library clue and decode hidden references");
    list.push_back(string(context.adminRoute ? "[x] " : "[ ] ") + "unlock archive route protocol from admin records");
    list.push_back(string(context.hasScrewdriver ? "[x] " : "[ ] ") + "secure screwdriver to enable hidden mechanical routes");
    list.push_back(string(context.hasKeycard ? "[x] " : "[ ] ") + "acquire keycard for restricted-zone control");
    list.push_back(string(context.hasRooftopKey ? "[x] " : "[ ] ") + "assemble code access and retrieve rooftop key");
    list.push_back(string(hasFolder ? "[x] " : "[ ] ") + "retrieve sealed folder from archive objective");

    vector<string> routes = availableEscapeRoutes(context);
    if (routes.empty()) {
        list.push_back("[ ] unlock at least one rooftop extraction route");
    } else if ((int)routes.size() == 1) {
        list.push_back("[x] unlock at least one rooftop extraction route");
        list.push_back("[ ] optional: unlock a second extraction fallback route");
    } else {
        list.push_back("[x] unlock at least one rooftop extraction route");
        list.push_back("[x] optional: unlock a second extraction fallback route");
    }

    bool preparedForFinal = context.libraryClue && context.adminRoute && hasFolder && !routes.empty();
    list.push_back(string(preparedForFinal ? "[x] " : "[ ] ") + "finalize heist chain: clue -> route -> folder -> extraction");

    if (!context.libraryClue) {
        list.push_back("next focus: stay in public wing and search library until clue appears");
    } else if (!context.adminRoute) {
        list.push_back("next focus: enter admin and secure archive route confirmation");
    } else if (!hasFolder) {
        list.push_back("next focus: approach archive with low suspicion and collect objective");
    } else if (routes.empty()) {
        list.push_back("next focus: unlock any rooftop path before risk resets the loop");
    } else {
        list.push_back("next focus: execute extraction via your safest unlocked rooftop route");
    }
    return list;
}
string CampusMap::layout() const {
    return "HKU route map\n"
           "Start side\n"
           "  Swire Hall Room [dorm]\n"
           "    -> University Street [corridor]\n"
           "    -> Main Library [library]\n"
           "    -> Grand Hall [lecture]\n"
           "    -> Chi Wah Learning Commons [commons]\n"
           "    -> Centennial Campus Courtyard [quad]\n"
           "\n"
           "Main route\n"
           "  Chi Wah -> Main Building Office [admin] -> Main Building Records Room [archive] -> Main Building Rooftop [rooftop]\n"
           "\n"
           "Alternate branches\n"
           "  Grand Hall -> Kadoorie Biological Sciences Building [lab] -> Centennial Service Tunnel [tunnel]\n"
           "  Courtyard -> University Street Footbridge [bridge]\n"
           "  Main Library -> Main Building Clock Tower [clocktower]\n"
           "  Courtyard -> Cafe 330 [canteen] or Sports Centre Hall [gym]\n"
           "\n"
           "Tip: use move with either the short id in brackets or the full HKU place name.";
}
