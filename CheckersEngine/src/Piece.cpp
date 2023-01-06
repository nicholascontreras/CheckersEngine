#include "Piece.h"

Piece::Piece(Color color, bool isKing) : color(color), isKing(isKing) {
}

bool Piece::operator==(const Piece other) const {
    return color == other.color && isKing == other.isKing;
}

std::pair<Direction, Direction> Piece::getForwardDirections() const {
    return color == Color::BLACK ? std::pair(Direction::SOUTH_EAST, Direction::SOUTH_WEST) : std::pair(Direction::NORTH_EAST, Direction::NORTH_WEST);
}

std::pair<Direction, Direction> Piece::getBackwardDirections() const {
    return color == Color::RED ? std::pair(Direction::SOUTH_EAST, Direction::SOUTH_WEST) : std::pair(Direction::NORTH_EAST, Direction::NORTH_WEST);
}

Piece::Color Piece::getOppositeColor(Color color) {
    return color == Color::BLACK ? Color::RED : Color::BLACK;
}

char Piece::toChar() const {
    if(color == Color::BLACK) {
        return isKing ? 'B' : 'b';
    } else {
        return isKing ? 'R' : 'r';
    }
}
