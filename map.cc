#include <ctype.h>
#include <iostream>
#include <string>
#include <fstream>
#include "map.h"

#define SUCCESS 0
#define FAILURE 1

#define LARGEST_CONTROL_CHARACTER 31

Map::Map(void) {
    __m_initialize();
}

Map::~Map(void) {
    removeMap();
}

bool Map::createMap(const char* filename) {
    int i, j;
    std::string line;
    std::ifstream file;

    __m_number_of_carrot = __m_score = 0;

    file.open(filename);

    getline(file, line);
    if (!__m_setWidth(line.c_str())) return false;

    getline(file, line);
    if (!__m_setHeight(line.c_str())) return false;

    __m_map = new char*[__m_height];
    for (i = 0; i < __m_height; ++i) __m_map[i] = new char[__m_width];

    for (i = 0; i < __m_height; ++i) {
        if (!getline(file, line)) {
            removeMap();
            return false;
        }

        const char* c_line = line.c_str();
        for (j = 0; j < __m_width; ++j) {
            if (!__m_isValidMark(c_line[j])) {
                removeMap();
                return false;
            }

            if ((__m_map[i][j] = c_line[j]) == A_CARROT) __m_number_of_carrot += 1;
        }
    }

    file.close();

    return true;
}

bool Map::removeMap(void) {
    if (!__m_width || !__m_height) return FAILURE;

    for (int i = 0; i < __m_height; ++i) delete [] __m_map[i];
    delete [] __m_map;

    __m_initialize();

    return SUCCESS;
}

bool Map::print(const int offset) {
    if (!__m_isMapExist()) return false;

    int i, j;
    int edge = __m_width + 2;

    for (i = 0; i < offset; ++i) std::cout << DESPLAY_A_SPACE;
    for (i = 0; i < edge; ++i) std::cout << DESPLAY_A_BAFFLE;
    std::cout << std::endl;

    for (i = 0; i < __m_height; ++i) {
        for (j = 0; j < offset; ++j) std::cout << DESPLAY_A_SPACE;
        std::cout << DESPLAY_A_BAFFLE;

        for (j = 0; j < __m_width; ++j) {
            switch (__m_map[i][j]) {
                case FACE_RIGHT:
                    std::cout << DESPLAY_FACE_RIGHT;
                    break;
                case FACE_DOWN:
                    std::cout << DESPLAY_FACE_DOWN;
                    break;
                case FACE_LEFT:
                    std::cout << DESPLAY_FACE_LEFT;
                    break;
                case FACE_UP:
                    std::cout << DESPLAY_FACE_UP;
                    break;
                case A_SPACE:
                    std::cout << DESPLAY_A_SPACE;
                    break;
                case A_CARROT:
                    std::cout << DESPLAY_A_CARROT;
                    break;
                default:
                    std::cout << DESPLAY_A_BAFFLE;
                    break;
            }
            std::cout << std::flush;
        }
        std::cout << DESPLAY_A_BAFFLE << std::endl;
    }

    for (i = 0; i < offset; ++i) std::cout << DESPLAY_A_SPACE;
    for (i = 0; i < edge; ++i) std::cout << DESPLAY_A_BAFFLE;
    std::cout << std::endl;

    return true;
}

Map& Map::goAhead(void) {
    int curr_x, curr_y;

    __m_searchRabbitPosition(curr_x, curr_y);
    int next_x = curr_x;
    int next_y = curr_y;

    switch (__m_map[curr_y][curr_x]) {
        case FACE_RIGHT:
            next_x = ((next_x + 1) == __m_width) ? next_x: next_x + 1;
            break;
        case FACE_DOWN:
            next_y = ((next_y + 1) == __m_height) ? next_y: next_y + 1;
            break;
        case FACE_LEFT:
            next_x = (!next_x) ? next_x: next_x - 1;
            break;
        default:
            next_y = (!next_y) ? next_y: next_y - 1;
            break;
    }

    if (next_x == curr_x && next_y == curr_y) return *this;
    if (__m_map[next_y][next_x] == A_BAFFLE) return *this;

    if (__m_map[next_y][next_x] == A_CARROT) __m_score += 1;
    __m_map[next_y][next_x] = __m_map[curr_y][curr_x];
    __m_map[curr_y][curr_x] = A_SPACE;

    return *this;
}

Map& Map::turnLeft(void) {
    int x, y;

    __m_searchRabbitPosition(x, y);

    switch (__m_map[y][x]) {
        case FACE_RIGHT:
            __m_map[y][x] = FACE_UP;
            return *this;
        case FACE_DOWN:
            __m_map[y][x] = FACE_RIGHT;
            return *this;
        case FACE_LEFT:
            __m_map[y][x] = FACE_DOWN;
            return *this;
        default:
            __m_map[y][x] = FACE_LEFT;
            return *this;
    }

    return *this;
}

Map& Map::turnRight(void) {
    int x, y;

    __m_searchRabbitPosition(x, y);

    switch (__m_map[y][x]) {
        case FACE_RIGHT:
            __m_map[y][x] = FACE_DOWN;
            return *this;
        case FACE_DOWN:
            __m_map[y][x] = FACE_LEFT;
            return *this;
        case FACE_LEFT:
            __m_map[y][x] = FACE_UP;
            return *this;
        default:
            __m_map[y][x] = FACE_RIGHT;
            return *this;
    }

    return *this;
}

bool Map::isPassed(void) {
    if (!__m_isMapExist()) return false;
    if (__m_score != __m_number_of_carrot) return false;
    return true;
}

void Map::__m_initialize(void) {
    __m_map = NULL;
    __m_number_of_carrot = __m_score = __m_width = __m_height = 0;
}

bool Map::__m_setWidth(const char* str) {
    if (!__m_isPositiveNumber(str)) return false;
    __m_width = atoi(str);
    return true;
}


bool Map::__m_setHeight(const char* str) {
    if (!__m_isPositiveNumber(str)) return false;
    __m_height = atoi(str);
    return true;
}

bool Map::__m_isMapExist(void) {
    if (!__m_width) return false;
    if (!__m_height) return false;
    if (!__m_map) return false;
    return true;
}

bool Map::__m_isValidMark(const char c) {
    switch (c) {
        case FACE_RIGHT : return true;
        case FACE_LEFT : return true;
        case FACE_DOWN : return true;
        case FACE_UP : return true;
        case A_CARROT : return true;
        case A_BAFFLE : return true;
        case A_SPACE : return true;
        default  : return false;
    }
    return false;
}

bool Map::__m_isPositiveNumber(const char* str) {
    if (str[0] == '0') return false;
    for (int i = 0; str[i] > LARGEST_CONTROL_CHARACTER; ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool Map::__m_searchRabbitPosition(int& x, int& y) {
    if (!__m_isMapExist()) return false;

    int i, j;

    for (i = 0; i < __m_height; ++i) {
        for (j = 0; j < __m_width; ++j) {
            switch (__m_map[i][j]) {
                case FACE_RIGHT : goto found;
                case FACE_DOWN : goto found;
                case FACE_LEFT : goto found;
                case FACE_UP : goto found;
            }
        }
    }

    return false;

    found:
    x = j;
    y = i;

    return true;
}