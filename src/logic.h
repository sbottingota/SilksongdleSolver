#ifndef LOGIC_H
#define LOGIC_H

#include "definitions.h"
#include "parser.h"

void modify_guess_info(struct GuessInfo *info, struct GuessResult result);
struct Guess calculate_best_guess(struct GuessInfo info, struct GuessListNode *search_space);
void cull_search_space(struct GuessListNode **search_space, struct GuessInfo info);

bool is_guess_valid(struct Guess guess, struct GuessInfo guess_info);

#endif
