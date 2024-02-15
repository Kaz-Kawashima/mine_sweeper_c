#include <stdbool.h>

typedef struct {
    char display;
    unsigned char bomb_value;
    bool is_open;
    bool is_flagged;
    bool is_bomb;
} Panel;

void new_blank_panel(Panel* p);
void new_bomb_panel(Panel* p);
void new_border_panel(Panel* p);
void flag(Panel* p);
bool open(Panel* p);

