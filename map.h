#ifndef _MAP_H_
#define _MAP_H_

#define FACE_RIGHT  '>'
#define FACE_DOWN   'v'
#define FACE_LEFT   '<'
#define FACE_UP     '^'
#define A_SPACE     'o'
#define A_CARROT    '$'
#define A_BAFFLE    '#'

#define DESPLAY_FACE_RIGHT  "→"
#define DESPLAY_FACE_DOWN   "↓"
#define DESPLAY_FACE_LEFT   "←"
#define DESPLAY_FACE_UP     "↑"
#define DESPLAY_A_SPACE     " "
#define DESPLAY_A_CARROT    "$"
#define DESPLAY_A_BAFFLE    "▉"

class Map {
public:
    Map(void);
    ~Map(void);

    bool createMap(const char* filename);
    bool removeMap(void);
    bool print(const int offset = 0);

    Map& goAhead(void);
    Map& turnLeft(void);
    Map& turnRight(void);
    bool isPassed(void);

private:
    char** __m_map;
    int __m_width;
    int __m_height;
    int __m_number_of_carrot;
    int __m_score;

    void __m_initialize(void);
    bool __m_setWidth(const char* str);
    bool __m_setHeight(const char* str);

    bool __m_isMapExist(void);
    bool __m_isValidMark(const char c);
    bool __m_isPositiveNumber(const char* str);

    bool __m_searchRabbitPosition(int& x, int& y);
};

#endif