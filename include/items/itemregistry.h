#pragma once
#include "items/consumable.h"
#include "items/loottable.h"

class ItemRegistry {
public:
    Consumable healthPotion = Consumable("Health potion", {7, 0, 0, 0, 0, 0});
    Consumable energizingPotion = Consumable("Energizing potion", {7, 2, 1, 0, 0, 0});
    Consumable stoneskinPotion = Consumable("Stone skin potion", {0, 0, 0, 1, 0, 0});

    Wearable amethystCloak = Wearable("Amethyst cloak", false, {0, 0, 0, 1, 0, 0});
    Wearable amethystDagger = Wearable("Amethyst dagger", true, {0, 0, 1, 0, 0, 0});

    Wearable amethystArmour = Wearable("Amethyst armour", false, {0, 0, 0, 3, 0, 0});
    Wearable amethystAxe = Wearable("Amethyst battle axe", true, {0, 0, 3, 0, 0, 0});

    LootTable *basicTable = LootTableFactory()
                            .ChanceOfFailure(8)
                            .AddItem(healthPotion, 2)
                            .Return();
    LootTable *yellowSlimeTable = LootTableFactory()
                            .ChanceOfFailure(15)
                            .AddItem(healthPotion, 4)
                            .AddItem(stoneskinPotion, 1)
                            .Return();
    LootTable *purpleSlimeTable = LootTableFactory()
                                  .ChanceOfFailure(25)
                                  .AddItem(stoneskinPotion, 10)
                                  .AddItem(healthPotion, 10)
                                  .AddItem(energizingPotion, 5)
                                  .Return();
    LootTable *amethystGolemTable = LootTableFactory()
                                    .ChanceOfFailure(0)
                                    .AddItem(amethystArmour, 1)
                                    .AddItem(amethystAxe, 1)
                                    .Return();
    LootTable *amethystMinionTable = LootTableFactory()
                                    .ChanceOfFailure(0)
                                    .AddItem(amethystCloak, 5)
                                    .AddItem(amethystDagger, 5)
                                    .AddItem(energizingPotion, 1)
                                    .Return();

    ~ItemRegistry() {
        delete basicTable;
        delete yellowSlimeTable;
        delete purpleSlimeTable;
        delete amethystGolemTable;
        delete amethystMinionTable;
    }
};