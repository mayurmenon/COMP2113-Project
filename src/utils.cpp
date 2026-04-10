#include "utils.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
using namespace std;
namespace Utils {
string lowerCopy(const string& text) {
    string lowered = text;
    transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char letter) { return (char) tolower(letter); });
    return lowered;
}
string trim(const string& text) {
    size_t start = 0;
    while (start < text.size() && isspace((unsigned char) text[start])) start++;
    size_t end = text.size();
    while (end > start && isspace((unsigned char) text[end - 1])) end--;
    return text.substr(start, end - start);
}
string join(const vector<string>& parts, const string& separator) {
    ostringstream out;
    for (int index = 0; index < (int) parts.size(); index++) {
        if (index > 0) out << separator;
        out << parts[index];
    }
    return out.str();
}
string formatTime(int minutes) {
    ostringstream out;
    out << setfill('0') << setw(2) << minutes / 60 << ':' << setfill('0') << setw(2) << minutes % 60;
    return out.str();
}
}
