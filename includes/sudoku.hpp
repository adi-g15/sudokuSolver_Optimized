#pragma once

#include "box.hpp"
#include <array>
#include <set>
#include <string>
#include <stdexcept>    //for std::invalid_argument

class sudoku{
    typedef unsigned short ushort;
    typedef std::array< std::array< box<ushort, ushort>, 9 >, 9 > _board_9x9_;
    _board_9x9_ board;
    std::array< std::array< std::set< ushort >, 9 >, 9> _possibilities;
    bool optimised = false;
    bool backTrackSolve(ushort index);
    bool backTrackSolve_opt(ushort index);  //uses optimised possibilities

    public:
        enum{
            EMPTY = 0U,
        };
        // sudoku() : board(sudokuGenerator()){}    //initialize with a sample board
        sudoku(const _board_9x9_& inputBoard) : board(inputBoard){
            if(!this->isValid())
                throw std::invalid_argument("ERROR: Sudoku board passed is invalid!");
        }

        void solve();
        bool isValid() const{ return this->isValid(this->board); }
        bool isValid(const _board_9x9_&) const;
        bool backTrackSolve();
        bool backTrackSolve_opt();
        ushort findNextEmpty(ushort index = 0) const;
        void optimize();    //use techniques to narrow down possibilities

        _board_9x9_ sudokuGenerator(); //CAUTION: NOT GUARANTEED, WHETHER SUDOKU WILL BE SOLVABLE, OR HAVING A SINGLE SOLUTION ONLY
        void print() const;
        void printSVG(std::string parentDir = "./pics/") const;
        void printPossibilities() const;
};
