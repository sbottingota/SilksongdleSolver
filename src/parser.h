#ifndef PARSER_H
#define PARSER_H

#include "definitions.h"

#define BUFFER_SIZE 128
#define N_LINE_TOKENS 6

struct GuessListNode {
    struct Guess guess;
    struct GuessListNode *next;
};

struct GuessListNode *parse_file(char *filename);

void free_guess_list(struct GuessListNode *head);

#endif
