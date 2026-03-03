#include "Manager.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

GameManager::GameManager()
    : currentPlayerIndex(0), state(GameState::WAITING_ROLL), gameOver(false) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameManager::init() {

    std::setlocale(0, "rus");
    std::cout << "\n=== ИНИЦИАЛИЗАЦИЯ ИГРЫ ===" << std::endl;

    board.init();
    deck.init();

    // Создание игроков
    std::string name1, name2;
    std::cout << "Введите имя Игрока 1: ";
    std::cin >> name1;
    std::cout << "Введите имя Игрока 2: ";
    std::cin >> name2;

    players.emplace_back(0, name1);
    players.emplace_back(1, name2);

    std::cout << "Игроки созданы. Стартовый капитал: " << START_MONEY << "$" << std::endl;
    std::cout << "==========================\n" << std::endl;
}

void GameManager::start() {
    std::cout << "\n=== ИГРА НАЧАЛАСЬ ===" << std::endl;

    while (!gameOver) {
        Player& player = players[currentPlayerIndex];

        // Пропуск банкротом
        if (player.isBankrupt) {
            std::cout << "[INFO] Игрок " << player.name << " банкрот, ход пропускается" << std::endl;
            endTurn();
            continue;
        }

        displayStatus();

        // === ЭТАП 1: ПРОВЕРКА ТЮРЬМЫ ===
        if (player.inJail) {
            std::cout << "\n[ТЮРЬМА] Вы в тюрьме!" << std::endl;
            std::cout << "1. Заплатить " << JAIL_FINE << "$ и выйти" << std::endl;
            std::cout << "2. Остаться еще на ход" << std::endl;

            int choice;
            std::cin >> choice;

            if (choice == 1 && player.canAfford(JAIL_FINE)) {
                player.changeMoney(-JAIL_FINE);
                player.inJail = false;
                player.jailTurns = 0;
                std::cout << "Вы вышли из тюрьмы!" << std::endl;
                // Продолжаем ход, бросаем кубики
            }
            else {
                player.jailTurns++;
                if (player.jailTurns >= 1) {
                    player.inJail = false;
                    player.jailTurns = 0;
                    std::cout << "Срок истек, вы свободны (но ход потрачен)" << std::endl;
                }
                else {
                    std::cout << "Остаетесь в тюрьме еще на ход" << std::endl;
                }
                endTurn();
                continue;  // Ход пропущен
            }
        }

        // === ЭТАП 2: БРОСОК КУБИКОВ ===
        std::cout << "\nНажмите Enter для броска кубиков...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        int dice = rollDiceValue();
        std::cout << "Выпало: " << dice << std::endl;

        // === ЭТАП 3: ДВИЖЕНИЕ ===
        int oldPos = player.position;
        movePlayer(dice);

        // Проверка прохождения СТАРТА
        if (player.position < oldPos) {
            player.changeMoney(PASS_START_BONUS);
            std::cout << "[СТАРТ] Вы прошли СТАРТ! +" << PASS_START_BONUS << "$" << std::endl;
        }

        // === ЭТАП 4: ОБРАБОТКА КЛЕТКИ ===
        Cell& cell = board.getCell(player.position);
        handleCellAction(player, cell);

        // === ЭТАП 5: ПРОВЕРКА БАНКРОТСТВА ===
        checkBankruptcy(currentPlayerIndex);

        // === ЭТАП 6: ЗАВЕРШЕНИЕ ХОДА ===
        if (!gameOver) {
            endTurn();
        }
    }

    end();
}

void GameManager::end() {
    std::cout << "\n=== ИГРА ОКОНЧЕНА ===" << std::endl;

    for (const auto& player : players) {
        if (!player.isBankrupt) {
            std::cout << "🏆 ПОБЕДИТЕЛЬ: " << player.name << " 🏆" << std::endl;
            break;
        }
    }
}

void GameManager::rollDice() {
    // Реализовано в start()
}

void GameManager::movePlayer(int steps) {
    Player& player = players[currentPlayerIndex];
    player.position = (player.position + steps) % BOARD_SIZE;

    std::cout << "[ДВИЖЕНИЕ] " << player.name << " переместился на клетку "
        << player.position << " (" << board.getCell(player.position).name << ")" << std::endl;
}

void GameManager::handleCellAction(Player& player, Cell& cell) {
    std::cout << "\n[КЛЕТКА] " << cell.name << " (" << cell.color << ")" << std::endl;

    switch (cell.type) {
    case CellType::START:
        // Бонус уже получен при движении
        break;

    case CellType::STREET:
        if (cell.isAvailable()) {
            // Можно купить
            std::cout << "Цена: " << cell.price << "$" << std::endl;
            if (player.canAfford(cell.price)) {
                std::cout << "Купить? (1 - Да, 0 - Нет): ";
                int buy;
                std::cin >> buy;
                if (buy == 1) {
                    player.changeMoney(-cell.price);
                    board.buyProperty(cell.id, player.id);
                    player.properties.push_back(cell.id);
                }
            }
            else {
                std::cout << "Недостаточно средств для покупки" << std::endl;
            }
        }
        else if (cell.isOwnedBy(player.id)) {
            // Своя улица - можно строить
            if (cell.houseCount < MAX_HOUSES) {
                int houseCost = cell.getHouseCost();
                std::cout << "Построить дом? Цена: " << houseCost
                    << "$ (текущих домов: " << cell.houseCount << ")" << std::endl;
                std::cout << "(1 - Да, 0 - Нет): ";
                int build;
                std::cin >> build;
                if (build == 1 && player.canAfford(houseCost)) {
                    player.changeMoney(-houseCost);
                    board.buildHouse(cell.id, player.id);
                }
            }
        }
        else {
            // Чужая улица - платим ренту
            int rent = board.calculateRent(cell.id);
            std::cout << "Владелец: Игрок " << (cell.ownerId + 1)
                << ", Рента: " << rent << "$" << std::endl;
            transferMoney(player.id, cell.ownerId, rent);
        }
        break;

    case CellType::TAX:
        std::cout << "[НАЛОГ] -" << TAX_AMOUNT << "$" << std::endl;
        player.changeMoney(-TAX_AMOUNT);
        break;

    case CellType::CHEST:
        std::cout << "[КАЗНА] Тянем карту..." << std::endl;
        {
            const Card& card = deck.drawChest();
            handleCardAction(player, card);
        }
        break;

    case CellType::CHANCE:
        std::cout << "[ШАНС] Тянем карту..." << std::endl;
        {
            const Card& card = deck.drawChance();
            handleCardAction(player, card);
        }
        break;

    case CellType::JAIL:
        std::cout << "[ТЮРЬМА] Просто посещение" << std::endl;
        break;

    case CellType::PARKING:
        std::cout << "[ПАРКОВКА] Бесплатная стоянка" << std::endl;
        break;
    }
}

void GameManager::handleCardAction(Player& player, const Card& card) {
    std::cout << ">>> Карта: " << card.text << std::endl;

    switch (card.effect) {
    case CardEffect::MONEY_ADD:
        player.changeMoney(card.value);
        std::cout << "+ " << card.value << "$" << std::endl;
        break;

    case CardEffect::MONEY_SUB:
        player.changeMoney(-card.value);
        std::cout << "- " << card.value << "$" << std::endl;
        break;

    case CardEffect::MOVE_TO:
        player.position = card.value;
        std::cout << "Перемещение на клетку " << card.value << std::endl;
        // Обработать новую клетку (без повторного взятия карт)
        {
            Cell& newCell = board.getCell(player.position);
            if (newCell.type == CellType::START) {
                player.changeMoney(PASS_START_BONUS);
            }
        }
        break;

    case CardEffect::MOVE_BACK:
        player.position = (player.position - card.value + BOARD_SIZE) % BOARD_SIZE;
        std::cout << "Перемещение на " << card.value << " клетки назад" << std::endl;
        break;

    case CardEffect::GO_TO_JAIL:
        player.position = card.value;
        player.inJail = true;
        player.jailTurns = 0;
        std::cout << "Отправлен в тюрьму!" << std::endl;
        break;

    default:
        break;
    }

    checkBankruptcy(player.id);
}

void GameManager::transferMoney(int fromId, int toId, int amount) {
    if (fromId == toId) return;

    Player& from = players[fromId];
    Player& to = players[toId];

    if (from.canAfford(amount)) {
        from.changeMoney(-amount);
        to.changeMoney(amount);
        std::cout << "[ПЕРЕВОД] " << amount << "$ от Игрока " << (fromId + 1)
            << " к Игроку " << (toId + 1) << std::endl;
    }
    else {
        // Недостаточно средств
        to.changeMoney(from.money);  // Отдаёт всё что есть
        from.changeMoney(-from.money);
        std::cout << "[ПЕРЕВОД] Неполная сумма: " << from.money << "$" << std::endl;
    }
}

void GameManager::checkBankruptcy(int playerId) {
    Player& player = players[playerId];

    if (player.money < 0 || player.isBankrupt) {
        player.isBankrupt = true;
        player.money = 0;
        board.resetOwner(playerId);
        player.clearProperties();

        std::cout << "\n!!! БАНКРОТСТВО !!!" << std::endl;
        std::cout << "Игрок " << player.name << " обанкротился!" << std::endl;

        checkWinCondition();
    }
}

void GameManager::checkWinCondition() {
    int activePlayers = 0;
    int lastActive = -1;

    for (size_t i = 0; i < players.size(); ++i) {
        if (!players[i].isBankrupt) {
            activePlayers++;
            lastActive = static_cast<int>(i);
        }
    }

    if (activePlayers == 1) {
        gameOver = true;
        std::cout << "\n[ПОБЕДА] Остался один игрок!" << std::endl;
    }
}

void GameManager::endTurn() {
    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
    state = GameState::WAITING_ROLL;
    std::cout << "\n--- Конец хода ---\n" << std::endl;
}

void GameManager::displayStatus() const {
    std::cout << "\n=== СОСТОЯНИЕ ИГРЫ ===" << std::endl;
    for (const auto& player : players) {
        std::string status = player.isBankrupt ? "БАНКРОТ" : std::to_string(player.money) + "$";
        std::string marker = (player.id == currentPlayerIndex) ? ">> " : "   ";
        std::cout << marker << player.name << ": " << status
            << " | Позиция: " << board.getCell(player.position).name << std::endl;
    }
    std::cout << "======================\n" << std::endl;
}

void GameManager::displayPlayerInfo(int playerId) const {
    const Player& player = players[playerId];
    std::cout << "\n=== ИНФОРМАЦИЯ ОБ ИГРОКЕ ===" << std::endl;
    std::cout << "Имя: " << player.name << std::endl;
    std::cout << "Деньги: " << player.money << "$" << std::endl;
    std::cout << "Позиция: " << player.position << std::endl;
    std::cout << "В тюрьме: " << (player.inJail ? "Да" : "Нет") << std::endl;
    std::cout << "Банкрот: " << (player.isBankrupt ? "Да" : "Нет") << std::endl;
    std::cout << "Свойства: " << player.properties.size() << " улиц" << std::endl;
    std::cout << "============================\n" << std::endl;
}

int GameManager::rollDiceValue() {
    return (std::rand() % 6) + 1;
}