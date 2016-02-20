// This program replicates the game MineSweeper. The user enters the
// desired dimensions, the desired number of mines, and an optional
// seed. The user then plays the game to its conclusion.

#include <stdio.h>
#include <stdlib.h>
#include "mine_sweeper.h"

int main(int argc, char **argv)
{
    Board gameBoard = create_board(argc, argv);
    play_game(&gameBoard);
    destroy_board(&gameBoard);
    return 0;
}