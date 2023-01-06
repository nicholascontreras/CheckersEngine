#include "CharacterImage.h"

#include <stdexcept>

CharacterImage::CharacterImage() {
}

CharacterImage::CharacterImage(int width, int height) {
    for(int i = 0; i < height; i++) {
        imageRows.push_back(std::string(width, ' '));
    }
}

CharacterImage::CharacterImage(std::initializer_list<std::string> rowLiterals) {
    for(std::string curRow : rowLiterals) {
        if(!imageRows.empty()) {
            if(curRow.size() != imageRows.front().size()) {
                throw std::invalid_argument("Image must be rectangular!");
            }
        }

        imageRows.push_back(curRow);
    }
}

void CharacterImage::draw(int x, int y, CharacterImage img) {
    if(!isOnImage(x, y)) {
        throw std::invalid_argument("Given position must be on the CharacterImage");
    }
    if(!isOnImage(x + img.getWidth() - 1, y + img.getHeight() - 1)) {
        throw std::invalid_argument("Given imageRows will exceed the bounds of the CharacterImage at the given position!");
    }

    for(int i = 0; i < img.getWidth(); i++) {
        for(int j = 0; j < img.getHeight(); j++) {
            char adding = img.getCharAt(i, j);
            if(adding != ' ') {
                setCharAt(i + x, j + y, adding);
            }
        }
    }
}

void CharacterImage::setCharAt(int x, int y, char c) {
    if(!isOnImage(x, y)) {
        throw std::invalid_argument("Given position must be on the CharacterImage");
    }

    imageRows.at(y).at(x) = c;
}

char CharacterImage::getCharAt(int x, int y) const {
    if(!isOnImage(x, y)) {
        throw std::invalid_argument("Given position must be on the CharacterImage");
    }

    return imageRows.at(y).at(x);
}

void CharacterImage::translate437() {
    CharacterImage orgCopy = *this;

    for(int x = 0; x < getWidth(); x++) {
        for(int y = 0; y < getHeight(); y++) {
            if(orgCopy.getCharAt(x, y) == '|' || orgCopy.getCharAt(x, y) == '-') {
                bool left = false, right = false, top = false, bottom = false;

                // Determine connections to adjacent symbols
                if(isOnImage(x - 1, y)) {
                    if(orgCopy.getCharAt(x - 1, y) == '-') {
                        left = true;
                    }
                    if(orgCopy.getCharAt(x - 1, y) == '|' && orgCopy.getCharAt(x, y) == '-') {
                        left = true;
                    }
                }
                if(isOnImage(x + 1, y)) {
                    if(orgCopy.getCharAt(x + 1, y) == '-') {
                        right = true;
                    }
                    if(orgCopy.getCharAt(x + 1, y) == '|' && orgCopy.getCharAt(x, y) == '-') {
                        right = true;
                    }
                }
                if(isOnImage(x, y - 1)) {
                    if(orgCopy.getCharAt(x, y - 1) == '|') {
                        top = true;
                    }
                    if(orgCopy.getCharAt(x, y - 1) == '-' && orgCopy.getCharAt(x, y) == '|') {
                        top = true;
                    }
                }
                if(isOnImage(x, y + 1)) {
                    if(orgCopy.getCharAt(x, y + 1) == '|') {
                        bottom = true;
                    }
                    if(orgCopy.getCharAt(x, y + 1) == '-' && orgCopy.getCharAt(x, y) == '|') {
                        bottom = true;
                    }
                }

                // Choose the correct symbol based on the connections
                if(left && right && top && bottom) {
                    setCharAt(x, y, toChar(197));
                } else if(left && right && top) {
                    setCharAt(x, y, toChar(193));
                } else if(left && right && bottom) {
                    setCharAt(x, y, toChar(194));
                } else if(left && top && bottom) {
                    setCharAt(x, y, toChar(180));
                } else if(right && top && bottom) {
                    setCharAt(x, y, toChar(195));

                // Two connection cases, ensuring that at least one of the connections is in the same direction
                } else if(left && right) {
                    if(orgCopy.getCharAt(x, y) == '|') {
                        throw std::logic_error("Invalid shape, only horizontal connections on vertical bar!");
                    }
                    setCharAt(x, y, toChar(196));
                } else if(left && top) {
                    setCharAt(x, y, toChar(217));
                } else if(left && bottom) {
                    setCharAt(x, y, toChar(191));
                } else if(right && top) {
                    setCharAt(x, y, toChar(192));
                } else if(right && bottom) {
                    setCharAt(x, y, toChar(218));
                } else if(top && bottom) {
                    if(orgCopy.getCharAt(x, y) == '-') {
                        throw std::logic_error("Invalid shape, only vertical connections on horizontal bar!");
                    }
                    setCharAt(x, y, toChar(179));

                // Only one connection, verify connection is in same direction
                } else if(left || right) {
                    if(orgCopy.getCharAt(x, y) == '|') {
                        throw std::logic_error("Invalid shape, only horizontal connections on vertical bar!");
                    }
                    setCharAt(x, y, toChar(196));
                } else if(top || bottom) {
                    if(orgCopy.getCharAt(x, y) == '-') {
                        throw std::logic_error("Invalid shape, only vertical connections on horizontal bar!");
                    }
                    setCharAt(x, y, toChar(179));
                
                // No connections, just replace with matching line piece
                } else if (orgCopy.getCharAt(x, y) == '|') {
                    setCharAt(x, y, toChar(179));
                } else {
                    setCharAt(x, y, toChar(196));
                }
            }
        }
    }
}

int CharacterImage::getWidth() const {
    if(!imageRows.empty()) {
        return (int)imageRows.front().size();
    } else {
        return 0;
    }
}

int CharacterImage::getHeight() const {
    return (int)imageRows.size();
}

std::string CharacterImage::toString() const {
    std::string output;
    for(std::string curRow : imageRows) {
        output += curRow + "\r\n";
    }
    return output.substr(0, output.size() - 2);
}

bool CharacterImage::isOnImage(int x, int y) const {
    return x >= 0 && x < getWidth() && y >= 0 && y < getHeight();
}

char CharacterImage::toChar(unsigned char unsignedChar) {
    return (char)unsignedChar;
}
