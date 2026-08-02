Attempted [silksongdle](https://silksongdle.com/classic) solver using information theory (inspired by [this](https://youtu.be/v68zYyaEmEA) 3b1b video).

## Compilation
Compile with `make` and then run with `build/main`.

Or if that doesn't work (haven't tested on other devices), just compile it manually with:
```
cc --std=c23 src/*.c -o [output file] -lm
```
And then run that file.

## Usage
For non-numerical fields (e.g. location), simply put `r` if the guess gives you a red square, `o` for an orange square, and `g` for a green square.
For numeric fields (e.g. health), put `h` if the game indicates that the answer is higher than your guess, and `l` if it is lower.
Only put `r` if no arrow appears (either the answer has n/a in that field, or your guess does), and `g` is naturally for a green square.

Example (from the 2nd of August 2026):
```
Best guess: undersweep. (Expected information: 5.443569)
grrhg
Best guess: cloverstag. (Expected information: 2.349163)
grrlg
Best guess: mnemonid. (Expected information: 1.584963)
grolg
Answer: wraith.
```

