#include "gameflow.h"

#include <stdio.h>
int main(void) {
    GameFlow& flow = GameFlow::getInstance();

    flow.gameStart();

    return 0;
}