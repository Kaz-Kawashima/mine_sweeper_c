#include "Panel.h"

typedef enum {
    Uninitialized,
    Playing,
    Win,
    Lose
} GameState;

typedef struct{
    Panel* panel_mat;
    int size_y;
    int size_x;
    int field_size_y;
    int field_size_x;
    int cursor_row;
    int cursor_col;
	int num_bomb;
	GameState state;
} GameBoard;

void init(GameBoard* gb, int y, int x, int num_bomb);
void mine_sweeper_cli(int y, int x, int num_bomb);