#ifndef JOURNAL_H
#define JOURNAL_H

#include <string>
#include <vector>

using namespace std;

struct journal_entry {
    string id;
    string text;
    bool permanent_unlock;
};

// Adds a new entry to the journal.
void journal_add_entry(vector<journal_entry>& journal, const journal_entry& entry);

// Checks if an entry with the given id already exists.
bool journal_has_entry(const vector<journal_entry>& journal, const string& id);

// Displays all journal entries to the player.
void journal_print(const vector<journal_entry>& journal);

// Saves all journal entries to a file.
void journal_save(const vector<journal_entry>& journal, const string& filename);

// Loads journal entries from a file.
void journal_load(vector<journal_entry>& journal, const string& filename);

#endif // JOURNAL_H