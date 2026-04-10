#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <vector>
using namespace std;
namespace Utils {
// Makes a lowercase copy of text. Input: text. Output: lowered text.
string lowerCopy(const string& text);
// Trims spaces from both ends. Input: text. Output: trimmed text.
string trim(const string& text);
// Joins string parts with a separator. Input: string list and separator. Output: joined text.
string join(const vector<string>& parts, const string& separator);
// Converts minutes into HH:MM. Input: total minutes. Output: formatted time.
string formatTime(int minutes);
}
#endif
