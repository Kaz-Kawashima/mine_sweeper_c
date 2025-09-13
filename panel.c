#include "panel.h"

void new_blank_panel(Panel* p){
    p->display = '#';
    p->bomb_value = 0;
    p->is_open = false;
    p->is_bomb = false;
    p->is_flagged = false;
}

void new_bomb_panel(Panel* p){
    p->display = '#';
    p->bomb_value = 0;
    p->is_open = false;
    p->is_bomb = true;
    p->is_flagged = false;
}

void new_border_panel(Panel* p){
    p->display = '=';
    p->bomb_value = 0;
    p->is_open = true;
    p->is_bomb = false;
    p->is_flagged = false;
}

void flag_panel(Panel* p){
    if (!p->is_open) {
        if (p->is_flagged) {
            p->is_flagged = false;
            p->display = '#';
        }
        else {
            p->is_flagged = true;
            p->display = 'F';
        }
    }
}

bool open_panel(Panel* p){
    if (p->is_flagged){
        return true;
    }
    if (p->is_bomb){
        p->is_open = true;
        p->display = 'B';
        return false;
    } else {
        if (!p->is_open) {
            p->is_open = true;
            unsigned char bomb_value = p-> bomb_value;
            if (bomb_value > 0) {
                p->display = itoc(bomb_value);
            } else {
                p->display = ' ';
            }
        }
        return true;
    }
}