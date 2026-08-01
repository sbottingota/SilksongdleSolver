#include <stdio.h>
#include <string.h>

#include "logic.h"
#include "parser.h"

#define GUESSES_FILENAME "src/guesses.csv"

int main() {
    struct GuessListNode *guess_list = parse_file(GUESSES_FILENAME);

    struct GuessInfo info = get_blank_guess_info();

    printf("Init complete!\n");

    printf("%s: 0x%lx 0x%lx 0x%lx %d %d\n", guess_list->guess.name, guess_list->guess.type, guess_list->guess.location, guess_list->guess.color, guess_list->guess.health, guess_list->guess.kill_count);

    free_guess_list(guess_list);
}

