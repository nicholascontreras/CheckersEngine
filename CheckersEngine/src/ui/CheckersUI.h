#pragma once

#include <optional>

#include "../Board.h"
#include "../Coord.h"

class CheckersUI {
public:
    static void run();

    static void setBoard(Board b);
private:
    class Cursor {
    public:
        inline void up() { if(row > 0) { row--; resetBlink(); } };
        inline void down() { if(row < Board::SIZE - 1) { row++; resetBlink(); } };
        inline void left() { if(col > 0) { col--; resetBlink(); } };
        inline void right() { if(col < Board::SIZE - 1) { col++; resetBlink(); } };

        inline int getRow() { return row; };
        inline int getCol() { return col; };

        inline void tickBlink() {
            if(++blinkCounter == 20) {
                blinkCounter = 0;
                blinkVisible = !blinkVisible;
            }
        };
        inline void setEnabled(bool e) { enabled = e; };
        inline bool isEnabled() { return enabled; };
        inline bool shouldShow() { return enabled && blinkVisible; };
        inline bool shouldRedraw() { return blinkCounter == 0; };
        inline void resetBlink() { blinkVisible = true; blinkCounter = 0; };

    private:
        bool enabled = true;
        int row = 0, col = 0;
        bool blinkVisible = true;
        int blinkCounter = 0;
    };

    static Board board;
    static Cursor cursor;
    static bool redrawBoard;

    static std::optional<Coord> selectedPiece;
    static std::string messageText;

    static std::optional<Turn> attemptedTurn;
    static std::vector<Turn> legalTurns;

    static void drawBoard();
    static void handleKeyPress();
};