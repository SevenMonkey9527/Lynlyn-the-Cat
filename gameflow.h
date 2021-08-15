#ifndef _GAMEFLOW_H_
#define _GAMEFLOW_H_

#include <string>
#include "def.h"
#include "map.h"

#define THE_FINAL_LEVEL 7
#define TOP_OF_PAGE_DISTANCE 50

class GameFlow {
public:
    static GameFlow& getInstance(void);

    GameFlow& gameStart(void);

private:
    GameFlow(void);
    ~GameFlow(void);

    Map __m_map;
    int __m_curr_level;
    int __m_passed_level;

    GameFlow& __m_getFilename(std::string& filename, const int level);
    GameFlow& __m_levelUp(void);
    GameFlow& __m_showInterface(std::string& cmd, std::string& msg);
    GameFlow& __m_showExecution(const std::string& cmd, const std::string& msg, const unsigned int gap);
    GameFlow& __m_newPage(void);
    GameFlow& __m_printScreen(void);
    GameFlow& __m_printHint(void);
    GameFlow& __m_getCommands(std::string& cmd);
    GameFlow& __m_executeCommands(const std::string& cmd, const int from, const int to, const bool isSub);

    bool __m_isValidCommands(const std::string& cmd);
    bool __m_isNonNegativeNumber(const char* str);
};

#endif