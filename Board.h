#ifndef BOARD_H
#define BOARD_H


#include "game_types.h"
#include <vector>

class Board {
private:
    std::vector<Cell> cells;

public:
    // === ИНИЦИАЛИЗАЦИЯ ===
    Board();
    void init();  // Заполняет доску данными из ТЗ

    // === ДОСТУП К КЛЕТКАМ ===
    Cell& getCell(int id);
    const Cell& getCell(int id) const;
    int getBoardSize() const { return BOARD_SIZE; }

    // === ПОКУПКА И СТРОИТЕЛЬСТВО ===
    bool buyProperty(int cellId, int playerId);
    bool buildHouse(int cellId, int playerId);

    // === РЕНТА ===
    int calculateRent(int cellId) const;
    bool payRent(int cellId, int payerId, int ownerId);

    // === СБРОС СОБСТВЕННОСТИ (при банкротстве) ===
    void resetOwner(int playerId);

    // === ОТЛАДКА ===
    void printBoard() const;
}; // <--- ВАЖНО: Точка с запятой обязательна здесь!

#endif // BOARD_H
