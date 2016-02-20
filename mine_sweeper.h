
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#ifndef MINE_SWEEPER_H
#define MINE_SWEEPER_H

// Enumerator for each tile struct
enum Visibility {REVEALED, COVERED, QUESTIONED, MARKED};

// Struct for each tile
typedef struct Tile_Struct{
    int isMine;
    int surroundingMines;
    enum Visibility visibility;
} Tile;

// Struct for the game board
typedef struct Board_struct{
    int rows, cols, numMines, randSeed, userRow, userCol;
    int cancel;
    Tile ** tiles;
} Board;

// Functions
void read_args(Board* b_ptr, int argc, char* argv[]);
Board create_board(int argc, char* argv[]);
void print_board(Board b);
void place_mines(Board* b_ptr);
void count_mines(Board* b);
int count_marks(Board b);
void get_play(Board* b);
void enter_action(Board* b_ptr);
void reveal_tiles(Board* b_ptr, int curRow, int curCol);
int game_won(Board b);
void player_won(Board *b_ptr);
void game_over(Board *b_ptr);
void play_game(Board* b_ptr);
void destroy_board(Board* b_ptr);

#endif