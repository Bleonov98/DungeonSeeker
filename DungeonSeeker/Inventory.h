#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include <unordered_map>

struct ItemData
{
    int cnt = 0;
    std::string textureName = "";
};

class Inventory
{
public:

    void AddItem(std::shared_ptr<Item> item) {
        items[item->GetID()].cnt++;
        items[item->GetID()].textureName = item->GetTextureName().substr(0, item->GetTextureName().size() - 1);
        items[item->GetID()].textureName.push_back('0');
    }

    void RemoveItem(ItemID type) {
        if (items[type].cnt > 0) {
            items[type].cnt--;
        }
    }

    int GetItemCount(ItemID type) const {
        auto it = items.find(type);
        if (it != items.end()) {
            return it->second.cnt;
        }
        return 0;
    }

    std::unordered_map<ItemID, ItemData> GetItems() { return items; }

private:

    std::unordered_map<ItemID, ItemData> items; // type and item counter

};

#endif // !INVENTORY_H