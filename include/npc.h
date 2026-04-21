#ifndef NPC_H
#define NPC_H

#include <string>
#include <vector>

using namespace std;

//NPC Roles
const int role_librarian = 0;
const int role_janitor = 1;
const int role_prefect = 2;
const int role_TA = 3;
const int role_admin = 4;
const int role_guard = 5;

//Alert levels (idle to curious to hostile to caught)
const int alert_idle = 0;
const int alert_curious = 1;
const int alert_hostile = 2;
const int alert_caught = 3;

//Dialogue types
const int dialogue_none = 0;
const int dialogue_greeting = 1;
const int dialogue_clue_given = 2;
const int dialogue_suspicious = 3;
const int dialogue_exhausted = 4;

//time in a day in minutes
struct schedule_entry {
    int start_min;
    int end_min;
    string room_ID;
};

//dialogue line an NPC can say
struct dialogue_line {
    string text;
    string condition;           
    bool gives_clue;
    int suspiciousness;
};

//data for an NPC
struct NPC {
    string name;
    int role;
    
    string current_room;
    vector<schedule_entry> schedule;

    int alert_level;
    int alert_timer;

    int dialogue_progress;
    vector<dialogue_line> dialogue_lines;
    bool has_clue;
    string clue_text;

    int detection_radius;

    int suspicion_per_tick;
};

// Returns the room ID where the NPC is at the given time.
string npc_room_at_time(const NPC& npc, int minute);

// Updates the NPC's current room based on schedule and time.
void npc_update_room(NPC& npc, int minute);

// Checks if the NPC can detect the player in the same room.
bool npc_detects_player(const NPC& npc, const string& player_room, bool player_hidden);

// Updates NPC alert level and returns suspicion change.
int npc_detection_tick(NPC& npc, const string& player_room, bool player_hidden);

// Resets the NPC's alert level back to idle.
void npc_clear_alert(NPC& npc);

// Returns a list of dialogue options available for this NPC.
vector<int> npc_get_available_dialogues(const NPC& npc, int loop_counter);

// Handles NPC dialogue interaction and returns suspicion change.
int npc_interact(NPC& npc, int dialogue_index, string& clue_out);

// Creates and returns a librarian NPC with difficulty-based stats.
NPC create_librarian(int difficulty);
// Creates and returns a janitor NPC with difficulty-based stats.
NPC create_janitor(int difficulty);
// Creates and returns a prefect NPC with difficulty-based stats.
NPC create_prefect(int difficulty);
// Creates and returns a TA NPC with difficulty-based stats.
NPC create_TA(int difficulty);
// Creates and returns an admin NPC with difficulty-based stats.
NPC create_admin(int difficulty);
// Creates and returns a guard NPC with difficulty-based stats.
NPC create_guard(int difficulty);

// Resets NPC state for the next gameplay loop.
void npc_reset_for_loop(NPC& npc);

#endif // NPC_H