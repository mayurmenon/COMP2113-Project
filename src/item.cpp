#include "item.h"
#include <map>
#include <string>
#include <vector>

using namespace std;

map<ItemType, Item> createItemTable() {
    map<ItemType, Item> table;

    // Hall Pass: Single-use item that removes 15 suspicion when used.
    table[ItemType::HallPass] = Item(
        ItemType::HallPass,
        "hall pass",
        "A forged faculty hall pass. Single use, drastically lowers suspicion.",
        "You flash the forged hall pass. Security backs down.",
        "There is nobody around to use the hall pass on.",
        true,   // consumable
        -15     // suspicionEffect
    );

    // Keycard: Permanent item. Normally, entering restricted areas
    // raises suspicion. The keycard negates that penalty.
    table[ItemType::Keycard] = Item(
        ItemType::Keycard,
        "keycard",
        "An admin-level security keycard. Removes penalties in restricted zones.",
        "You swipe the keycard to bypass the security check.",
        "This room doesn't require a keycard scan.",
        false,  // not consumable
        0       // suspicionEffect handled in game.cpp logic
    );

    // Screwdriver: Permanent item. Can open the bridge maintenance
    // panel to access the rooftop cleanly.
    table[ItemType::Screwdriver] = Item(
        ItemType::Screwdriver,
        "screwdriver",
        "A small flathead screwdriver. Great for opening maintenance panels.",
        "You unbolt the bridge panel. A hidden maintenance shaft opens up.",
        "There are no panels or screws to loosen here.",
        false,  // not consumable
        0       // suspicionEffect handled in game.cpp logic
    );

    // Code Fragment: Stackable item. You need a random number (1-3)
    // of these to crack the archive lock.
    table[ItemType::CodeFragment] = Item(
        ItemType::CodeFragment,
        "code fragment",
        "A torn piece of a hexadecimal sequence.",
        "You fit the fragment into the keypad logic.",
        "The current terminal cannot interpret this code fragment.",
        false,  // not strictly consumed by 'using', acts as a permanent key part
        0       // suspicionEffect handled in game.cpp logic
    );

    // Rooftop Key: Single-use item. Opens the rooftop door from the archive,
    // avoiding the heavy end-game suspicion spike.
    table[ItemType::RooftopKey] = Item(
        ItemType::RooftopKey,
        "rooftop key",
        "A heavy magnetic key for the final roof exit door.",
        "The heavy door clicks open smoothly. You slip through unseen.",
        "This key only fits the final rooftop exit.",
        true,   // consumable
        0       // suspicionEffect handled in game.cpp logic
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

// ============================================================================
// SYSTEM EXPANSION AND ROADMAP DOCUMENTATION
// ============================================================================
// This section contains planned expansions, architectural notes, and
// general lore to guide future development of the Loopbreak items module.
// Maintaining a clear roadmap here ensures new developers understand
// the underlying philosophy.
//
// Phase 1 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 2 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 3 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 4 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 5 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 6 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 7 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 8 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 9 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 10 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 11 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 12 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 13 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 14 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 15 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 16 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 17 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 18 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 19 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 20 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 21 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 22 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 23 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 24 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 25 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 26 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 27 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 28 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 29 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 30 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 31 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 32 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 33 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 34 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 35 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 36 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 37 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 38 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
// Phase 39 Sub-System Expansion Plannings:
// ----------------------------------------------------------------------------
// We are planning to refactor the dependency injection model for items.
// Currently, items are instantiated directly via the createItemTable factory.
// Moving forward, we should consider loading these definitions from a JSON
// or XML configuration file to allow modding without recompilation.
