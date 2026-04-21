#include "journal.h"
#include <iostream>
#include <fstream>

using namespace std;

void journal_add_entry(vector<journal_entry>& journal, const journal_entry& entry) 
{
    // Only add the entry if we don't already have it
    if (journal_has_entry(journal, entry.id)) {
        return;
    }
    journal.push_back(entry);
}

bool journal_has_entry(const vector<journal_entry>& journal, const string& id) 
{
    // Check if we've already recorded this particular clue
    for (int i=0; i<(int)journal.size(); i++) {
        if (journal[i].id == id) {
            return true;
        }
    }
    return false;
}

void journal_print(const vector<journal_entry>& journal) 
{
    // Display all the clues we've collected so far
    if(journal.empty()) {
        cout << "Your journal is empty." << endl;
        return;
    }
    cout << "=== Journal ===" << endl;
    for (int i=0; i<(int)journal.size(); i++) {
        cout << "- " << journal[i].text << endl;
    }
    cout << "==================" << endl;
}

void journal_save(const vector<journal_entry>& journal, const string& filename) 
{
    // Write all our collected clues to a file for later
    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Error: Could not save journal to " << filename << endl;
        return;
    }
    for (int i=0; i<(int)journal.size(); i++) {
        file << journal[i].id << "\n";
        file << journal[i].text << "\n";
        if (journal[i].permanent_unlock) {
            file << "1" << "\n";
        }
        else {
            file << "0" << "\n";
        }
    }
    file.close();
}

void journal_load(vector<journal_entry>& journal, const string& filename) {
    // Read back our saved clues from a previous session
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Error: Could not load journal from " << filename << endl;
        return;
    }
    string id, text, unlock_flag;
    while (getline(file, id) && getline(file, text) && getline(file, unlock_flag)) {
        journal_entry entry;
        entry.id = id;
        entry.text = text;
        if (unlock_flag == "1") {
            entry.permanent_unlock = true;
        }
        else {
            entry.permanent_unlock = false;
        }
        journal_add_entry(journal, entry);
    }
    file.close();
}