# C++ Solutions to Advent of Code 2015

This repository contains solutions to the [Advent of Code 2015](https://adventofcode.com/2015)
Advent puzzles.  All solutions are written in C++.  The solutions aren't designed to be nice or
pretty.  Instead I am using it as an experiment in understanding how I approach solving
problems.

Each Puzzle lies in its own directory named `puzzle-<day>-<number>`.

The solutions are authored by Matthew Gretton-Dann, and Copyright 2020, Matthew Gretton-Dann.  Licensed under Apache 2.0 - see [license](./LICENSE).

## Building

The build system uses CMake:

```sh
git clone https://github.com/matt-gretton-dann/advent-of-code-2015/
cd advent-of-code-2015
cmake -Bbuild -S.
cmake --build build
```

To run each puzzle then do:

```sh
DAY=01
PUZZLE=01
INPUT=input1.txt
./driver.sh $DAY $PUZZLE $INPUT
```

## Sources

All code is written in C++.

### Directory Layout

Each puzzle is in a self-contained directory `puzzle-DAY-NUMBER`.  

Within the directory there are the following files:

 * Mandatory: `CMakeLists.txt` which contains the build instructions.  Executable should be called
   `puzzle-DAY-NUMBER`.  See below for the expected command-line interface.
 * Optional: `exampleNN.txt`.  The example given in the problem description
 * Optional: `driver.sh`.  Driver program to use if you can't just call the executable.
 * Optional: `*.cc`: C++ sources.

### Command Line Interface

The executable command line interface should just take the input in on standard-input and print its
result on standard-output.

If this is not possible the script `driver.sh` within the directory should be provided (and made
executable).  It should take two arguments - the executable to run and the input file to use.