#include "CheckersUI.h"

#include <thread>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <conio.h>

#include "../Engine.h"

#include "CharacterImage.h"

#include "console_utils.h"

Board CheckersUI::board;
CheckersUI::Cursor CheckersUI::cursor;

std::optional<Coord> CheckersUI::selectedPiece = std::nullopt;
std::string CheckersUI::messageText = "";

std::optional<Turn> CheckersUI::attemptedTurn = std::nullopt;
std::vector<Turn> CheckersUI::legalTurns;

void CheckersUI::run() {
    enableVirtualConsole();
    std::cout << "\x1b[2J" << std::flush;

    legalTurns = Engine::getAllTurnOptions(board, Piece::Color::BLACK);

    Board prevBoard = board;
    while(true) {
        if(!(board == prevBoard) || cursor.shouldRedraw()) {
            drawBoard();
        }

        cursor.tickBlink();
        handleKeyPress();
        prevBoard = board;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void CheckersUI::setBoard(Board b) {
    board = b;
}

void CheckersUI::drawBoard() {
    CharacterImage cell = {
        "------",
        "|     ",
        "|     ",
    };

    CharacterImage selected = {
        "      ",
        "      ",
        "      ",
        "  ^^^ "
    };

    CharacterImage black = {
    "      ",
    "  ### ",
    "  ### ",
    };

    CharacterImage red = {
        "      ",
        "  OOO ",
        "  OOO ",
    };

    CharacterImage king = {
        "      ",
        "  K K ",
        "  K K ",
    };

    CharacterImage rowCloser = {
        "|",
        "|",
        "|"
    };

    CharacterImage gridCloser = { std::string((size_t)cell.getWidth() * Board::SIZE + 1, '-') };

    CharacterImage drawing(cell.getWidth() * Board::SIZE + 1, cell.getHeight() * Board::SIZE + 1);

    for(int row = 0; row < Board::SIZE; row++) {
        for(int col = 0; col < Board::SIZE; col++) {
            drawing.draw(col * cell.getWidth(), row * cell.getHeight(), cell);

            if(board.isPieceAt({ row, col })) {
                Piece curPiece = board.getPieceAt({ row, col });

                if(curPiece.color == Piece::Color::BLACK) {
                    drawing.draw(col * cell.getWidth(), row * cell.getHeight(), black);
                } else {
                    drawing.draw(col * cell.getWidth(), row * cell.getHeight(), red);
                }

                if(curPiece.isKing) {
                    drawing.draw(col * cell.getWidth(), row * cell.getHeight(), king);
                }
            }
        }
        drawing.draw(drawing.getWidth() - 1, row * cell.getHeight(), rowCloser);
    }
    drawing.draw(0, drawing.getHeight() - 1, gridCloser);

    if(cursor.shouldShow()) {
        drawing.draw(cursor.getCol() * cell.getWidth(), cursor.getRow() * cell.getHeight(), selected);
    }

    drawing.translate437();

    // Clear old grid
    for(int i = 0; i < drawing.getHeight() + 1; i++) {
        std::cout << "\x1b[1A";
    }
    std::cout << std::flush;

    // Draw grid
    std::cout << drawing.toString() << "\r\n" << std::left << std::setw(30) << (messageText + std::string(30, ' ')) << std::endl;
}

void CheckersUI::handleKeyPress() {
    if(!cursor.isEnabled()) {
        return;
    }

    if(_kbhit()) {
        int c = _getch();
        if(c == 0 || c == 224) {
            int ex = _getch();
            switch(ex) {
            case 72: // Up
                cursor.up();
                break;
            case 75: // Left
                cursor.left();
                break;
            case 77: // Right
                cursor.right();
                break;
            case 80: // Down
                cursor.down();
                break;
            }
        } else if(c == ' ') {
            if(!selectedPiece.has_value()) {
                if(board.isPieceAt({ cursor.getRow(), cursor.getCol() })) {
                    Piece curPiece = board.getPieceAt({ cursor.getRow(), cursor.getCol() });
                    if(curPiece.color == Piece::Color::BLACK) {
                        selectedPiece.emplace(Coord(cursor.getRow(), cursor.getCol()));
                        messageText = "Piece selected, choose destination...";
                    }
                }
            } else {
                Move attemptedMove = Move(selectedPiece.value(), { cursor.getRow(), cursor.getCol() });
                if(attemptedTurn.has_value()) {
                    attemptedTurn.emplace(attemptedTurn.value().addMove(attemptedMove));
                } else {
                    attemptedTurn.emplace(Turn(attemptedMove));
                }

                std::vector<Turn>::iterator startsWithResultItr = std::find_if(legalTurns.begin(), legalTurns.end(), [](Turn curLegalTurn) {
                    return curLegalTurn.startsWith(attemptedTurn.value());
                });

                if(startsWithResultItr == legalTurns.end()) {
                    messageText = "Illegal move, try again";
                } else {
                    cursor.setEnabled(false);
                    board = board.applyMove(attemptedMove);
                    messageText = "Opponent is thinking...";

                    std::vector<Turn>::iterator foundItr = std::find(legalTurns.begin(), legalTurns.end(), attemptedTurn.value());
                    if(foundItr != legalTurns.end()) {
                        std::thread([]() {
                            std::optional<Turn> selectedTurn = Engine::getBestTurn(board, Piece::Color::RED, 6);

                            if(selectedTurn.has_value()) {
                                messageText = "Opponent is moving...";
                                for(Move curMove : selectedTurn.value().moves) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                                    board = board.applyMove(curMove);
                                }
                                legalTurns = Engine::getAllTurnOptions(board, Piece::Color::BLACK);
                                if(!legalTurns.empty()) {
                                    messageText = "Select Piece";
                                    cursor.setEnabled(true);
                                } else {
                                    messageText = "Game Over: You Lose!";
                                }
                            } else {
                                messageText = "Game Over: You Win!";
                            }
                        }).detach();
                    } else {
                        messageText = "Continue turn...";
                    }
                }

                selectedPiece.reset();
                attemptedTurn.reset();
            }
        }
    }
}
