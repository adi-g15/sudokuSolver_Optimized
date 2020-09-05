#include "sudoku.hpp"
#include "timer.hpp"

#include <iostream>
#include <array>

using std::clog;    //For debugging purposes

int main(int argc, char const *argv[])
{
    sudoku board;

    std::array<std::array<box<unsigned short, unsigned short>, 9>, 9> enteredBoard;
    std::array< std::array< box<unsigned short, unsigned short>, 9 >, 9 > defaultBoard{{
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
    std::cout << "Chose among the following (For selecting the default, just press enter)"
              << "\n1. Go with default"
              << "\n2. Enter board manually"
              << "\n3. Generate a random board (CAUTION- No Gaurantee of Validity)\n";

    unsigned choice;
    std::cin >> std::noskipws >>choice;

    switch (choice)
    {
    case 3:
        board.setBoard(board.sudokuGenerator());
        break;
    case 2:
        unsigned short val;

        std::cout << "Now enter the board's 81 number...\n";
        for (unsigned i = 0; i < 81; ++i)
        {
            std::cin >> val;
            enteredBoard[i/9][i%9] = val;
        }

        try{
            board.setBoard(enteredBoard);
        }catch(std::invalid_argument& e){
            std::cerr<< e.what();
            return 0;
        }
        break;

    default:
        board.setBoard(defaultBoard);
        break;
    }

    timer t;    //for timing the code, you can simply remove it
    t.start();

    board.print();
    board.optimize(); //optimise the board, just comment this if you want UNOPTIMISED
    board.solve();

    t.stop();
    board.print();
    board.printSVG();

    std::cin.ignore();   //to have the window display for some time, remove if not required

    return 0;
}
