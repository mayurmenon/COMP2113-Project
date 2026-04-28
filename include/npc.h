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

// Returns the room where the NPC should be now. Input: NPC data and current minute. Output: room id.
string npc_room_at_time(const NPC& npc, int minute);

// Updates one NPC to the correct scheduled room. Input: NPC data and current minute. Output: none.
void npc_update_room(NPC& npc, int minute);

// Checks whether the NPC can currently see the player. Input: NPC data, player room, and hidden flag. Output: true or false.
bool npc_detects_player(const NPC& npc, const string& player_room, bool player_hidden);

// Advances NPC detection state for one tick. Input: NPC data, player room, and hidden flag. Output: suspicion change.
int npc_detection_tick(NPC& npc, const string& player_room, bool player_hidden);

// Resets one NPC back to calm state. Input: NPC data. Output: none.
void npc_clear_alert(NPC& npc);

// Returns the dialogue lines currently available. Input: NPC data and loop count. Output: dialogue indices.
vector<int> npc_get_available_dialogues(const NPC& npc, int loop_counter);

// Runs one dialogue choice. Input: NPC data, dialogue index, and clue output text. Output: suspicion change.
int npc_interact(NPC& npc, int dialogue_index, string& clue_out);

// Creates the librarian NPC. Input: difficulty index. Output: NPC data.
NPC create_librarian(int difficulty);
// Creates the janitor NPC. Input: difficulty index. Output: NPC data.
NPC create_janitor(int difficulty);
// Creates the prefect NPC. Input: difficulty index. Output: NPC data.
NPC create_prefect(int difficulty);
// Creates the teaching assistant NPC. Input: difficulty index. Output: NPC data.
NPC create_TA(int difficulty);
// Creates the admin NPC. Input: difficulty index. Output: NPC data.
NPC create_admin(int difficulty);
// Creates the guard NPC. Input: difficulty index. Output: NPC data.
NPC create_guard(int difficulty);

// Resets one NPC for a fresh loop. Input: NPC data. Output: none.
void npc_reset_for_loop(NPC& npc);

#endif // NPC_H
