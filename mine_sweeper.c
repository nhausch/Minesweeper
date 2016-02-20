#include <stdio.h>
#include <stdlib.h>
#include "mine_sweeper.h"

void read_args(Board* b_ptr, int argc, char* argv[]){
    // This function reads the user's command line arguements and stores them in the Broard struct.
    
    // Too few args
    if (argc < 4) {
        printf("Not enough arguments. Usage:\n");
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
    // Too many args
    else if (argc > 5) {
        printf("Too many arguments. Usage:\n");
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
    // Without seed
    else if (argc == 4){
        b_ptr->rows = atoi(argv[1]);
        b_ptr->cols = atoi(argv[2]);
        b_ptr->numMines = atoi(argv[3]);
        srand((int)(time(0)));
        b_ptr->randSeed = rand();
    }
    // With seed
    else if (argc == 5){
        b_ptr->rows = atoi(argv[1]);
        b_ptr->cols = atoi(argv[2]);
        b_ptr->numMines = atoi(argv[3]);
        b_ptr->randSeed = atoi(argv[4]);
    }
    return;
}

Board create_board(int argc, char** argv){
    // This function allocates space and generates a board based on the command line args
    
    int i, j;
    Board board;
    board.cancel = 0;
    read_args(&board, argc, argv);
    
    board.tiles = (Tile**)malloc(board.rows * sizeof(Tile*));
    for (i = 0; i < board.rows; i ++) {
        board.tiles[i] = (Tile*)malloc((board.cols + 1) * sizeof(Tile));
    }
    for (i = 0; i < board.rows; i ++) {
        for (j = 0; j < board.cols; j ++) {
            board.tiles[i][j].isMine = 0;
            board.tiles[i][j].surroundingMines = 0;
            board.tiles[i][j].visibility = COVERED;
        }
    }
    return board;
}

void print_board(Board b){
    // This function prints the current board
    int i, j;
    for (i = b.rows - 1; i >= 0; i --) {
        printf("%d ", i);
        for (j = 0; j < b.cols; j ++) {
            if (b.tiles[i][j].visibility == COVERED)
                printf("# ");
            else if (b.tiles[i][j].visibility == QUESTIONED)
                printf("? ");
            else if (b.tiles[i][j].visibility == MARKED)
                printf("! ");
            else if (b.tiles[i][j].visibility == REVEALED) {
                if (b.tiles[i][j].isMine){
                    printf("* ");
                }
                else
                    printf("%d ", b.tiles[i][j].surroundingMines);
            }
            else
                printf("X ");
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < b.cols; j ++){
        printf("%d ", j);
    }
    printf("\n");
    return;
}

void place_mines(Board* b_ptr) {
    // This function places the desired number of mines on the board
    
    srand(b_ptr->randSeed);
    int mineRow, mineCol;
    int minesPrinted = 0;
    while (minesPrinted < b_ptr->numMines) {
        mineRow = (rand() % b_ptr->rows);
        mineCol = (rand() % b_ptr->cols);
        if (!(b_ptr->tiles[mineRow][mineCol].isMine)){
            printf("Placing mine at %d, %d\n", mineRow, mineCol);
            b_ptr->tiles[mineRow][mineCol].isMine = 1;
            minesPrinted ++;
        }
    }
}

void count_mines(Board* b){
    // This function counts the number of surrounding mines for each individual tile
    
    int i, j, k, l;
    int mine_count = 0;
    for(k = 0; k < b->rows; k++){
        for(l = 0; l < b->cols; l++){
            if(k > 0 && k < b->rows - 1 && l > 0 && l < b->cols - 1){
                for(i = k - 1; i <= k + 1; i++ ){
                    for(j = l - 1; j <= l + 1; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == 0 && l > 0 && l < b->cols - 1){
                for(i = k; i <= k + 1; i++ ){
                    for(j = l - 1; j <= l + 1; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == b->rows - 1 && l > 0 && l < b->cols - 1){
                for(i = k - 1; i <= k; i++ ){
                    for(j = l - 1; j <= l + 1; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k > 0 && k < b->rows - 1  && l == 0){
                for(i = k - 1; i <= k + 1; i++ ){
                    for(j = l; j <= l + 1; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k > 0 && k < b->rows - 1 && l == b->cols - 1){
                for(i = k - 1; i <= k + 1; i++ ){
                    for(j = l - 1; j <= l; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == 0 && l == 0){
                if (b->rows != 1){
                    for(i = 0; i < 2; i++ ){
                        for(j = 0; j < 2; j++){
                            if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                                ++mine_count;
                                b->tiles[k][l].surroundingMines = mine_count;
                            }
                        }
                    }
                }
                else {
                    for(i = 0; i < 1; i++ ){
                        for(j = 0; j < 2; j++){
                            if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                                ++mine_count;
                                b->tiles[k][l].surroundingMines = mine_count;
                            }
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == 0 && l == b->cols - 1){
                if (b->rows != 1){
                    for(i = k; i <= k + 1; i++ ){
                        for(j = l - 1; j <= l; j++){
                            if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                                ++mine_count;
                                b->tiles[k][l].surroundingMines = mine_count;
                            }
                        }
                    }
                }
                else {
                    for(i = k; i <= k; i++ ){
                        for(j = l - 1; j <= l; j++){
                            if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                                ++mine_count;
                                b->tiles[k][l].surroundingMines = mine_count;
                            }
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == b->rows - 1 && l == 0){
                for(i = k - 1; i <= k; i++ ){
                    for(j = l; j <= l + 1; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
            else if(k == b->rows - 1 && l == b->cols - 1){
                for(i = k - 1; i <= k; i++ ){
                    for(j = l - 1; j <= l; j++){
                        if(b->tiles[i][j].isMine && !b->tiles[k][l].isMine){
                            ++mine_count;
                            b->tiles[k][l].surroundingMines = mine_count;
                        }
                    }
                }
                mine_count = 0;
            }
        }
    }
    return;
}

int count_marks(Board b) {
    // This function counts the number of marked tiles on the board
    
    int i, j;
    int markCounter = 0;
    for (i = 0; i < b.rows; i++) {
        for (j = 0; j < b.cols; j++) {
            if (b.tiles[i][j].visibility == MARKED) {
                markCounter++;
            }
        }
    }
    return markCounter;
}

void get_play(Board* b) {
    // This function prompts the user to enter a row and column to act upon
    
    // Loop until a valid coordinate is entered
    while (1) {
        printf("Enter row a row between 0-%d and a column between 0-%d: ", (b->rows - 1), (b->cols - 1));
        scanf("%d %d", &(b->userRow), &(b->userCol));
        
        if (b->userRow > (b->rows - 1) || b->userRow < 0) {
            continue;
        }
        else if (b->userCol > (b->cols - 1) || b->userCol < 0) {
            continue;
        }
        else if (b->tiles[b->userRow][b->userCol].visibility == REVEALED) {
            printf("This tile is already revealed.\n");
            continue;
        }
        else {
            break;
        }
    }
    return;
}

void enter_action(Board* b_ptr){
    // This function prompts the user to enter an action to apply to the selected tile
    
    int userNum;
    if (b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility == COVERED) {
        printf("Enter Action\n");
        printf("0. Reveal\n");
        printf("1. Question\n");
        printf("2. Mark\n");
        printf("3. Cancel\n");
        printf("Action: ");
        scanf("%d", &userNum);
        if (userNum == 0 && !b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].isMine) {
            reveal_tiles(b_ptr, b_ptr->userRow, b_ptr->userCol);
        }
        else if (userNum == 0 && b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].isMine) {
            game_over(b_ptr);
        }
        else if (userNum == 1) {
            b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility = QUESTIONED;
        }
        else if (userNum == 2) {
            b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility = MARKED;
        }
        else {
            b_ptr->cancel = 1;
        }
    }
    else if (b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility == QUESTIONED) {
        printf("Enter Action\n");
        printf("0. UnQuestion\n");
        printf("1. Cancel\n");
        printf("Action: ");
        scanf("%d", &userNum);
        if (userNum == 0){
            b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility = COVERED;
        }
        else {
            b_ptr->cancel = 1;
        }
    }
    else if (b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility == MARKED) {
        printf("Enter Action\n");
        printf("0. UnMark\n");
        printf("1. Cancel\n");
        printf("Action: ");
        scanf("%d", &userNum);
        if (userNum == 0){
            b_ptr->tiles[b_ptr->userRow][b_ptr->userCol].visibility = COVERED;
        }
        else {
            b_ptr->cancel = 1;
        }
    }
    else {
        return;
    }
}

void reveal_tiles(Board* b_ptr, int curRow, int curCol){
    // This function uses recursion to reveal tiles
    
    if (b_ptr->tiles[curRow][curCol].isMine)
        return;
    else if (b_ptr->tiles[curRow][curCol].visibility == REVEALED ||
             b_ptr->tiles[curRow][curCol].visibility == MARKED)
        return;
    else if (b_ptr->tiles[curRow][curCol].surroundingMines != 0){
        b_ptr->tiles[curRow][curCol].visibility = REVEALED;
        return;
    }
    else if (b_ptr->tiles[curRow][curCol].visibility == COVERED) {
        b_ptr->tiles[curRow][curCol].visibility = REVEALED;
        if (curRow != b_ptr->rows - 1){
            reveal_tiles(b_ptr, curRow + 1, curCol);
        }
        if (curCol != b_ptr->cols - 1){
            reveal_tiles(b_ptr, curRow, curCol + 1);
        }
        if (curRow != 0){
            reveal_tiles(b_ptr, curRow - 1, curCol);
        }
        if (curCol != 0){
            reveal_tiles(b_ptr, curRow, curCol - 1);
        }
        if(curRow != 0 && curCol != 0){
            reveal_tiles(b_ptr, curRow - 1, curCol - 1);
        }
        if(curRow != b_ptr->rows - 1 && curCol != 0){
            reveal_tiles(b_ptr, curRow + 1, curCol - 1);
        }
        if(curRow != b_ptr->rows - 1 && curCol != b_ptr->cols - 1){
            reveal_tiles(b_ptr, curRow + 1, curCol + 1);
        }
        if(curRow != 0 && curCol != b_ptr->cols - 1){
            reveal_tiles(b_ptr, curRow - 1, curCol + 1);
        }
        return;
    }
}

int game_won(Board b){
    // This function checks if the game has been won
    
    int i, j;
    int mark_count = 0;
    for(i = 0; i < b.rows; i ++){
        for(j = 0; j < b.cols; j++){
            if(b.tiles[i][j].visibility == MARKED){
                mark_count++;
            }
            if(b.tiles[i][j].visibility == COVERED ||
               b.tiles[i][j].visibility == QUESTIONED)
                return 0;
        }
    }
    if(mark_count == b.numMines)
        return 1;
    else
        return 0;
}

void player_won(Board* b_ptr){
    // This function executes the appropriate actions if the user has won
    
    int i, j;
    for (i = 0; i < b_ptr->rows; i++) {
        for (j = 0; j < b_ptr->cols; j++) {
            b_ptr->tiles[i][j].visibility = REVEALED;
        }
    }
    print_board(*b_ptr);
    printf("You Won!!\n");
    exit(0);
}

void game_over(Board* b) {
    // This function executes the appropriate actions if the user has lost
    
    int i, j;
    for (i = 0; i < b->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            b->tiles[i][j].visibility = REVEALED;
        }
    }
    print_board(*b);
    printf("You Lost :(\n");
    exit(0);
}

void play_game(Board* b_ptr){
    // This is the main function to play the game
    
    place_mines(b_ptr);
    count_mines(b_ptr);
    printf("There are %d mines left\n", b_ptr->numMines - count_marks(*b_ptr));
    print_board(*b_ptr);
    while (!game_won(*b_ptr)) {
        get_play(b_ptr);
        enter_action(b_ptr);
        if (b_ptr->cancel){
            b_ptr->cancel = 0;
            continue;
        }
        if (game_won(*b_ptr)){
            player_won(b_ptr);
        }
        printf("There are %d mines left\n", b_ptr->numMines - count_marks(*b_ptr));
        print_board(*b_ptr);
    }
}

void destroy_board(Board* b_ptr){
    // This function frees the space allocated for the program
    
    int i;
    for (i = 0; i < b_ptr->rows; i ++){
        free(b_ptr->tiles[i]);
    }
    free(b_ptr->tiles);
    return;
}