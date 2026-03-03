#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "game_types.h"
#include "board.h"
#include "deck.h"
#include <vector>
#include <string>

class GameManager {
private:
    Board board;
    Deck deck;
    std::vector<Player> players;
    int currentPlayerIndex;
    GameState state;
    bool gameOver;

    // Внутренние методы
    void handleCellAction(Player& player, Cell& cell);
    void handleCardAction(Player& player, const Card& card);
    void transferMoney(int fromId, int toId, int amount);
    void checkBankruptcy(int playerId);
    void checkWinCondition();

public:
    GameManager();

    // === ЖИЗНЕННЫЙ ЦИКЛ ===
    void init();  // Инициализация игры
    void start();  // Запуск игрового цикла
    void end();  // Завершение игры

    // === ХОД ИГРЫ ===
    void rollDice();  // Бросок кубиков
    void movePlayer(int steps);  // Движение игрока
    void endTurn();  // Завершение хода

    // === ВВОД/ВЫВОД ===
    void displayStatus() const;  // Показать текущее состояние
    void displayPlayerInfo(int playerId) const;  // Информация об игроке

    // === ГЕТТЕРЫ ===
    int getCurrentPlayerIndex() const { return currentPlayerIndex; }
    const Player& getCurrentPlayer() const { return players[currentPlayerIndex]; }
    const Player& getPlayer(int id) const { return players[id]; }
    const Board& getBoard() const { return board; }
    bool isGameOver() const { return gameOver; }
    GameState getState() const { return state; }

    // === УТИЛИТЫ ===
    int rollDiceValue();  // Генерация случайного числа 2-12
};

#endif // GAME_MANAGER_H

