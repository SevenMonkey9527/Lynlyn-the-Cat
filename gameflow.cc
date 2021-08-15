#include <unistd.h>
#include <ctype.h>
#include <iostream>
#include <stack>
#include "gameflow.h"

#define LARGEST_CONTROL_CHARACTER 31

GameFlow::GameFlow(void) {
    __m_curr_level = 1;
    __m_passed_level = 1;
}

GameFlow::~GameFlow(void) {}

GameFlow& GameFlow::getInstance(void) {
    static GameFlow instance;
    return instance;
}

GameFlow& GameFlow::gameStart(void) {
    std::cout << std::endl << "**** Welcome to play The-Rabbit ****" << std::endl;
    std::string filename;
    std::string message;
    std::string input;
    int jump_level;

    for (__m_curr_level = 1; __m_curr_level <= THE_FINAL_LEVEL;) {
        __m_getFilename(filename, __m_curr_level);

        __m_map.createMap(filename.c_str());

        __m_showInterface(input, message);

        if (input[0] == 'q') break;
        if (!__m_isValidCommands(input)) {
            message = "The command is incorrect!";
            continue;
        } else message = "";

        __m_executeCommands(input, 0, (int)input.length(), false);

        if (__m_map.isPassed()) {
            __m_showExecution(input, "Congratulation!! \\(O v O)/", CONGRATULATION_TIME_US);
            __m_levelUp();
        } else message = "Oops!! Try again! (o A O)!!";
        __m_map.removeMap();
    }

    if (__m_curr_level > THE_FINAL_LEVEL) {
        std::cout << std::endl << "Congratulation!! You are SUPER LYNLYN!!\\(O v O)/" << std::endl;
    }
    std::cout << std::endl << "**** Good bye. See you next time ****" << std::endl;
    std::cout << std::endl;

    return *this;
}

GameFlow& GameFlow::__m_getFilename(std::string& filename, const int level) {
    filename = MAP_INFORMATION_DIRECTORY;
    filename += "/level_";
    filename += std::to_string(level);
    filename += ".map";
    return *this;
}

GameFlow& GameFlow::__m_levelUp(void) {
    __m_curr_level += 1;
    if (__m_passed_level < __m_curr_level) __m_passed_level = __m_curr_level;
    return *this;
}

GameFlow& GameFlow::__m_showInterface(std::string& cmd, std::string& msg) {
    __m_newPage();
    std::cout << " - Level " << __m_curr_level << " - "<< std::endl;
    std::cout << std::endl;
    __m_printScreen();
    __m_printHint();
    std::cout << "Messages: " << msg << std::endl;
    std::cout << std::endl;
    __m_getCommands(cmd);

    return *this;
}

GameFlow& GameFlow::__m_showExecution(const std::string& cmd, const std::string& msg, const unsigned int gap) {
    __m_newPage();
    std::cout << " - Level " << __m_curr_level << " - "<< std::endl;
    std::cout << std::endl;
    __m_printScreen();
    __m_printHint();
    std::cout << "Messages: " << msg << std::endl;
    std::cout << "Commands: " << cmd << std::endl;
    usleep(gap);

    return *this;
}

GameFlow& GameFlow::__m_newPage(void) {
    for (int i = 0; i < TOP_OF_PAGE_DISTANCE; ++i) std::cout << std::endl;
    return *this;
}

GameFlow& GameFlow::__m_printScreen(void) {
    __m_map.print(DESPLAY_MAP_LEFT_DISTANCE);
    std::cout << std::endl;
    return *this;
}

GameFlow& GameFlow::__m_printHint(void) {
    std::cout << "--------------------" << std::endl;
    std::cout << "Option:" << std::endl;
    std::cout << " - exit: '" << OPTION_EXIT << "';" << std::endl;
    std::cout << "--------------------" << std::endl;
    std::cout << "Command:" << std::endl;
    std::cout << " - Go Ahead  :  '" << CONTROL_RABBIT_GO_AHEAD      << "';" << std::endl;
    std::cout << " - Turn Left :  '" << CONTROL_RABBIT_TURN_LEFT     << "';" << std::endl;
    std::cout << " - Turn Right:  '" << CONTROL_RABBIT_TURN_RIGHT    << "';" << std::endl;
    std::cout << " - Loop      :  ([commands]*[times]);" << std::endl;
    std::cout << "--------------------" << std::endl;
    return *this;
}

GameFlow& GameFlow::__m_getCommands(std::string& cmd) {
    std::cout << "Enter the commands: " << std::flush;
    std::cin >> cmd;
    return *this;
}

GameFlow& GameFlow::__m_executeCommands(const std::string& cmd, const int from, const int to, const bool isSub) {
    int i, j, times;
    int sub_from, sub_to;
    int cnt = 0;
    std::string str_times;

    if (!isSub) __m_showExecution(cmd, "", EXECUTION_INTERVAL_TIME_US);

    for (int i = from; i < to; ++i) {
        if (cmd[i] == ' ') continue;
        if (cmd[i] <= LARGEST_CONTROL_CHARACTER) continue;

        switch (cmd[i]) {
            case CONTROL_RABBIT_GO_AHEAD:
                if (cnt) break;
                __m_map.goAhead();
                __m_showExecution(cmd, "", EXECUTION_INTERVAL_TIME_US);
                break;

            case CONTROL_RABBIT_TURN_LEFT:
                if (cnt) break;
                __m_map.turnLeft();
                __m_showExecution(cmd, "", EXECUTION_INTERVAL_TIME_US);
                break;

            case CONTROL_RABBIT_TURN_RIGHT:
                if (cnt) break;
                __m_map.turnRight();
                __m_showExecution(cmd, "", EXECUTION_INTERVAL_TIME_US);
                break;

            case '(':
                if (cnt++) break;
                sub_from = i + 1;
                break;

            case '*':
                if (cnt != 1) break;
                sub_to = i;
                str_times = "";
                break;

            case ')':
                if (--cnt) break;
                times = atoi(str_times.c_str());
                for (j = 0; j < times; ++j) __m_executeCommands(cmd, sub_from, sub_to, true);
                break;

            default:
                if (cnt != 1) break;
                str_times += cmd[i];
                break;
        }
    }

    return *this;
}

bool GameFlow::__m_isValidCommands(const std::string& cmd) {
    std::stack<char> stack;
    std::string input = "(";
    std::string digit_buffer;
    bool allowAlphabet = true;      // true:= allow alphabet only;  false:= allow digit only;
    int i, len;

    len = cmd.length();
    for (i = 0; i < len; ++i) input += cmd[i];
    input += "*1)";

    len = input.length();
    for (int i = 0; i < len; ++i) {
        if (input[i] == ' ') continue;
        if (input[i] <= LARGEST_CONTROL_CHARACTER) continue;
        std::cout << input[i] << std::endl;

        switch (input[i]) {
            case CONTROL_RABBIT_GO_AHEAD:
                if (!allowAlphabet) return false;
                stack.push(input[i]);
                break;

            case CONTROL_RABBIT_TURN_LEFT:
                if (!allowAlphabet) return false;
                stack.push(input[i]);
                break;

            case CONTROL_RABBIT_TURN_RIGHT:
                if (!allowAlphabet) return false;
                stack.push(input[i]);
                break;

            case '(':
                if (!allowAlphabet) return false;
                stack.push(input[i]);
                break;

            case '*':
                if (!allowAlphabet) return false;
                while (!stack.empty()) {
                    if (stack.top() == '(') break;
                    stack.pop();
                }
                if (stack.empty()) return false;
                stack.push(input[i]);
                allowAlphabet = false;
                digit_buffer = "";
                break;

            case ')':
                if (allowAlphabet) return false;
                if (!__m_isNonNegativeNumber(digit_buffer.c_str())) return false;

                if (stack.empty()) return false;
                if (stack.top() != '*') return false;
                stack.pop();

                if (stack.empty()) return false;
                if (stack.top() != '(') return false;
                stack.pop();

                allowAlphabet = true;
                break;

            default:
                if (!isdigit(input[i])) return false;
                if (allowAlphabet) return false;
                digit_buffer += input[i];
                break;
        }
    }

    if (!stack.empty()) return false;

    return true;
}

bool GameFlow::__m_isNonNegativeNumber(const char* str) {
    if (!str[0]) return false;
    if (str[0] == '0' && !str[1]) return true;

    for (int i = 0; str[i]; ++i) {
        if (!isdigit(str[i])) return false;
    }

    return true;
}