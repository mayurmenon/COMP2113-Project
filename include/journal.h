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

// Adds one entry if it is not already present. Input: journal list and new entry. Output: none.
void journal_add_entry(vector<journal_entry>& journal, const journal_entry& entry);

// Checks whether one journal id already exists. Input: journal list and id. Output: true or false.
bool journal_has_entry(const vector<journal_entry>& journal, const string& id);

// Prints the current journal to the terminal. Input: journal list. Output: none.
void journal_print(const vector<journal_entry>& journal);

// Saves the journal to a text file. Input: journal list and file name. Output: none.
void journal_save(const vector<journal_entry>& journal, const string& filename);

// Loads journal entries from a text file. Input: journal list and file name. Output: none.
void journal_load(vector<journal_entry>& journal, const string& filename);

#endif // JOURNAL_H
