#include "Deck.h"

#include <iostream>

Deck::Deck() : chestIndex(0), chanceIndex(0) {}

void Deck::init() {
    chestCards.clear();
    chanceCards.clear();
    chestIndex = 0;
    chanceIndex = 0;

    // === ОБЩЕСТВЕННАЯ КАЗНА (8 карт) ===
    chestCards.emplace_back(1, "Вы выиграли в лотерею", CardEffect::MONEY_ADD, 100);
    chestCards.emplace_back(2, "Оплата за обучение", CardEffect::MONEY_SUB, 50);
    chestCards.emplace_back(3, "Получите наследство", CardEffect::MONEY_ADD, 150);
    chestCards.emplace_back(4, "Штраф за парковку", CardEffect::MONEY_SUB, 30);
    chestCards.emplace_back(5, "Банковский дивиденд", CardEffect::MONEY_ADD, 75);
    chestCards.emplace_back(6, "Ремонт дома", CardEffect::MONEY_SUB, 40);
    chestCards.emplace_back(7, "Подарок от друга", CardEffect::MONEY_ADD, 50);
    chestCards.emplace_back(8, "Медицинские расходы", CardEffect::MONEY_SUB, 60);

    // === ШАНС (8 карт) ===
    chanceCards.emplace_back(1, "Идите на СТАРТ", CardEffect::MOVE_TO, 0);
    chanceCards.emplace_back(2, "Идите в ТЮРЬМУ", CardEffect::GO_TO_JAIL, 8);
    chanceCards.emplace_back(3, "Выиграли конкурс", CardEffect::MONEY_ADD, 200);
    chanceCards.emplace_back(4, "Налоговый штраф", CardEffect::MONEY_SUB, 100);
    chanceCards.emplace_back(5, "Переместитесь на 3 клетки назад", CardEffect::MOVE_BACK, 3);
    chanceCards.emplace_back(6, "Ваша акция выросла", CardEffect::MONEY_ADD, 120);
    chanceCards.emplace_back(7, "Потеряли кошелек", CardEffect::MONEY_SUB, 80);
    chanceCards.emplace_back(8, "Бонус от банка", CardEffect::MONEY_SUB, 90);

    std::cout << "[DECK] Колоды инициализированы (Казна: " << chestCards.size()
        << ", Шанс: " << chanceCards.size() << ")" << std::endl;
}

const Card& Deck::drawChest() {
    const Card& card = chestCards[chestIndex];
    chestIndex = (chestIndex + 1) % chestCards.size();  // Циклически
    return card;
}

const Card& Deck::drawChance() {
    const Card& card = chanceCards[chanceIndex];
    chanceIndex = (chanceIndex + 1) % chanceCards.size();  // Циклически
    return card;
}

void Deck::reset() {
    chestIndex = 0;
    chanceIndex = 0;
}