#include "event.h"
#include <cstdlib>
#include <vector>
#include <ctime>

using namespace std;

vector<game_event> create_event_list(int difficulty) {
    // Build up all the random events that can happen during gameplay
    vector<game_event> events;

    int low_prob = 15;
    int med_prob = 20;
    int high_prob = 30;

    if (difficulty == 1) {
        low_prob = 25;
        med_prob = 35;
        high_prob = 50;
    }
    else if (difficulty == 2) {
        low_prob = 40;
        med_prob = 55;
        high_prob = 70;
    }
    
    game_event fire_alarm;
    fire_alarm.id = "fire_alarm";
    fire_alarm.description = "The fire alarm goes off, causing chaos and confusion.";
    fire_alarm.effect_type = effect_npc_moved;
    fire_alarm.target_room = "main_corridor";
    fire_alarm.target_npc = "";
    fire_alarm.suspiciousness_change = -5;
    fire_alarm.duration = 10;
    fire_alarm.trigger_start = 120;
    fire_alarm.trigger_end = 360;
    fire_alarm.probability = low_prob;
    fire_alarm.status = event_inactive;
    fire_alarm.ticks_remaining = 0;
    events.push_back(fire_alarm);

    game_event inspection;
    inspection.id = "corridor_inspection";
    inspection.description = "A surprise inspection is announced, and the prefects are ordered to patrol the corridors.";
    inspection.effect_type = effect_none;
    inspection.target_room = "faculty_corridor";
    inspection.target_npc = "Student Prefect";
    inspection.suspiciousness_change = 0;
    inspection.duration = 8;
    inspection.trigger_start = 60;
    inspection.trigger_end = 480;
    inspection.probability = med_prob;
    inspection.status = event_inactive;
    inspection.ticks_remaining = 0;
    events.push_back(inspection);

    game_event power_dip;
    power_dip.id = "power_dip";
    power_dip.description = "The power briefly goes out, plunging the school into darkness.";
    power_dip.effect_type = effect_camera_offline;
    power_dip.target_room = "security_room";
    power_dip.target_npc = "";
    power_dip.suspiciousness_change = 0;
    power_dip.duration = 6;
    power_dip.trigger_start = 300;
    power_dip.trigger_end = 540;
    power_dip.probability = low_prob;
    power_dip.status = event_inactive;
    power_dip.ticks_remaining = 0;
    events.push_back(power_dip);

    game_event class_cancel;
    class_cancel.id = "class_cancellation";
    class_cancel.description = "A class is cancelled, the lecture hall is empty and quiet.";
    class_cancel.effect_type = effect_room_unlocked;
    class_cancel.target_room = "lecture_hall";
    class_cancel.target_npc = "";
    class_cancel.suspiciousness_change = 0;
    class_cancel.duration = 60;
    class_cancel.trigger_start = 60;
    class_cancel.trigger_end = 240;
    class_cancel.probability = low_prob;
    class_cancel.status = event_inactive;
    class_cancel.ticks_remaining = 0;
    events.push_back(class_cancel);

    game_event crowd;
    crowd.id = "student_crowd";
    crowd.description = "A large group of students floods chi wah, making it difficult to find a seat.";
    crowd.effect_type = effect_suspicious_down;
    crowd.target_room = "chi_wah";
    crowd.target_npc = "";
    crowd.suspiciousness_change = -3;
    crowd.duration = 15;
    crowd.trigger_start = 180;
    crowd.trigger_end = 420;
    crowd.probability = med_prob;
    crowd.status = event_inactive;
    crowd.ticks_remaining = 0;
    events.push_back(crowd);

    game_event staff_meeting;
    staff_meeting.id = "staff_meeting";
    staff_meeting.description = "A staff meeting is called. the admin officer and security guard are both in the main corridor for a while.The admin office is empty.";
    staff_meeting.effect_type = effect_room_unlocked;
    staff_meeting.target_room = "admin_office";
    staff_meeting.target_npc = "Admin Officer";
    staff_meeting.suspiciousness_change = 0;
    staff_meeting.duration = 20;
    staff_meeting.trigger_start = 240;
    staff_meeting.trigger_end = 420;
    staff_meeting.probability = high_prob;
    staff_meeting.status = event_inactive;
    staff_meeting.ticks_remaining = 0;
    events.push_back(staff_meeting);

    game_event rain;
    rain.id = "rain";
    rain.description = "It starts raining, the rooftop exit is closed.";
    rain.effect_type = effect_room_locked;
    rain.target_room = "rooftop";
    rain.target_npc = "";
    rain.suspiciousness_change = 0;
    rain.duration = 120;
    rain.trigger_start = 0;
    rain.trigger_end = 300;
    rain.probability = med_prob;
    rain.status = event_inactive;
    rain.ticks_remaining = 0;
    events.push_back(rain);

    game_event janitor_hint;
    janitor_hint.id = "janitor_hint";
    janitor_hint.description = "The janitor mutters something and drops a folded note as he rushes past you.";
    janitor_hint.effect_type = effect_none;
    janitor_hint.target_room = "science_lab";
    janitor_hint.target_npc = "Mr George (Janitor)";
    janitor_hint.suspiciousness_change = 0;
    janitor_hint.duration = 0;
    janitor_hint.trigger_start = 120;
    janitor_hint.trigger_end = 300;
    janitor_hint.probability = high_prob;
    janitor_hint.status = event_inactive;
    janitor_hint.ticks_remaining = 0;
    events.push_back(janitor_hint);

    return events;
}

void event_check_trigger(vector<game_event>& events, int current_minute) {
    // Check if any events should randomly trigger at this moment
    static bool seeded = false;
    if (!seeded) {
        srand(time(NULL));
        seeded = true;
    }
    for (int i = 0; i < (int) events.size(); i++) {
        if (events[i].status != event_inactive) {
            continue;
        }
        if (current_minute < events[i].trigger_start) {
            continue;
        }
        if (current_minute > events[i].trigger_end) {
            continue;
        }

        int roll = rand() % 100;
        if (roll < events[i].probability) {
            events[i].status = event_active;
            events[i].ticks_remaining = events[i].duration;
        }
    }
}

void events_tick(vector<game_event>& events) {
    // Count down the duration of any active events
    for (int i = 0; i < (int) events.size(); i++) {
        if (events[i].status != event_active) {
            continue;
        }
        events[i].ticks_remaining--;
        if (events[i].ticks_remaining <= 0) {
            events[i].status = event_done;
        }
    }
}

vector<game_event*> get_active_events_for_room(vector<game_event>& events) {
    // Get all events that are currently happening
    vector<game_event*> active;
    for (int i = 0; i < (int) events.size(); i++) {
        if (events[i].status == event_active) {
            active.push_back(&events[i]);
        }
    }
    return active;
}

void events_reset_for_loop(vector<game_event>& events) {
    // Reset all events back to inactive for the next loop
    for (int i = 0; i < (int) events.size(); i++) {
        events[i].status = event_inactive;
        events[i].ticks_remaining = 0;
    }
}