#include "Panel.h"

typedef struct{
    Panel* panel_mat;
    int size_y;
    int size_x;
    int field_size_y;
    int field_size_x;
} GameBoard;

void init(GameBoard* gb, int y, int x, int bomb_num);
void mine_sweeper_cli(int y, int x, int bomb_num);