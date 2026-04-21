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

// Creates and returns the list of all events for the difficulty level.
vector<game_event> create_event_list(int difficulty);

// Checks if any events should be triggered at the current time.
void event_check_trigger(vector<game_event>& events, int current_minute);

// Decrements the duration of active events.
void events_tick(vector<game_event>& events);

// Returns a list of currently active events.
vector<game_event*> get_active_events_for_room(const vector<game_event>& events);

// Resets all events for the next gameplay loop.
void events_reset_for_loop(vector<game_event>& events);

#endif // EVENT_H