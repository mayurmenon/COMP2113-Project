#ifndef EVENT_H
#define EVENT_H

#include <string>
#include <vector>

using namespace std;

const int effect_none = 0;
const int effect_suspicious_down = 1;
const int effect_npc_moved = 2;
const int effect_room_locked = 3;
const int effect_room_unlocked = 4;
const int effect_camera_offline = 5;

const int event_inactive = 0;
const int event_active = 1;
const int event_done = 2;

struct game_event {
    string id;
    string description;
    int effect_type;
    string target_room;
    string target_npc;
    int suspiciousness_change;
    int duration;
    int trigger_start;
    int trigger_end;
    int probability;
    int status;
    int ticks_remaining;
};

// Builds the full event list for one difficulty. Input: difficulty index. Output: event vector.
vector<game_event> create_event_list(int difficulty);

// Tries to trigger inactive events at the current loop minute. Input: event list and minute since loop start. Output: none.
void event_check_trigger(vector<game_event>& events, int loop_minute);

// Counts down active events and closes expired ones. Input: event list and elapsed minutes. Output: none.
void events_tick(vector<game_event>& events, int minutes_elapsed);

// Returns pointers to the events that are active now. Input: event list. Output: active-event pointers.
vector<game_event*> get_active_events_for_room(vector<game_event>& events);

// Resets every event for a fresh loop. Input: event list. Output: none.
void events_reset_for_loop(vector<game_event>& events);

#endif // EVENT_H
