#include "mine_sweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

Panel* get_panel(GameBoard* gb, int row, int col) {
    Panel* pos = gb->panel_mat;
    pos += row * gb->field_size_x + col;
    return pos;
}

int rand_n(int min, int max) {
    return rand() % max + min;
}

void calc_panel_bomb_value(GameBoard * gb, int y, int x) {
    unsigned char counter = 0;
    for (int row = y - 1; row <= (y + 1); row++) {
        for (int col = x - 1; col <= (x + 1); col++) {
            Panel* p = get_panel(gb, row, col);
            if (p->is_bomb) {
                counter++;
            }
        }
    }
    Panel* current_panel = get_panel(gb, y, x);
    current_panel->bomb_value = counter;
}

void calc_bomb_value(GameBoard* gb) {
    for (int row = 1; row <= gb->size_y; row++) {
        for (int col = 1; col <= gb->size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            if (!p->is_bomb) {
                calc_panel_bomb_value(gb, row, col);
            }
        }
    }
}

void set_bomb(GameBoard* gb){
	int num_bomb = gb->num_bomb;    
    int counter = 0;
    while (counter < num_bomb){
        int row = rand_n(1, gb->size_y);
        int col = rand_n(1, gb->size_x);
        if (row == gb->cursor_row && col == gb->cursor_col) {
            continue;
        }
        Panel* p = get_panel(gb, row, col);
        if (!p->is_bomb) {
            new_bomb_panel(p);
            counter++;
        }
    }
	calc_bomb_value(gb);
	gb->state = Playing;
}

void init(GameBoard* gb, int y, int x, int num_bomb){
    Panel* p;
    //init random generator
    srand((unsigned int)time(NULL));
    //init cursor
    gb->cursor_col = 1;
    gb->cursor_row = 1;
    //init game board
    gb->size_y = y;
    gb->size_x = x;
    int field_size_y = y + 2;
    int field_size_x = x + 2;
    gb->field_size_y = field_size_y;
    gb->field_size_x = field_size_x;
	gb->num_bomb = num_bomb;
    int mat_size = field_size_y * field_size_x;
    gb->panel_mat = malloc(sizeof(Panel)* mat_size);
    //Fill Panel
    for (int row = 1; row <= y; row++){
        for (int col = 1; col <= x; col++){
            p = get_panel(gb, col, row);
            new_blank_panel(p);
        }
    }
    //Fill Border
    for (int row = 0; row < field_size_y; row++){
        p = get_panel(gb, row, 0);
        new_border_panel(p);
        p = get_panel(gb, row, field_size_x -1);
        new_border_panel(p);
    }
    for (int col = 0; col < field_size_x; col++){
        p = get_panel(gb, 0, col);
        new_border_panel(p);
        p = get_panel(gb, field_size_y - 1, col);
        new_border_panel(p);
    }
	gb->state = Uninitialized;
}

void del(GameBoard* gb) {
    free(gb->panel_mat);
}

GameState get_state(GameBoard* gb) {
    if (gb->state == Uninitialized){
        return gb->state;
    }
    gb->state = Win;
	for (int row = 1; row <= gb->size_y; row++) {
		for (int col = 1; col <= gb->size_x; col++) {
			Panel* p = get_panel(gb, row, col);
			if (!p->is_open && !p->is_bomb) {
				gb->state = Playing;
			}
            if (p->is_open && p->is_bomb) {
                gb->state = Lose;
                return gb->state;
            }
		}
	}
	return gb->state;
}

void cascade_open(GameBoard* gb) {
    int new_open = 0;
    do {
        new_open = 0;
        for (int row = 1; row <= gb->size_y; row++) {
            for (int col = 1; col <= gb->size_x; col++) {
                Panel* p = get_panel(gb, row, col);
                if (p->is_open && p->bomb_value == 0) {
                    new_open += open_around(gb, row, col);
                }
            }
        }
    } while (new_open > 0);
    return;
}

GameState open(GameBoard* gb) {
    if (gb->state == Uninitialized) {
        set_bomb(gb);
        calc_bomb_value(gb);
        gb->state = Playing;
    }
    Panel* p = get_panel(gb, gb->cursor_row, gb->cursor_col);
    int ret = open_panel(p);
    if (ret == true) {
		cascade_open(gb);
    }
	return get_state(gb);
 }

void print_gb(GameBoard* gb) {
    int buff_size = (gb->field_size_x * 2 + 1) * gb->field_size_y + 1;
    char* buff = malloc(sizeof(char) * buff_size);
    int pos = 0;
    //print game board
    for(int row = 0; row < gb->field_size_y; row++){
        for(int col = 0; col < gb->field_size_x; col++){
            Panel* p = get_panel(gb, row, col);
            char display = p->display;
            buff[pos] = display;
            pos++;
            buff[pos] = ' ';
            pos++;
        }
        buff[pos] = '\n';
        pos++;
    }
    buff[pos] = 0;
    //print_cursor_row;
    pos = (gb->field_size_x * 2 + 1) * gb->cursor_row;
    buff[pos] = '>';
    pos += (gb->field_size_x * 2 - 2);
    buff[pos] = '<';
    //print_cursor_row;
    pos = gb->cursor_col * 2;
    buff[pos] = 'v';
    pos += (gb->field_size_x * 2 + 1) * (gb->field_size_y - 1);
    buff[pos] = '^';
    system("cls");    //windows
    //system("clear"); //linux
    //princ_cursor
    Panel* p = get_panel(gb, gb->cursor_row, gb->cursor_col);
    pos = (gb->field_size_x * 2 + 1) * gb->cursor_row + gb->cursor_col * 2;
    if (!p->is_open && !p->is_flagged) {
        buff[pos] = '@';
    }
    else if (p->is_open && !p->is_bomb) {
        buff[pos] = '_';
    }
    printf(buff);
    free(buff);
    int flag_count = count_flag(gb);
    printf("\ninput <- ^v -> / O open / F flag (%d)\n", flag_count);
}

void print_gb_debug(GameBoard* gb) {
	printf("\n");
    int buff_size = (gb->field_size_x * 2 + 1) * gb->field_size_y + 1;
    char* buff = malloc(sizeof(char) * buff_size);
    int pos = 0;
    //print game board
    for (int row = 0; row < gb->field_size_y; row++) {
        for (int col = 0; col < gb->field_size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            char display = p->display;
            if (display != '=') {
                if (p->is_bomb) {
                    display = 'B';
                }
                else {
                    display = itoc(p->bomb_value);
                }
            }
            buff[pos] = display;
            pos++;
            buff[pos] = ' ';
            pos++;
        }
        buff[pos] = '\n';
        pos++;
    }
    buff[pos] = 0;
    //print_cursor_row;
    pos = (gb->field_size_x * 2 + 1) * gb->cursor_row;
    buff[pos] = '>';
    pos += (gb->field_size_x * 2 - 2);
    buff[pos] = '<';
    //print_cursor_row;
    pos = gb->cursor_col * 2;
    buff[pos] = 'v';
    pos += (gb->field_size_x * 2 + 1) * (gb->field_size_y - 1);
    buff[pos] = '^';
    //system("cls");    //windows
    //system("clear"); //linux
    //princ_cursor
    Panel* p = get_panel(gb, gb->cursor_row, gb->cursor_col);
    pos = (gb->field_size_x * 2 + 1) * gb->cursor_row + gb->cursor_col * 2;
    if (!p->is_open && !p->is_flagged) {
        buff[pos] = '@';
    }
    else if (p->is_open && !p->is_bomb) {
        buff[pos] = '_';
    }
    printf(buff);
    free(buff);
}




int count_flag(GameBoard* gb) {
    int counter = 0;
    for (int row = 1; row <= gb->size_y; row++) {
        for (int col = 1; col <= gb->size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            if (p->is_flagged) {
                counter++;
            }
        }
    }
    return counter;
}


GameState key_input(GameBoard* gb) {

    Panel* p = 0;
	GameState state = Uninitialized;
    do{
        print_gb(gb);
		//print_gb_debug(gb);
        char input = _getch();
        if (input == 0x00 || input == 0xe0) {
            input = _getch();
        }
        switch (input) {
            case(0x4b):
                gb->cursor_col--;
                if (gb->cursor_col< 1) {
                    gb->cursor_col = 1;
                }
                break;
            case(0x48):
                gb->cursor_row--;
                if (gb->cursor_row < 1) {
                    gb->cursor_row = 1;
                }
                break;
            case(0x50):
                gb->cursor_row++;
                if (gb->cursor_row > gb->size_y) {
                    gb->cursor_row = gb->size_y;
                }
                break;
            case(0x4d):
                gb->cursor_col++;
                if (gb->cursor_col > gb->size_x) {
                    gb->cursor_col = gb->size_x;
                }
                break;
            case('O'):
            case('o'):
				state = open(gb);
                break;
            case('F'):
            case('f'):
                p = get_panel(gb, gb->cursor_row, gb->cursor_col);
                flag_panel(p);
                break;
			case('Q'):
			case('q'):
                exit(0);
        }
    } while (state == Playing || state == Uninitialized);
    return state;
}



int open_around(GameBoard* gb, int y, int x) {
    int counter = 0;
    for(int row = y - 1; row <= (y + 1); row++){
        for (int col = x - 1; col <= (x + 1); col++) {
            Panel* p = get_panel(gb, row, col);
            if (!p->is_open) {
                open_panel(p);
                counter++;
            }
        }
    }
    return counter;
}

void bomb_open(GameBoard* gb) {
    for (int row = 1; row <= gb->size_y; row++) {
        for (int col = 1; col <= gb->size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            if (p->is_bomb) {
                open_panel(p);
            }
        }
    }
    return;
}

void mine_sweeper_cli(int y, int x, int bomb_num) {
    GameBoard game_board;
    GameBoard* gb = &game_board;

    init(gb, y, x, bomb_num);
    GameState state = key_input(gb);
    if(state == Win){
		print_gb(gb);
        printf("You Win!\n\nhit any key...");
        _getch();
    } else {
        bomb_open(gb);
        print_gb(gb);
        printf("Game Over!\n\nhit any key...");
        _getch();
    }
    del(gb);
}