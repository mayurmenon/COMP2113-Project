#include "item.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

map<ItemType, Item> createItemTable() {
    map<ItemType, Item> table;

    table[ItemType::HallPass] = Item(
        ItemType::HallPass,
        "hall pass",
        "A forged late-study pass with a staff stamp. Single use, lowers suspicion.",
        "You flash the late-study pass and buy yourself some breathing room.",
        "There is nobody here to calm down with the pass.",
        true,
        -15
    );

    table[ItemType::Keycard] = Item(
        ItemType::Keycard,
        "keycard",
        "A Main Building access card that softens suspicion in watched rooms.",
        "You swipe the card and move through before anyone asks questions.",
        "There is no reader here that needs the card.",
        false,
        0
    );

    table[ItemType::Screwdriver] = Item(
        ItemType::Screwdriver,
        "screwdriver",
        "A flathead screwdriver from a Kadoorie maintenance bench.",
        "You work the screws loose and open a hidden service access.",
        "There is nothing here that needs the screwdriver.",
        false,
        0
    );

    table[ItemType::CodeFragment] = Item(
        ItemType::CodeFragment,
        "code fragment",
        "Part of a roof access code copied from old HKU records.",
        "You line the fragment up with the rest of the code trail.",
        "You still need the full code sequence for this to matter.",
        false,
        0
    );

    table[ItemType::RooftopKey] = Item(
        ItemType::RooftopKey,
        "rooftop key",
        "A magnetic roof key paired with the Main Building stair door.",
        "The old roof door clicks open and the way out clears.",
        "This key only matters at the final roof exit.",
        true,
        0
    );

    return table;
}

bool itemNameToType(const string& name, ItemType& outType) {
    if (name == "hall pass" || name == "hallpass" || name == "pass") {
        outType = ItemType::HallPass;
        return true;
    }
    if (name == "keycard" || name == "card") {
        outType = ItemType::Keycard;
        return true;
    }
    if (name == "screwdriver" || name == "driver") {
        outType = ItemType::Screwdriver;
        return true;
    }
    if (name == "code fragment" || name == "fragment" || name == "code") {
        outType = ItemType::CodeFragment;
        return true;
    }
    if (name == "rooftop key" || name == "rooftopkey" || name == "key") {
        outType = ItemType::RooftopKey;
        return true;
    }
    return false;
}

string itemTypeToTag(ItemType type) {
    if (type == ItemType::HallPass) return "hallpass";
    if (type == ItemType::Keycard) return "keycard";
    if (type == ItemType::Screwdriver) return "screwdriver";
    if (type == ItemType::CodeFragment) return "fragment";
    if (type == ItemType::RooftopKey) return "rooftopkey";
    return "unknown";
}

bool tagToItemType(const string& tag, ItemType& outType) {
    if (tag == "hallpass") { outType = ItemType::HallPass; return true; }
    if (tag == "keycard") { outType = ItemType::Keycard; return true; }
    if (tag == "screwdriver") { outType = ItemType::Screwdriver; return true; }
    if (tag == "fragment") { outType = ItemType::CodeFragment; return true; }
    if (tag == "rooftopkey") { outType = ItemType::RooftopKey; return true; }
    return false;
}

vector<ItemType> allItemTypes() {
    vector<ItemType> types;
    types.push_back(ItemType::HallPass);
    types.push_back(ItemType::Keycard);
    types.push_back(ItemType::Screwdriver);
    types.push_back(ItemType::CodeFragment);
    types.push_back(ItemType::RooftopKey);
    return types;
}
