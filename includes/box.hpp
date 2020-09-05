#pragma once

#include <utility>

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
