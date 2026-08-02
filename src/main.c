#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "logic.h"
#include "parser.h"

#define GUESSES_FILENAME "src/guesses.csv"
#define STDIN_BUFFER_SIZE 64


struct GuessInfo get_blank_guess_info(void) {
    struct GuessInfo info = {0};

    info.possible_types = ALL_TYPES;
    info.is_type_correct = false;

    info.possible_locations = ALL_LOCATIONS;
    info.is_location_correct = false;

    info.possible_colors = ALL_COLORS;
    info.is_color_correct = false;

    info.min_health = -1;
    info.max_health = INT_MAX;
    info.has_health = true;
    info.is_health_correct = false;

    info.min_kill_count = -1;
    info.max_kill_count = INT_MAX;
    info.has_kill_count = true;
    info.is_kill_count_correct = false;

    return info;
}

int main() {
    struct GuessListNode *guess_list = parse_file(GUESSES_FILENAME);
    struct GuessInfo info = get_blank_guess_info();

    struct Guess best_guess;
    char guess_result_string[STDIN_BUFFER_SIZE];

    // loop until the length of guess_list is either 0 or 1
    while (guess_list != NULL && guess_list->next != NULL) {
        best_guess = calculate_best_guess(info, guess_list);
        printf("Best guess: %s.\n", best_guess.name);

        fgets(guess_result_string, sizeof(guess_result_string), stdin);

        struct GuessResult result = parse_guess(guess_result_string, best_guess);
        modify_guess_info(&info, result);
        cull_search_space(&guess_list, info);
    }

    if (guess_list != NULL) {
        printf("Answer: %s.\n", guess_list->guess.name);

    } else {
        printf("No such answer found.\n");
    }

    free_guess_list(guess_list);
}

