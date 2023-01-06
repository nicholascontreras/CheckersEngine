#include <iostream>

#include "Board.h"

#include "Engine.h"

#include "ui/CheckersUI.h"

int main() {

    Board b;
    CheckersUI::setBoard(b);
    CheckersUI::run();
    return 0;
}