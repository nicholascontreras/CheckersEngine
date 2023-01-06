#pragma once

#include <vector>

template <class T>
inline void appendVector(std::vector<T>& target, std::vector<T> toAppend) {
    target.insert(target.end(), toAppend.begin(), toAppend.end());
};