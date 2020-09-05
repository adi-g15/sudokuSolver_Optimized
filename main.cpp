#include "sudoku.hpp"
#include "timer.hpp"

#include <iostream>
#include <array>

using std::clog;    //For debugging purposes

int main(int argc, char const *argv[])
{
    std::array< std::array< box<unsigned short, unsigned short>, 9 >, 9 > tmp{{
    // std::vector< std::vector< box<> > > tmp = {
        {5,3,0,0,7,0,0,0,0},
        {6,0,0,1,9,5,0,0,0},
        {0,9,8,0,0,0,0,6,0},
        {8,0,0,0,6,0,0,0,3},
        {4,0,0,8,0,3,0,0,1},
        {7,0,0,0,2,0,0,0,6},
        {0,6,0,0,0,0,2,8,0},
        {0,0,0,4,1,9,0,0,5},
        {0,0,0,0,8,0,0,7,9},
    }};
    sudoku board(tmp);

    // board.solve();

    timer t;
    // board.print();
    t.start();
    board.optimize();
    // board.print();
    // board.printPossibilities();
    board.solve();
    t.stop();
    board.print();
    // std::cout<<std::boolalpha<<board.isValid();

    return 0;
}
//93243153459800
//93294279778300