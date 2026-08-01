#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>

enum Type {
    BOSS = 0x1,
    ENEMY = 0x2,
    MERCHANT = 0x4,
    NPC = 0x8,
    PLAYER = 0x10,

    ALL_TYPES = 0x1F
};

enum Color {
    QUESTION_MARK = 0x1,
    BEIGE = 0x2,
    BLACK = 0x4,
    BLUE = 0x8,
    BROWN = 0x10,
    GOLD = 0x20,
    GRAY = 0x40,
    GREEN = 0x80,
    ORANGE = 0x100,
    PINK = 0x200,
    PURPLE = 0x400,
    RED = 0x800,
    WHITE = 0x1000,
    YELLOW = 0x2000,
    
    ALL_COLORS = 0x3FFF
};

struct Guess {
    char name[16];

    // bitfields
    uint8_t type;
    uint64_t location;
    uint16_t color;

    // actual numbers; use -1 for n/a
    int health;
    int kill_count;
};

struct Guess all_guesses[100];

void init_all_guesses(void) {
    strcpy(all_guesses[0].name, "aknid");
    all_guesses[0].type = ENEMY;
    all_guesses[0].location = 0;
    all_guesses[0].color = WHITE | GREEN;
    all_guesses[0].health = 15;
    all_guesses[0].kill_count = 15;
    
    strcpy(all_guesses[1].name, "alchemist zylotol");
    all_guesses[1].type = NPC;
    all_guesses[1].location = 0;
    all_guesses[1].color = WHITE | BLUE;
    all_guesses[1].health = -1;
    all_guesses[1].kill_count = -1;

    strcpy(all_guesses[2].name, "alita");
    all_guesses[2].type = ENEMY;
    all_guesses[2].location = 0;
    all_guesses[2].color = RED | BLACK;
    all_guesses[2].health = 80;
    all_guesses[2].kill_count = 6;
}

int main() {
    init_all_guesses();

    uint8_t type = ALL_TYPES;
    uint64_t location = -1;
    uint16_t color = ALL_COLORS;

    int min_health = -1;
    int max_health = INT_MAX;

    int min_kill_count = -1;
    int max_kill_count = INT_MAX;

    printf("Init complete!\n");
}

