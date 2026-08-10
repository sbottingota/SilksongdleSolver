Attempted [silksongdle](https://silksongdle.com/classic) solver using information theory (inspired by [this](https://youtu.be/v68zYyaEmEA) 3b1b video).

## Compilation
Compile with `make` and then run with `build/main`.

Or if that doesn't work (haven't tested on other devices), just compile it manually with:
```
cc --std=c23 src/*.c -o [output file] -lm
```
And then run that file.

To make the program print debug info, compile with `make debug` and run the same way.

Or again, compile it manually with:
```
cc --std=c23 src/*.c -o [output file] -lm -DDEBUG_INFO
```

(N.B.: the `--std=c23` is because my bitfied enums use `'` number separators, which require a modern version of C. If this is a problem, you can always manually delete the quotes from `src/definitions.h` ¯\\\_(ツ)\_/¯.)

## Usage
For non-numerical fields (e.g. location), simply put `r` if the guess gives you a red square, `o` for an orange square, and `g` for a green square.
For numeric fields (e.g. health), put `h` if the game indicates that the answer is higher than your guess, and `l` if it is lower.
Only put `r` if no arrow appears (either the answer has n/a in that field, or your guess does), and `g` is naturally for a green square.

**Ignore the character column**, as this provides no useful information other than whether your guess was the answer or not.

|Letter|Visuals|Usage|
|---|---|---|
|g | Green square|correct|
|o | Orange square **with no arrow**|Partially correct|
|r| Red square **with no arrow**| Incorrect|
|h| Up arrow (i.e. higher) | Answer is higher than guess|
|l| Down arrow (i.e. lower) | Answer is lower than guess|

(Yes I know this sucks.)

----

Example (from the 10th of August 2026):
```
Best guess: pilgrim hulk. (Expected information: 4.970599)
goolh
Best guess: aknid. (Expected information: 3.000000)
grrgl
Answer: lavalug.
```

<img width="886" height="348" alt="image" src="https://github.com/user-attachments/assets/feaab784-1ce7-4fb5-b480-b2dfff7bd139" />

