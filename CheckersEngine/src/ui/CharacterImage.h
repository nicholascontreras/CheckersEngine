#pragma once

#include <string>
#include <vector>
#include <initializer_list>

class CharacterImage {
public:
    CharacterImage();
    CharacterImage(int width, int height);
    CharacterImage(std::initializer_list<std::string> rowLiterals);

    void draw(int x, int y, CharacterImage img);

    void setCharAt(int x, int y, char c);
    char getCharAt(int x, int y) const;

    void translate437();

    int getWidth() const;
    int getHeight() const;

    std::string toString() const;
private:
    std::vector<std::string> imageRows;

    bool isOnImage(int x, int y) const;
    char toChar(unsigned char unsignedChar);
};
