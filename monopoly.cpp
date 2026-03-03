#include "Manager.h"
#include <iostream>
#include <locale>

int main() {
    // Поддержка русского языка
    std::setlocale(0, "rus");

    std::cout << "╔════════════════════════════════════╗" << std::endl;
    std::cout << "║       М О Н О П О Л И Я            ║" << std::endl;
    std::cout << "║       Консольная версия C++        ║" << std::endl;
    std::cout << "╚════════════════════════════════════╝" << std::endl;

    GameManager game;
    game.init();
    game.start();

    std::cout << "\nСпасибо за игру!" << std::endl;
    return 0;
}