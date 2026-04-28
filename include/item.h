#ifndef ITEM_H
#define ITEM_H
#include <map>
#include <string>
#include <vector>
using namespace std;

// Lists every collectible item type used by the game.
enum class ItemType {
    HallPass,       // single-use pass that lowers suspicion
    Keycard,        // bypasses restricted-room suspicion penalty
    Screwdriver,    // unlocks the bridge maintenance shortcut
    CodeFragment,   // one piece of the coded-lock puzzle
    RooftopKey      // opens the rooftop stair door cleanly
};

// Stores the fixed data for one item definition.
struct Item {
    ItemType    type;             // which item this is
    string      name;             // display name shown to the player
    string      description;      // flavour text for the inventory screen
    string      useMessage;       // feedback printed when the item is used
    string      failMessage;      // feedback when the item cannot be used
    bool        consumable;       // true if the item disappears after one use
    int         suspicionEffect;  // suspicion change when used (negative = good)

    // Builds an empty item with safe defaults.
    // Input: none.
    // Output: none.
    Item()
        : type(ItemType::HallPass),
          consumable(false),
          suspicionEffect(0) {}

    // Builds a fully specified item.
    // Input: all item fields.
    // Output: none.
    Item(ItemType itemType,
         const string& itemName,
         const string& itemDescription,
         const string& itemUseMessage,
         const string& itemFailMessage,
         bool itemConsumable,
         int itemSuspicionEffect)
        : type(itemType),
          name(itemName),
          description(itemDescription),
          useMessage(itemUseMessage),
          failMessage(itemFailMessage),
          consumable(itemConsumable),
          suspicionEffect(itemSuspicionEffect) {}
};

// Creates and returns the master item-definition table.
// Input: none.
// Output: map from ItemType to its Item definition.
map<ItemType, Item> createItemTable();

// Converts a lowercase user-typed name to an ItemType.
// Input: lowercase item name string, output ItemType reference.
// Output: true if the name was recognized, false otherwise.
bool itemNameToType(const string& name, ItemType& outType);

// Returns a short tag string for an ItemType (used in save files).
// Input: the ItemType value.
// Output: a lowercase tag like "hallpass" or "keycard".
string itemTypeToTag(ItemType type);

// Converts a tag string back to an ItemType.
// Input: lowercase tag string, output ItemType reference.
// Output: true if the tag was recognized, false otherwise.
bool tagToItemType(const string& tag, ItemType& outType);

// Returns a vector of all defined ItemType values.
// Input: none.
// Output: vector with every ItemType enum value.
vector<ItemType> allItemTypes();

#endif
