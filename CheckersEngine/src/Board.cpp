#include "Board.h"

#include <stdexcept>

Board::Board() : pieces(generateStartingPosition()) {
}

bool Board::operator==(const Board& other) const {
    return pieces == other.pieces;
}

bool Board::isPieceAt(Coord coord) const {
    return pieces.at(arrayIndex(coord)).has_value();
}

Piece Board::getPieceAt(Coord coord) const {
    if(!isPieceAt(coord)) {
        throw std::invalid_argument("No Piece at given Coord!");
    }
    return pieces.at(arrayIndex(coord)).value();
}

Board Board::applyMove(Move move) const {
    if(!isPieceAt(move.from)) {
        throw std::invalid_argument("Illegal Move, no piece at starting Coord!");
    }
    if(isPieceAt(move.to)) {
        throw std::invalid_argument("Illegal Move, piece already at ending Coord!");
    }
    if(move.isCapture()) {
        if(!isPieceAt(move.getCapturedCoord())) {
            throw std::invalid_argument("Illegal Move, no piece at expected capture Coord!");
        }
        if(getPieceAt(move.getCapturedCoord()).color == getPieceAt(move.from).color) {
            throw std::invalid_argument("Illegal Move, piece at capture Coord is friendly!");
        }
    }

    // Determine if the moved Piece should be a king
    Piece pieceMoving = getPieceAt(move.from);
    bool isKing = pieceMoving.isKing;
    if(!isKing) {
        if(pieceMoving.color == Piece::Color::BLACK) {
            if(move.to.row == Board::SIZE - 1) {
                isKing = true;
            }
        } else {
            if(move.to.row == 0) {
                isKing = true;
            }
        }
    }

    // Remove the old Piece, create new Piece
    std::vector<std::optional<Piece>> newPieces = pieces;
    newPieces.at(arrayIndex(move.from)).reset();
    newPieces.at(arrayIndex(move.to)).emplace(Piece(pieceMoving.color, isKing));
    
    // Remove the Piece we captured, if the Move was a capture
    if(move.isCapture()) {
        newPieces.at(arrayIndex(move.getCapturedCoord())).reset();
    }

    return Board(newPieces);
}

Board Board::applyTurn(Turn turn) const {
    Board newBoard = *this;
    for(Move move : turn.moves) {
        newBoard = newBoard.applyMove(move);
    }
    return newBoard;
}

int Board::numPiecesForColor(Piece::Color color) const {
    int count = 0;
    for(std::optional<const Piece> curPiece : pieces) {
        if(curPiece.has_value() && curPiece.value().color == color) {
            count++;
        }
    }

    return count;
}

int Board::scoreForColor(Piece::Color color) const {
    return numPiecesForColor(color) - numPiecesForColor(Piece::getOppositeColor(color));
}

std::string Board::toString() const {
    std::string output;

    output += std::string(SIZE + 2, '-') + "\r\n";
    for(int row = 0; row < SIZE; row++) {
        output += '|';
        for(int col = 0; col < SIZE; col++) {
            output += isPieceAt({ row, col }) ? getPieceAt({ row, col }).toChar() : ' ';
        }
        output += "|\r\n";
    }
    output += std::string(SIZE + 2, '-');

    return output;
}

Board::Board(std::vector<std::optional<Piece>> pieces) : pieces(pieces) {
}

int Board::arrayIndex(Coord coord) const {
    return (coord.row * SIZE) + coord.col;
}

std::vector<std::optional<Piece>> Board::generateStartingPosition() {
    std::vector<std::optional<Piece>> startingPosition;

    for(int row = 0; row < SIZE; row++) {
        for(int col = 0; col < SIZE; col++) {
            if((row + col) % 2 == 1) {
                if(row < 3) {
                    startingPosition.push_back(std::make_optional(Piece(Piece::Color::BLACK, false)));
                } else if (row > 4) {
                    startingPosition.push_back(std::make_optional(Piece(Piece::Color::RED, false)));
                } else {
                    startingPosition.push_back(std::nullopt);
                }
            } else {
                startingPosition.push_back(std::nullopt);
            }
        }
    }

    return startingPosition;
}
