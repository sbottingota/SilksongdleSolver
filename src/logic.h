#ifndef LOGIC_H
#define LOGIC_H

#include "definitions.h"

void modify_guess_info(struct GuessInfo*, struct GuessResult);
struct GuessResult parse_guess(const char*, struct Guess);
void add_combo(uint64_t combo, uint64_t *combo_array);
struct GuessInfo get_blank_guess_info(void);

#endif
