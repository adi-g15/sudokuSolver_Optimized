# An Optimized Sudoku Solver
![CMake](https://github.com/AdityaGupta150/sudokuSolver_Optimized/workflows/CMake/badge.svg)
![Build Status](https://travis-ci.com/AdityaGupta150/sudokuSolver_Optimized.svg?branch=master)

An optimised backtracking sudoku solver, that only iterates for the narrowed down possibilities, includes usual unoptimized backtracking too

Implements multi-threading, using upto 9 threads during optimisation, for each row.

And, if you have any questions about this or related, even what may seem dumb to you, feel free to start an issue, or dm me on twitter 🚀

## Building

----

### Windows

Two Ways ->

1. If you have MSVC compiler, open vs code from Visual Studio Developer Command Prompt, and click `Ctrl + Shift + B` to run the default build task

2. Using CMAKE,

    ``` sh
    cd build
    cmake ..
    cmake --build .
    ```

    \* Or open the .sln solution file using visual studio then get executable

    <br>

    Using either way, you will get a sudokuSolver.exe, on a succesful build, that's it ! 😄

### Linux

* Using CMAKE and makefiles-

    ```sh
    cd build
    cmake ..
    make .
    ```

    This will provide you with an executable named `sudokuSolver`, on a successful build 👍

_Fun Fact - This time i have more build methods for Windows, not Linux... quite rare_ 😅

### Working -

* The `sudoku` class (sudoku.hpp) is the primary class, and uses `box<>` (box.hpp) as elements of 9x9 grid (for now, they just store a single value)

* The `sudoku::board` is the grid storing the actual values... and `EMPTY` is an enum var equating to `0`. `sudoku::_possibilities` is also a 2D array, storing `std::set` of possible values at that index

* `sudoku::optimise` iterates through each row, assigns possibile values to each cell... then iterate through each column, and then each 3x3 block, thereby reducing the possibilities, and hence the recursion depth

* `sudoku::solve` applies simple backtrakking to solve the board,
if `sudoku::optimise` has been called earlier, then the optimised vairant of `sudoku::backtrackSolve_opt` will be used, else `sudoku::backtrackSolve`

If you want more detail about something else, or more briefing on the above ones, do raise an issue :D

### Further Improvements -

* `sudoku::optimise()` can be populated with more sudoku tricks, like naked triplets/pairs and others.

    You may refer https://bestofsudoku.com/sudoku-strategy for some strategies
