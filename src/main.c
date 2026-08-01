#include <stdio.h>
#include <string.h>

#include "logic.h"

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

    struct GuessInfo info = get_blank_guess_info();

    printf("Init complete!\n");
}

