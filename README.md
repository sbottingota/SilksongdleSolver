Attempted [silksongdle](https://silksongdle.com/classic) solver using information theory (inspired by [this](https://youtu.be/v68zYyaEmEA) 3b1b video).

Logic written in C, and GUI written in C++ using [Dear ImGui](https://github.com/ocornut/imgui/).

## Compilation
```
cmake -B build
cmake --build build
```

And then run with:
```
build/main
```

To make the program print debug info, instead write `cmake -B build -DDEBUG_INFO=ON` and then proceed normally.

## Dependencies
They exist, but I can't be bothered to list them right now (TODO: do this later).

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

## GUI
Currently disfunctional: ignore.

