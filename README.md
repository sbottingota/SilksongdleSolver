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

<img width="899" height="522" alt="image" src="https://github.com/user-attachments/assets/17b6654c-6294-42cc-9e48-295043f82dd4" />
