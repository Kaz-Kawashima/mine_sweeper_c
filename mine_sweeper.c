#include "mine_sweeper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define BUFF_LENGTH 16

Panel* get_panel(GameBoard* gb, int row, int col) {
    Panel* pos = gb->panel_mat;
    pos += row * gb->field_size_x + col;
    return pos;
}

int rand_n(int min, int max) {;
    return rand() % max + min;
}

void set_bomb(GameBoard* gb, int bomb_num){
    int counter = 0;
    while (counter < bomb_num){
        int row = rand_n(1, gb->size_y);
        int col = rand_n(1, gb->size_x);
        Panel* p = get_panel(gb, row, col);
        if (!p->is_bomb) {
            new_bomb_panel(p);
            counter++;
        }
    } 
}

void calc_panel_bomb_value(GameBoard* gb, int y, int x){
    unsigned char counter = 0;
    for(int row = y-1; row <= (y + 1); row++){
        for(int col = x-1; col <= (x + 1); col++){
            Panel* p = get_panel(gb, col, row);
            if (p->is_bomb) {
                counter++;
            }
        }
    }
    Panel* current_panel = get_panel(gb, y, x);
    current_panel->bomb_value = counter;
}

void calc_gb_bomb_value(GameBoard* gb){
    for(int row = 0; row <= gb->size_y; row++){
        for(int col = 0; col <= gb->size_x; col++){
            calc_panel_bomb_value(gb, col, row);
        }
    } 
}

unsigned char calc_bomb_value(GameBoard* gb, int y, int x) {
    unsigned char counter = 0;
    for (int row = y - 1; row <= (y + 1); row++) {
        for (int col = x - 1; col <= (x + 1); col++) {
            Panel* p = get_panel(gb, row, col);
            if (p->is_bomb) {
                counter++;
            }
        }
    }
    return counter;
}

void calc_bomb_value_all(GameBoard* gb) {
    for (int row = 1; row <= gb->size_y; row++) {
        for (int col = 1; col <= gb->size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            if (!p->is_bomb) {
                p->bomb_value = calc_bomb_value(gb, row, col);
            }
        }
    }
}

void init(GameBoard* gb, int y, int x, int bomb_num){
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
    int mat_size = field_size_y * field_size_x;
    gb->panel_mat = malloc(sizeof(Panel)* mat_size);
    //Fill Panel
    for (int row = 1; row <= y; row++){
        for (int col = 1; col <= x; col++){
            p = get_panel(gb, col, row);
            new_blank_panel(p);
        }
    }
    //Set Bomb
    set_bomb(gb, bomb_num);
    //Fill Boarder
    for (int row = 0; row < field_size_y; row++){
        p = get_panel(gb, row, 0);
        new_boarder_panel(p);
        p = get_panel(gb, row, field_size_x -1);
        new_boarder_panel(p);
    }
    for (int col = 0; col < field_size_x; col++){
        p = get_panel(gb, 0, col);
        new_boarder_panel(p);
        p = get_panel(gb, field_size_y - 1, col);
        new_boarder_panel(p);
    }
    calc_bomb_value_all(gb);
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
    if (!p->is_open && !p->is_flagged) {
        pos = (gb->field_size_x * 2 + 1) * gb->cursor_row + gb->cursor_col * 2;
        buff[pos] = '*';
    }
    printf(buff);
    free(buff);
}


bool key_input(GameBoard* gb) {

    printf("\ninput <- ^v -> / O open / F flag\n");

    bool finished = false;
    bool safe = true;
    Panel* p = 0;

    do{
        finished = true;
        char input = _getch();
        if (input == 0x00 || input == 0xe0) {
            input = _getch();
        }
        switch (input) {
            case(0x4b):
                gb->cursor_col--;
                break;
            case(0x48):
                gb->cursor_row--;
                break;
            case(0x50):
                gb->cursor_row++;
                break;
            case(0x4d):
                gb->cursor_col++;
                break;
            case('O'):
            case('o'):
                p = get_panel(gb, gb->cursor_row, gb->cursor_col);
                safe = open(p);
                break;
            case('F'):
            case('f'):
                p = get_panel(gb, gb->cursor_row, gb->cursor_col);
                flag(p);
                break;
            default:
                finished = false;
        }
        if (gb->cursor_row < 1) {
            gb->cursor_row = 1;
        }
        if (gb->cursor_row > gb->size_y) {
            gb->cursor_row = gb->size_y;
        }
        if (gb->cursor_col < 1) {
            gb->cursor_col = 1;
        }
        if (gb->cursor_col > gb->size_x) {
            gb->cursor_col = gb->size_x;
        }
    } while (!finished);

    return safe;
}



int open_around(GameBoard* gb, int y, int x) {
    int counter = 0;
    for(int row = y - 1; row <= (y + 1); row++){
        for (int col = x - 1; col <= (x + 1); col++) {
            Panel* p = get_panel(gb, row, col);
            if (!p->is_open) {
                open(p);
                counter++;
            }
        }
    }
    return counter;
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

bool is_finished(GameBoard* gb) {
    for (int row = 1; row <= gb->size_y; row++) {
        for (int col = 1; col <= gb->size_x; col++) {
            Panel* p = get_panel(gb, row, col);
            if (!p->is_open && !p->is_bomb) {
                return false;
            }
        }
    }
    return true;
}

void bomb_open(GameBoard* g) {
    return;
}

void mine_sweeper_cli(int y, int x, int bomb_num) {
    bool finished = false;
    int row, col;
    GameBoard game_board;
    GameBoard* gb = &game_board;

    init(gb, y, x, bomb_num);
    while (true) {
        print_gb(gb);
        bool safe = key_input(gb);
        if(safe){
            cascade_open(gb);
            finished = is_finished(gb);
            if (finished) {
                print_gb(gb);
                printf("You Win!\n\nhit any key...");
                _getch();
                return;
            }
        }
        else {
            bomb_open(gb);
            print_gb(gb);
            printf("Game Over!\n\nhit any key...");
            _getch();
            return;
        }
    }
}