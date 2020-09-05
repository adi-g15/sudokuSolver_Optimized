#include "sudoku.hpp"
#include "util.hpp"

#include <iostream>
#include <utility>
#include <fstream>
#include <vector>
#include <sstream>
#include <random> //for the simplest std::linear_congruential_engine random engine provided by C++ standard
#include <thread>
#include <mutex>

void sudoku::solve()
{
    if (!this->isValid())
    {
        std::cerr << "ERROR: Board passed is not valid\n";
        return;
    }
    if (this->optimised)
    {
        if (!this->backTrackSolve_opt())
        {
            std::cerr << "ERROR: Sudoku Couldn't be solved, verify the board given is right\n";
        }
    }
    else if (!this->backTrackSolve())
    {
        std::cerr << "ERROR: Sudoku Couldn't be solved, verify the board given is right\n";
    }
}

sudoku::ushort sudoku::findNextEmpty(ushort index) const
{ //searches INCLUDING THE PASSED INDEX
    while (true)
    {
        if (this->board[index / 9][index % 9].value == EMPTY)
            return index;
        ++index;
    }
    return 81; //past the board
}

bool sudoku::backTrackSolve()
{
    return this->backTrackSolve(this->findNextEmpty(0));
}

bool sudoku::backTrackSolve(ushort index)
{
    if (index > 80)
        if (this->isValid())
            return true;
        else
            return false;

    for (ushort n = 1; n <= 9; ++n)
    {
        this->board[index / 9][index % 9] = n;

        if (this->isValid())
            if ( this->backTrackSolve(this->findNextEmpty(index)) )
                return true;
    }

    this->board[index / 9][index % 9] = EMPTY; //resetting
    return false;
}

bool sudoku::backTrackSolve_opt()
{
    return this->backTrackSolve_opt(this->findNextEmpty(0));
}

bool sudoku::backTrackSolve_opt(ushort index)
{ //assumes first position passed SHOULD BE EMPTY,... else it will just return false

    if (index > 80)
        if (this->isValid())
            return true;
        else
            return false;

    // if(this->board[index/9][index%9].value != EMPTY){ index = findNextEmpty(index); }    //For SAFETY, though till assumption is abided by, and backTrackSolve_opt(void) is used, this isnt required

    for (auto &n : this->_possibilities[index / 9][index % 9])
    {
        this->board[index / 9][index % 9] = n;

        if (this->isValid())
            if ( this->backTrackSolve_opt(this->findNextEmpty(index)) )
                return true;

        this->board[index / 9][index % 9] = EMPTY; //resetting
    }

    return false;
}

bool sudoku::isValid(const _board_9x9_ &grid) const
{
    std::set<ushort> hashSet;
    for (ushort i = 0; i < 9; ++i)
    {
        for (ushort j = 0; j < 9; ++j)
        {
            if (grid[i][j].value != EMPTY)
                if (!hashSet.insert(grid[i][j].value).second)
                    return false;
        }
        hashSet.clear();
        for (ushort j = 0; j < 9; ++j) //checking each column... notice that we are using indixes in reverse order -> [j][i] here
        {
            if (grid[j][i].value != EMPTY)
                if (!hashSet.insert(grid[j][i].value).second)
                    return false;
        }
        hashSet.clear();
    }

    for (ushort i = 0; i < 9 / 3; ++i)
    {
        for (ushort j = 0; j < 9 / 3; ++j)
        {
            for (ushort i1 = 0; i1 < 3; ++i1)
            {
                for (ushort j1 = 0; j1 < 3; ++j1)
                {
                    if (grid[i * 3 + i1][j * 3 + j1].value != EMPTY)
                        if (!hashSet.insert(grid[i * 3 + i1][j * 3 + j1].value).second)
                            return false;
                }
            }
            hashSet.clear();
        }
    }

    return true;
}

void sudoku::optimize()
{
    //@TODO - Use multiple threads here, probably one per row

    const std::set<ushort> fullPossible{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // thread_local std::set< ushort > tmpPossible(fullPossible);
    // std::set<ushort> tmpPossible(fullPossible);

    std::vector<std::thread> threads;
    std::mutex m;
    auto rowInspect = [&](ushort i) {
        // m.lock();
        std::set<ushort> tmpPossible = fullPossible; //complexity -> N
        // m.unlock();

        for (ushort j = 0; j < 9; ++j) //this loop sets the tmpPossible
        {
            // m.lock();
            tmpPossible.erase(this->board[i][j].value);
            // m.unlock();
        }

        for (ushort j = 0; j < 9; ++j)
        {
            if (this->board[i][j].value != EMPTY)
                continue;

            m.lock();
            this->_possibilities[i][j] = tmpPossible;
            m.unlock();
            // for (ushort j2 = 0; j2 < 9; ++j2) //columnwise       //WORKS, but expensive, iterates through the column for all empty element positions, better iterate column once, and erase the common elements from each element in column
            // {
            //     this->_possibilities[i][j].erase(this->board[j2][j].value);
            // }
        }
    };

    for (ushort i = 0; i < 9; ++i) //rowwise
    {
        threads.push_back(std::thread(rowInspect, i));
    }
    for(auto &t: threads)
        if(t.joinable())    t.join();

    std::set<ushort> tmpPossible;
    for (ushort j = 0; j < 9; ++j) //columnwise
    {
        tmpPossible = fullPossible;
        for (ushort i = 0; i < 9; ++i)
        {
            tmpPossible.erase(this->board[i][j].value);
        }
        for (ushort i = 0; i < 9; ++i)  //complexity -> N*N*N
        {
            if (this->board[i][j].value != EMPTY) //if the block is empty
                continue;

            take_common(this->_possibilities[i][j], tmpPossible); //remove common elements b/w 1st and 2nd FROM 1ST ONE
        }
    }

    //NOTE- Many a times i have used  a single index instead of two for loops, eg. for(0..9), the x-coord will be i/3, while y will be i%3.. for example when iterating through all 9 3x3 boxes

    tmpPossible.clear();    //will now store box content, for each 3x3 block
    for (ushort bi = 0; bi < 9; ++bi)   //box-index... iterate through all 9 3x3 blocks... each next index actually jumps through 3 x units, and 3y units
    {
        for (ushort index = 0; index < 9; ++index)   //the loop to itearte through all values in 3x3 block
            if (this->board[(bi/3)* 3 + index/3][(bi%3)*3 + index%3].value != EMPTY)
                tmpPossible.insert(this->board[(bi / 3) * 3 + index / 3][(bi % 3) * 3 + index % 3].value);

        for (ushort index = 0; index < 9; ++index)  //iterators through all 9 boxes inside the 3x3 block
                if( !this->board[(bi/3)*3 + index/3][(bi%3)*3 + index%3] != EMPTY)
                    remove_common(this->_possibilities[(bi/3)*3 + index/3][(bi%3)*3 + index%3], tmpPossible);

        tmpPossible.clear();
    }

    ushort count = 0;
    //Do the below only after all preprocessing done, and maximum narrowing down has already occurred
    for (ushort i = 0; i < 9; ++i)  //assigning simple possibilities
    {
        for (ushort j = 0; j < 9; ++j) //final assigning of
        {
            if (this->_possibilities[i][j].size() == 1)
            {
                this->board[i][j].assign(this->_possibilities[i][j].lower_bound(0).operator*());
                this->_possibilities[i][j].clear();
                ++count;
            }
        }
    }
    // std::cout<<"Filled "<<count<<" elements, during optimization"<<std::endl;

    if( !this->isValid() )
        throw std::invalid_argument("ERROR: Board invalid after optimisations, recheck the board");
    this->optimised = true;
}

sudoku::_board_9x9_ sudoku::sudokuGenerator()
{
    _board_9x9_ newBoard;
    ushort NUM_CLUES = 18;
    ushort randomIndex;
    std::linear_congruential_engine<ushort, 3, 2, 81> genEngine; // x=(3*x + 2)%81 ... returns [0,81)
    genEngine.seed(std::rand() % 81);

    for (ushort i = 0; i < 9; ++i)
        newBoard[i].fill(EMPTY);

    for (ushort i = 0; i < NUM_CLUES; ++i)
    {
        randomIndex = genEngine();

        if (newBoard[randomIndex / 9][randomIndex % 9] != EMPTY)
        {
            --i;
            continue;
        }
        do
        {
            newBoard[randomIndex / 9][randomIndex % 9] = genEngine() % 9 + 1;
        } while (!this->isValid(newBoard));
    }

    return newBoard;
}

void sudoku::print() const
{
    for (ushort i = 0; i < 39; ++i)
        std::cout << '=';
    std::cout << "\n";

    auto printPos = [&](ushort i, ushort j) {
        if (this->board[i][j].value != EMPTY)
            std::cout << ' ' << this->board[i][j].value << ' ';
        else
            std::cout << "   ";
    };

    for (ushort i = 0; i < 8; ++i)
    {
        std::cout << "\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPos(i, j);
            std::cout << '|';
        }
        printPos(i, 8);
        std::cout << "\\\\\n\\\\";
        for (ushort i = 0; i < 35; ++i)
            std::cout << '-';
        std::cout << "\\\\\n";
    }
    //for the 9th row
    std::cout << "\\\\";
    for (ushort j = 0; j < 8; ++j)
    {
        printPos(8, j);
        std::cout << '|';
    }
    printPos(8, 8);
    std::cout << "\\\\\n";
    for (ushort i = 0; i < 39; ++i)
        std::cout << '=';
    std::cout << std::endl;
}

//this member function referenced from : git@github.com:vaithak/Sudoku-Generator.git
void sudoku::printSVG(std::string parentDir) const
{
    std::string svgFile = "svgHead.txt";

    std::ifstream fin(svgFile); //internally ifstream(str) constructor calls .open(str), which then calls .open() with str.c_str()
    std::ostringstream svgHead;

    svgHead << fin.rdbuf();

    std::ofstream fout(parentDir + "sudoku.svg");
    fout << svgHead.rdbuf();

    for (ushort i = 0; i < 9; ++i)
    {
        for (ushort j = 0; j < 9; ++j)
        {
            if (this->board[i][j].value != EMPTY)
            {
                // std::ostringstream s;
                // s.str(std::string("<text x=\"" + std::to_string((50*j) + 16) + "\" y=\"" + std::to_string((50*i) + 35) + "\" style=\"font-weight:bold\" font-size=\"30px\">" + std::to_string(this->board[i][j].value) + "</text>\n"));

                fout << "<text x=\"" << (50 * j) + 16 << "\" y=\"" << (50 * i) + 35 << "\" style=\"font-weight:bold\" font-size=\"30px\">" << this->board[i][j].value << "</text>\n";
                // fout<<s.rdbuf();
            }
        }
    }

    fout << "</svg>";
}

void sudoku::printPossibilities() const
{
    if (!this->optimised)
        return;

    auto printPoss = [&](ushort i, ushort j) {
        if (this->_possibilities[i][j].size() != 0)
        {
            // std::cout<<' '<<this->_possibilities[i][j].size()<<' ';
            std::cout << "   ";
            if (this->_possibilities[i][j].size() < 3)
            {
                for (auto &&i : this->_possibilities[i][j])
                {
                    std::cout << i << ',';
                }
            }
        }
        else
            std::cout << "   ";
    };

    for (ushort i = 0; i < 39; ++i)
        std::cout << '=';
    std::cout << "\n";

    for (ushort i = 0; i < 8; ++i)
    {
        std::cout << "\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPoss(i, j);
            std::cout << '|';
        }
        printPoss(i, 8);
        std::cout << "\\\\\n\\\\";
        for (ushort i = 0; i < 35; ++i)
            std::cout << '-';
        std::cout << "\\\\\n";
    }
    //for the 9th row
    std::cout << "\\\\";
    for (ushort j = 0; j < 8; ++j)
    {
        printPoss(8, j);
        std::cout << '|';
    }
    printPoss(8, 8);
    std::cout << "\\\\\n";
    for (ushort i = 0; i < 39; ++i)
        std::cout << '=';
    std::cout << std::endl;
}
