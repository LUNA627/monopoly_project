#include "Board.h"
#include <iostream>

Board::Board() {
    cells.reserve(BOARD_SIZE);
}

void Board::init() {
    cells.clear();

    // === ЗАПОЛНЕНИЕ ПО ДАННЫМ ИЗ ТЗ ===
    // Формат: Cell(id, type, name, color, price, baseRent)

    cells.emplace_back(0, CellType::START, "ВПЕРЕД (СТАРТ)", "—", 0, 0);
    cells.emplace_back(1, CellType::STREET, "Старый Проспект", "🟤", 60, 10);
    cells.emplace_back(2, CellType::STREET, "Улица Мира", "🟤", 80, 12);
    cells.emplace_back(3, CellType::CHEST, "Общественная казна", "📦", 0, 0);
    cells.emplace_back(4, CellType::STREET, "Цветочная", "🟦", 100, 15);
    cells.emplace_back(5, CellType::STREET, "Парковая", "🟦", 120, 18);
    cells.emplace_back(6, CellType::STREET, "Садовая", "🟪", 140, 20);
    cells.emplace_back(7, CellType::STREET, "Лесная", "🟪", 160, 22);
    cells.emplace_back(8, CellType::JAIL, "ТЮРЬМА", "🟧", 0, 0);
    cells.emplace_back(9, CellType::STREET, "Солнечная", "🟠", 180, 25);
    cells.emplace_back(10, CellType::STREET, "Речная", "🟠", 200, 28);
    cells.emplace_back(11, CellType::CHANCE, "Шанс", "❓", 0, 0);
    cells.emplace_back(12, CellType::TAX, "Налог", "⬜", 0, TAX_AMOUNT);
    cells.emplace_back(13, CellType::STREET, "Центральная", "🔴", 220, 30);
    cells.emplace_back(14, CellType::STREET, "Главная", "🔴", 240, 35);
    cells.emplace_back(15, CellType::CHEST, "Общественная казна", "📦", 0, 0);
    cells.emplace_back(16, CellType::STREET, "Звездная", "🟨", 260, 40);
    cells.emplace_back(17, CellType::STREET, "Победы", "🟨", 280, 45);
    cells.emplace_back(18, CellType::PARKING, "Бесплатная стоянка", "🅿️", 0, 0);
    cells.emplace_back(19, CellType::STREET, "Олимпийская", "🟩", 300, 50);
    cells.emplace_back(20, CellType::STREET, "Космическая", "🟩", 320, 55);
    cells.emplace_back(21, CellType::CHANCE, "Шанс", "❓", 0, 0);
    cells.emplace_back(22, CellType::STREET, "Элитная", "🟫", 350, 60);
    cells.emplace_back(23, CellType::STREET, "Премиум", "🟫", 400, 70);

    std::cout << "[BOARD] Доска инициализирована (" << cells.size() << " клеток)" << std::endl;
}

Cell& Board::getCell(int id) {
    return cells[id];
}

const Cell& Board::getCell(int id) const {
    return cells[id];
}

bool Board::buyProperty(int cellId, int playerId) {
    if (cellId < 0 || cellId >= BOARD_SIZE) return false;

    Cell& cell = cells[cellId];

    // Проверки
    if (cell.type != CellType::STREET) return false;
    if (cell.ownerId != -1) return false;  // Уже куплена

    // Покупка
    cell.ownerId = playerId;
    // Добавляем в список свойств игрока (делается в GameManager)

    std::cout << "[BOARD] Игрок " << playerId << " купил " << cell.name
        << " за " << cell.price << "$" << std::endl;

    return true;
}

bool Board::buildHouse(int cellId, int playerId) {
    if (cellId < 0 || cellId >= BOARD_SIZE) return false;

    Cell& cell = cells[cellId];

    // Проверки
    if (cell.type != CellType::STREET) return false;
    if (cell.ownerId != playerId) return false;  // Не принадлежит
    if (cell.houseCount >= MAX_HOUSES) return false;  // Максимум домов

    // Строительство
    cell.houseCount++;

    std::cout << "[BOARD] Построен дом на " << cell.name
        << " (всего домов: " << cell.houseCount << ")" << std::endl;

    return true;
}

int Board::calculateRent(int cellId) const {
    if (cellId < 0 || cellId >= BOARD_SIZE) return 0;
    return cells[cellId].getCurrentRent();
}

bool Board::payRent(int cellId, int payerId, int ownerId) {
    if (cellId < 0 || cellId >= BOARD_SIZE) return false;
    if (ownerId == -1 || ownerId == payerId) return false;  // Нет ренты

    int rent = calculateRent(cellId);
    // Перевод денег делается в GameManager

    std::cout << "[BOARD] Рента " << rent << "$ с игрока " << payerId
        << " владельцу " << ownerId << std::endl;

    return true;
}

void Board::resetOwner(int playerId) {
    for (auto& cell : cells) {
        if (cell.ownerId == playerId) {
            cell.ownerId = -1;
            cell.houseCount = 0;
        }
    }
    std::cout << "[BOARD] Свойства игрока " << playerId << " сброшены" << std::endl;
}

void Board::printBoard() const {
    std::cout << "\n=== СОСТОЯНИЕ ДОСКИ ===" << std::endl;
    for (const auto& cell : cells) {
        std::string owner = (cell.ownerId == -1) ? "БАНК" : "Игрок " + std::to_string(cell.ownerId);
        std::cout << cell.id << ". " << cell.name << " [" << cell.color << "] "
            << "| Владелец: " << owner << " | Дома: " << cell.houseCount << std::endl;
    }
    std::cout << "======================\n" << std::endl;
}
