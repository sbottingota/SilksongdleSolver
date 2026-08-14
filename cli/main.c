#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "logic.h"
#include "parser.h"

#define GUESSES_FILENAME "core/guesses.csv"
#define STDIN_BUFFER_SIZE 64

// #define DEBUG_INFO // enable, either through code or through the compiler, for debug info to be printed


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
    struct GuessListNode *remaining_guesses = copy_guess_list(guess_list);

    struct GuessInfo info = get_blank_guess_info();

    struct Guess best_guess;
    char guess_result_string[STDIN_BUFFER_SIZE];

    // loop until the length of guess_list is either 0 or 1
    while (remaining_guesses != NULL && remaining_guesses->next != NULL) {
        best_guess = calculate_best_guess(info, remaining_guesses, guess_list);

        printf("Best guess: %s. (Expected information: %lf)\n",
            best_guess.name, calculate_expected_entropy(best_guess, info, remaining_guesses));

        fgets(guess_result_string, sizeof(guess_result_string), stdin);

        struct GuessResult result = parse_guess(guess_result_string, best_guess);
        modify_guess_info(&info, result);

        cull_search_space(&remaining_guesses, info);

        #ifdef DEBUG_INFO
        printf("Remaining guesses: ");
        print_guesses(remaining_guesses);
        printf("\n\n");

        print_guess_info(&info);
        #endif
    }

    if (remaining_guesses != NULL) {
        printf("Answer: %s.\n", remaining_guesses->guess.name);

    } else {
        printf("No such answer found.\n");
    }

    free_guess_list(guess_list);
    free_guess_list(remaining_guesses);
}

