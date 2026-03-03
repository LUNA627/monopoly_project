#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <string>
#include <vector>

// ===  ŒÕ—“¿Õ“€ (˜ÂÂÁ enum - ‡·ÓÚ‡ÂÚ ‚ Î˛·ÓÏ ÒÚ‡Ì‰‡ÚÂ C++) ===
enum GameConstants : int {
    BOARD_SIZE = 24,
    START_MONEY = 1500,
    PASS_START_BONUS = 200,
    JAIL_FINE = 50,
    TAX_AMOUNT = 100,
    MAX_HOUSES = 2,
    MAX_PLAYERS = 2
};

// === “»œ€  À≈“Œ  ===
enum class CellType {
    STREET,
    START,
    JAIL,
    PARKING,
    TAX,
    CHEST,
    CHANCE
};

// === “»œ€  ¿–“ ===
enum class CardEffect {
    MONEY_ADD,
    MONEY_SUB,
    MOVE_TO,
    MOVE_BACK,
    GO_TO_JAIL,
    FREE_PASS
};

// === —Œ—“ŒﬂÕ»≈ »√–€ ===
enum class GameState {
    WAITING_ROLL,
    MOVING,
    CELL_ACTION,
    CARD_ACTION,
    END_TURN,
    GAME_OVER
};

// === —“–” “”–¿  ¿–“€ ===
struct Card {
    int id;
    std::string text;
    CardEffect effect;
    int value;

    Card(int i, const std::string& t, CardEffect e, int v)
        : id(i), text(t), effect(e), value(v) {
    }
};

// === —“–” “”–¿  À≈“ » ===
struct Cell {
    int id;
    CellType type;
    std::string name;
    std::string color;
    int price;
    int baseRent;
    int rentValues[3];
    int ownerId;
    int houseCount;

    Cell(int i, CellType t, const std::string& n, const std::string& c,
        int p = 0, int r = 0)
        : id(i), type(t), name(n), color(c), price(p), baseRent(r),
        ownerId(-1), houseCount(0) {
        rentValues[0] = baseRent * 1;
        rentValues[1] = baseRent * 2;
        rentValues[2] = baseRent * 4;
    }

    int getCurrentRent() const {
        if (houseCount < 0 || houseCount > 2) return rentValues[0];
        return rentValues[houseCount];
    }

    int getHouseCost() const {
        return price / 2;
    }

    bool isAvailable() const {
        return type == CellType::STREET && ownerId == -1;
    }

    bool isOwnedBy(int playerId) const {
        return ownerId == playerId;
    }
};

// === —“–” “”–¿ »√–Œ ¿ ===
struct Player {
    int id;
    std::string name;
    int money;
    int position;
    bool isBankrupt;
    bool inJail;
    int jailTurns;
    std::vector<int> properties;

    Player(int i, const std::string& n)
        : id(i), name(n), money(START_MONEY), position(0),  // START_MONEY ÚÂÔÂ¸ ËÁ enum
        isBankrupt(false), inJail(false), jailTurns(0) {
    }

    bool canAfford(int amount) const {
        return money >= amount;
    }

    bool changeMoney(int amount) {
        money += amount;
        if (money < 0) {
            money = 0;
            return false;
        }
        return true;
    }

    void clearProperties() {
        properties.clear();
    }
};

#endif // GAME_TYPES_H