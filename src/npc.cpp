#include "npc.h"
#include <cstdlib>

using namespace std;

string npc_room_at_time(const NPC& npc, int minute) {
    // Figure out where this NPC should be at a specific time based on their schedule
    string last_room = npc.current_room;
    for (int i=0; i< (int)npc.schedule.size(); i++) {
        if (minute >= npc.schedule[i].start_min && minute < npc.schedule[i].end_min) {
            return npc.schedule[i].room_ID;
        }
        if (npc.schedule[i].start_min <= minute) {
            last_room = npc.schedule[i].room_ID;
        }
    }
    return last_room;
}

void npc_update_room(NPC& npc, int minute) {
    // Move the NPC to wherever they should be at this time
    npc.current_room = npc_room_at_time(npc, minute);
}

bool npc_detects_player(const NPC& npc, const string& player_room, bool player_hidden) {
    // Check if this NPC can spot the player - they can't see hidden players or when caught
    if (player_hidden) {
        return false;
    }
    if(npc.alert_level == alert_caught) {
        return false;
    }
    return npc.current_room == player_room;
}

int npc_detection_tick(NPC& npc, const string& player_room, bool player_hidden) {
    // Handle the NPC's suspicion mechanics each turn
    if (!npc_detects_player(npc, player_room, player_hidden)) {
        npc.alert_timer--;
        if (npc.alert_timer <= 0) {
            npc.alert_level--;
            npc.alert_timer = 3;
        }
        return 0;
    }
    npc.alert_timer--;
    if (npc.alert_timer <0) {
        if (npc.alert_level < alert_caught) {
            npc.alert_level++;
        }
        if (npc.alert_level == alert_curious) {
            npc.alert_timer = 4;
        }
        else if (npc.alert_level == alert_hostile) {
            npc.alert_timer = 3;
        }
        else{
            npc.alert_timer = 2;
        }
    }

    if (npc.alert_level == alert_curious) {
        return npc.suspicion_per_tick;
    }
    else if (npc.alert_level == alert_hostile) {
        return npc.suspicion_per_tick * 2;
    }
    else if (npc.alert_level == alert_caught) {
        return npc.suspicion_per_tick * 4;
    }
    return 0;
}

void npc_clear_alert(NPC& npc) {
    // Reset the NPC back to normal alertness
    npc.alert_level = alert_idle;
    npc.alert_timer = 0;
}

static bool condition_met(const NPC& npc, const dialogue_line& line, int loop_counter) {
    // Check if the conditions are right for this dialogue option to be available
    if (line.condition == "always") {
        return true;
    }
    if (line.condition == "has_clue") {
        return npc.has_clue;
    }
    if (line.condition == "suspicious") {
        return npc.alert_level >= alert_curious;
    }
    if (line.condition == "loop2+") {
        return loop_counter >= 2;
    }
    return false;
}

vector<int> npc_get_available_dialogues(const NPC& npc, int loop_counter) {
    // Get all the dialogue options this NPC will offer right now
    vector<int> available;
    if (npc.dialogue_progress == dialogue_exhausted || npc.dialogue_progress == dialogue_suspicious) {
        return available;
    }
    for (int i=0; i< (int)npc.dialogue_lines.size(); i++) {
        if (condition_met(npc, npc.dialogue_lines[i], loop_counter)) {
            available.push_back(i);
        }
    }
    return available;
}

int npc_interact(NPC& npc, int dialogue_index, string& clue_out) {
    // Handle what happens when the player talks to this NPC
    if (dialogue_index < 0 || dialogue_index >= (int)npc.dialogue_lines.size()) {
        return 0;
    }
    dialogue_line& line = npc.dialogue_lines[dialogue_index];

    if (line.gives_clue && npc.has_clue) {
        clue_out = npc.clue_text;
        npc.has_clue = false;
        npc.dialogue_progress = dialogue_clue_given;
    }
    else if (npc.dialogue_progress == dialogue_none) {
        npc.dialogue_progress = dialogue_greeting;
    }

    if (npc.alert_level >= alert_curious) {
        npc.dialogue_progress = dialogue_suspicious;
    }

    if (!npc.has_clue && npc.dialogue_progress == dialogue_greeting) {
        npc.dialogue_progress = dialogue_exhausted;
    }
    return line.suspiciousness;
}

static void apply_difficulty(NPC& npc, int difficulty) {
    // Adjust NPC stats based on the game's difficulty level
    if (difficulty == 0) {
        npc.detection_radius = 1;
        npc.suspicion_per_tick = 1;
    }
    else if (difficulty == 1) {
        npc.detection_radius = 2;
        npc.suspicion_per_tick = 2;
    }
    else {
        npc.detection_radius = 3;
        npc.suspicion_per_tick = 4;
    }
}

static NPC blank_npc() {
    // Create a basic NPC template with default values
    NPC n;
    n.role = role_librarian;
    n.alert_level = alert_idle;
    n.alert_timer = 0;
    n.dialogue_progress = dialogue_none;
    n.has_clue = false;
    n.detection_radius = 1;
    n.suspicion_per_tick = 1;
    return n;
}

NPC create_librarian(int difficulty) {
    // Set up the librarian character with her schedule and dialogue
    NPC n = blank_npc();
    n.name = "Ms. Mayur (Librarian)";
    n.role = role_librarian;
    n.has_clue = true;
    n.clue_text = "The archive room code changes every week. Check the noticeboard near the admin office.";

    schedule_entry s1 = {0, 240, "library"};
    schedule_entry s2 = {240, 300, "chi_wah"};
    schedule_entry s3 = {300, 600, "library"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.current_room = "library";

    dialogue_line greet = {"Hello there! Can I help you find a book?", "always", false, 0};
    dialogue_line hint = { "I heard the archive code is posted somewhere in the school.", "clue_ready", true, -1};
    dialogue_line suspicious = {"You seem to be up to something. What are you doing here?", "suspicious", false, 3};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(hint);
    n.dialogue_lines.push_back(suspicious);

    apply_difficulty(n, difficulty);
    return n;
}

NPC create_janitor(int difficulty) {
    // Set up the janitor character with his schedule and dialogue
    NPC n = blank_npc();
    n.name = "Mr George (Janitor)";
    n.role = role_janitor;
    n.has_clue = true;
    n.clue_text = "The service corridor behind the science lab is never locked after 9pm.";
    
    schedule_entry s1 = {0, 120, "main_corridor"};
    schedule_entry s2 = {120, 300, "science_lab"};
    schedule_entry s3 = {300, 480, "admin_office"};
    schedule_entry s4 = {480, 600, "main_corridor"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.schedule.push_back(s4);
    n.current_room = "main_corridor";

    dialogue_line greet = {"Just doing my rounds, nothing to see here.", "always", false, 0};
    dialogue_line hint = {"Between you and me, I leave a door open...", "clue_ready", true, 0};
    dialogue_line loop2 = {"You again? What do you want?", "loop2+", false, 2};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(hint);
    n.dialogue_lines.push_back(loop2);

    apply_difficulty(n, difficulty);
    return n;
}

NPC create_prefect(int difficulty) {
    // Set up the prefect character with his schedule and dialogue
    NPC n = blank_npc();
    n.name = "Student Prefect";
    n.role = role_prefect;
    n.has_clue = false;
    n.clue_text = "";

    schedule_entry s1 = {0, 180, "main_corridor"};
    schedule_entry s2 = {180, 360, "faculty_corridor"};
    schedule_entry s3 = {360, 600, "main_corridor"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.current_room = "main_corridor";

    dialogue_line greet = {"Keep it moving.","always", false, 0};
    dialogue_line sus = {"Do you have a pass for this area?","suspicious", false, 4};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(sus);

    apply_difficulty(n, difficulty);
    return n;
}

NPC create_TA(int difficulty) {
    // Set up the TA character with her schedule and dialogue
    NPC n = blank_npc();
    n.name = "Ms.Aikagra (TA)";
    n.role = role_TA;
    n.has_clue = true;
    n.clue_text = "Prof. Wong's TA access card is kept in the top drawer of the lab bench.";

    schedule_entry s1 = {60, 240, "lecture_hall"};
    schedule_entry s2 = {240, 420, "science_lab"};
    schedule_entry s3 = {420, 600, "chi_wah"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.current_room = "lecture_hall";

    dialogue_line greet = {"Office hours are over, sorry.", "always",false,  0};
    dialogue_line hint = {"Actually... I probably shouldn't say this.", "clue_ready", true,  -2};
    dialogue_line loop2 = {"Wait, weren't you here yesterday too?", "loop2+",false, 2};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(hint);
    n.dialogue_lines.push_back(loop2);

    apply_difficulty(n, difficulty);
    return n;
}

NPC create_admin(int difficulty) {
    // Set up the admin officer character with her schedule and dialogue
    NPC n = blank_npc();
    n.name = "Admin Officer";
    n.role = role_admin;
    n.has_clue = true;
    n.clue_text = "The archive request form number is stamped on the folder in Room 301.";

    schedule_entry s1 = {60, 300, "admin_office"};
    schedule_entry s2 = {300, 360, "main_corridor"};
    schedule_entry s3 = {360, 540, "admin_office"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.current_room = "admin_office";

    dialogue_line greet = {"The office is closed to students.", "always", false, 2};
    dialogue_line hint = {"Fine, I'll tell you what I know.","clue_ready", true, 0};
    dialogue_line sus = {"I'm going to have to ask you to leave.", "suspicious", false,  5};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(hint);
    n.dialogue_lines.push_back(sus);

    apply_difficulty(n, difficulty);
    return n;
}

NPC create_guard(int difficulty) {
    // Set up the security guard character with his schedule and dialogue
    NPC n = blank_npc();
    n.name = "Security Guard";
    n.role = role_guard;
    n.has_clue = true;
    n.clue_text = "Camera 3 goes offline every night around 10pm for about five minutes.";

    schedule_entry s1 = {0, 200, "security_room"};
    schedule_entry s2 = {200, 400, "main_corridor"};
    schedule_entry s3 = {400, 600, "security_room"};
    n.schedule.push_back(s1);
    n.schedule.push_back(s2);
    n.schedule.push_back(s3);
    n.current_room = "security_room";

    dialogue_line greet = {"Campus is closing soon.","always",false,  0};
    dialogue_line hint = {"Between you and me, the cameras aren't perfect.", "clue_ready", true,   0};
    dialogue_line sus = {"Stop right there. Show me your ID.",  "suspicious", false,  6};
    n.dialogue_lines.push_back(greet);
    n.dialogue_lines.push_back(hint);
    n.dialogue_lines.push_back(sus);

    apply_difficulty(n, difficulty);
    return n;
}

void npc_reset(NPC& npc) {
    // Reset the NPC for a new loop iteration
    npc.alert_level = alert_idle;
    npc.alert_timer = 0;
    npc.dialogue_progress = dialogue_none;
    npc.has_clue = true;
}