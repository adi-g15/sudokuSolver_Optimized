#include <iostream>
#include <utility>
#include <array>
#include <set>
#include <unordered_set>

using std::clog;

template<typename T, typename indexType = size_t>
struct box{
    typedef std::pair<indexType, indexType> _coord;

    private:
        // static indexType row, col, currIndex;   //currIndex is 0-based
        T value;
        // _coord coordinate;

    public:
        box() 
            // coordinate({currIndex/(row+1), currIndex%(col+1)}),
            {
                // ++currIndex;
        }
        // box(const T& val, const _coord& coord = {currIndex/row, currIndex%row}): 
        box(const T& val):
            value(val)
            {

            }

            //LEARNT -> It is special, since we dont give it the return type
        operator T() {    //to auto decay to T
            return this->value;
        }

        auto operator==(const T& val){
            return this->value == val;
        }
        auto operator=(const T& val){
            return this->assign(val);
        }
        auto assign(const T& val){ this->value = val; }
        auto get(){ return this->value; }

    friend class sudoku;
};

    //inilitializing static members
// indexType box<unsigned short, unsigned short>::row = 0;
// indexType box<unsigned short, unsigned short>::col = 0;
// indexType box<unsigned short, unsigned short>::currIndex = 0;

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
        bool isValid() const;
        bool backTrackSolve();
        bool backTrackSolve_opt();
        ushort findNextEmpty(ushort index = 0) const;
        void optimize();    //use techniques to narrow down possibilities

        void print() const;
        void printPossibilities() const;
};

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

    board.print();
    board.solve();
    // board.optimize();
    // board.printPossibilities();
    board.print();
    std::cout<<std::boolalpha<<board.isValid();

    return 0;
}

void sudoku::solve(){
    if(!this->isValid()){
        std::cerr<<"ERROR: Board passed is not valid\n";
        return;
    }
    if( this->optimised ){
        if( !this->backTrackSolve_opt() ){
            std::cerr<<"ERROR: Sudoku Couldn't be solved, verify the board given is right\n";
        }
    }
    else if( !this->backTrackSolve() ){
            std::cerr<<"ERROR: Sudoku Couldn't be solved, verify the board given is right\n";
        }
}

sudoku::ushort sudoku::findNextEmpty(ushort index) const{   //searches INCLUDING THE PASSED INDEX
    while (true)
    {
        if(this->board[index/9][index%9].value == EMPTY)
            return index;
        ++index;
    }
    return 81;  //past the board
}

bool sudoku::backTrackSolve(){
    return this->backTrackSolve(this->findNextEmpty(0));
}

bool sudoku::backTrackSolve(ushort index){
    if(index>80)
        if(this->isValid())
            return true;
        else return false;

    for (ushort n = 1; n <= 9; ++n)
    {
        this->board[index/9][index%9] = n;

        if(this->isValid())
            if( this->backTrackSolve(this->findNextEmpty(index)) )
                return true;
    }

    this->board[index/9][index%9] = EMPTY;  //resetting
    return false;
}

bool sudoku::backTrackSolve_opt(){
    return this->backTrackSolve_opt(this->findNextEmpty(0));
}

bool sudoku::backTrackSolve_opt(ushort index){  //assumes first position passed SHOULD BE EMPTY,... else it will just return false

    std::clog<<"We are at "<<index<<'\n';

    if(index>80)
        if(this->isValid())
            return true;
        else return false;

    // if(this->board[index/9][index%9].value != EMPTY){ index = findNextEmpty(index); }    //For SAFETY, though till assumption is abided by, and backTrackSolve_opt(void) is used, this isnt required

    for (auto &n: this->_possibilities[index/9][index%9])
    {
        this->board[index/9][index%9] = n;

        if(this->isValid())
            if( !this->backTrackSolve_opt(this->findNextEmpty(index)) )
                return true;

        this->board[index/9][index%9] = EMPTY;  //resetting
    }

    return false;
}

bool sudoku::isValid() const{
    std::unordered_set<ushort> hashSet;
    for (ushort i=0; i<9; ++i)
    {
        for (ushort j = 0; j < 9; ++j)
        {
            if(this->board[i][j].value != EMPTY)
                if( !hashSet.insert(this->board[i][j].value).second )
                    return false;
        }
        hashSet.clear();
        for (ushort j = 0; j < 9; ++j)  //checking each column... notice that we are using indixes in reverse order -> [j][i] here
        {
            if(this->board[j][i].value != EMPTY)
                if( !hashSet.insert(this->board[j][i].value).second )
                    return false;
        }
        hashSet.clear();
    }

    for (ushort i = 0; i < 9/3; ++i)
    {
        for (ushort j = 0; j < 9/3; ++j)
        {
            for (ushort i1 = 0; i1 < 3; ++i1)
            {
                for (ushort j1 = 0; j1 < 3; ++j1)
                {
                    if(this->board[i*3 + i1][j*3 + j1].value != EMPTY)
                        if( !hashSet.insert(this->board[i*3 + i1][j*3 + j1].value).second )
                            return false;
                }
            }
            hashSet.clear();
        }
    }

    return true;
}

void sudoku::optimize(){
    //@TODO - Use multiple threads here, probably one per row

    const std::set< ushort > fullPossible{1,2,3,4,5,6,7,8,9};
    // thread_local std::set< ushort > tmpPossible(fullPossible);
    std::set< ushort > tmpPossible(fullPossible);

    for (ushort i = 0; i < 9; ++i)  //rowwise
    {
        // tmpPossible.merge(fullPossible);    //complexity N*log(N+size())    ... could have directly used assignment
        tmpPossible = fullPossible; //complexity -> N
        for (ushort j = 0; j < 9; ++j)
        {
            tmpPossible.erase(this->board[i][j].value);
        }
        // this->_possibilities[i].fill(tmpPossible);
        for (ushort j = 0; j < 9; ++j)
        {
            if(this->board[i][j].value == EMPTY)    //if the block is empty
                this->_possibilities[i][j] = tmpPossible;
        }

        for (ushort j = 0; j < 9; ++j)  //columnwise
        {
            tmpPossible.insert(this->board[j][i].value);
            for (ushort i2 = 0; i2 < 9; ++i2)  //rowwise
            {
                this->_possibilities[i][j].erase( this->board[i2][j].value );
            }
        }
    }

    tmpPossible.clear();
    for (ushort i = 0; i < 9/3; ++i)
    {
        for (ushort j = 0; j < 9/3; ++j)
        {
            for (ushort i1 = 0; i1 < 3; ++i1)
            {
                for (ushort j1 = 0; j1 < 3; ++j1)
                {
                    if(this->board[i*3 + i1][j*3 + j1].value != EMPTY)
                        tmpPossible.insert(this->board[i*3 + i1][j*3 + j1].value).second;
                }
            }

            for (ushort i1 = 0; i1 < 3; ++i1)
            {
                for (ushort j1 = 0; j1 < 3; ++j1)
                {
                    for(auto &val : tmpPossible)
                        this->_possibilities[i*3 +i1][j*3 +j1].erase(val);
                }
            }
            tmpPossible.clear();
        }
    }




    ushort count = 0;
        //Do the below only after all preprocessing done, and maximum narrowing down has already occurred
    for (ushort i = 0; i < 9; ++i){
        for (ushort j = 0; j < 9; ++j)  //final assigning of 
        {
            if( this->_possibilities[i][j].size() == 1){
                this->board[i][j].assign( this->_possibilities[i][j].lower_bound(0).operator*() );
                this->_possibilities[i][j].clear();
                ++count;
            }
        }
    }
    std::cout<<"Filled "<<count<<" elements, during optimization"<<std::endl;

    this->optimised = true;

}

void sudoku::print() const{
    for (ushort i = 0; i < 39; ++i) std::cout<<'=';
    std::cout<<"\n";

    auto printPos = [&](ushort i, ushort j){
        if( this->board[i][j].value != EMPTY )
            std::cout<<' '<<this->board[i][j].value<<' ';
        else std::cout<<"   ";
    };


    for (ushort i = 0; i < 8; ++i)
    {
        std::cout<<"\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPos(i,j);
            std::cout<<'|';
        }
        printPos(i,8);
        std::cout<<"\\\\\n\\\\";
        for (ushort i = 0; i < 35; ++i) std::cout<<'-';
        std::cout<<"\\\\\n";
    }
        //for the 9th row
        std::cout<<"\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPos(8,j);
            std::cout<<'|';
        }
        printPos(8,8);
        std::cout<<"\\\\\n";
        for (ushort i = 0; i < 39; ++i) std::cout<<'=';
        std::cout<<std::endl;

}

void sudoku::printPossibilities() const{
    if( !this->optimised )
        return;

    auto printPoss = [&](ushort i, ushort j){
        if( this->_possibilities[i][j].size() != 0 ){
            // std::cout<<' '<<this->_possibilities[i][j].size()<<' ';
            std::cout<<"   ";
            if(this->_possibilities[i][j].size() < 3){
                for (auto &&i : this->_possibilities[i][j])
                {
                    std::cout<<i<<',';
                }
                
            }
        }
        else std::cout<<"   ";
    };

    for (ushort i = 0; i < 39; ++i) std::cout<<'=';
    std::cout<<"\n";

    for (ushort i = 0; i < 8; ++i)
    {
        std::cout<<"\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPoss(i,j);
            std::cout<<'|';
        }
        printPoss(i,8);
        std::cout<<"\\\\\n\\\\";
        for (ushort i = 0; i < 35; ++i) std::cout<<'-';
        std::cout<<"\\\\\n";
    }
        //for the 9th row
        std::cout<<"\\\\";
        for (ushort j = 0; j < 8; ++j)
        {
            printPoss(8,j);
            std::cout<<'|';
        }
        printPoss(8, 8);
        std::cout<<"\\\\\n";
        for (ushort i = 0; i < 39; ++i) std::cout<<'=';
        std::cout<<std::endl;

}
