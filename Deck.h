#ifndef DECK_H
#define DECK_H

// УДАЛЕНО: #define GAME_TYPES_H (эта строка ломала включение game_types.h!)
#include "game_types.h"  // ← Теперь Card будет корректно импортирован
#include <vector>

class Deck {
private:
    std::vector<Card> chestCards;
    std::vector<Card> chanceCards;
    int chestIndex;
    int chanceIndex;

public:
    Deck();
    void init();

    const Card& drawChest();
    const Card& drawChance();
    void reset();

    int getChestSize() const { return chestCards.size(); }
    int getChanceSize() const { return chanceCards.size(); }
};

#endif // DECK_H
