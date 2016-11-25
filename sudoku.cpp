// Copyright 2016 Matthew Chandler

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

#include <cctype>
#include <cmath>
#include <cstring>

template<std::size_t size> using Board = std::array<std::array<char, size>, size>;
template<std::size_t size> using Alphabet = std::array<char, size>;
using Board_vec = std::vector<std::vector<char>>;

// method based on babalonian sqrt
bool is_square(std::size_t n)
{
    if(n == 0 || n == 1)
        return true;

    auto x = n / 2;
    std::unordered_set<std::size_t> seen;
    seen.insert(x);

    while(x * x != n)
    {
        x = (x + (n / x)) / 2;
        if(seen.count(x))
            return false;
        seen.insert(x);
    }
    return true;
}

// print a board
// size must be a perfect square
template <std::size_t size> void disp_board(const Board<size> & board)
{
    constexpr std::size_t box_size = sqrt(size);

    std::cout<<"+-";
    for(std::size_t c = 0; c < size; ++c)
    {
        std::cout<<"--";

        if((c + 1) % box_size == 0)
        {
            std::cout<<"+";
            if(c != size - 1)
                std::cout<<"-";
        }
    }
    std::cout<<std::endl;

    for(std::size_t r = 0; r < size; ++r)
    {
        std::cout<<"| ";
        for(std::size_t c = 0; c < size; ++c)
        {
            if(board[r][c] == '0')
                std::cout<<"  ";
            else
                std::cout<<board[r][c]<<" ";

            if((c + 1) % box_size == 0)
                std::cout<<"| ";
        }
        std::cout<<std::endl;

        if((r + 1) % box_size == 0)
        {
            std::cout<<"+-";
            for(std::size_t c = 0; c < size; ++c)
            {
                std::cout<<"--";

                if((c + 1) % box_size == 0)
                {
                    std::cout<<"+";
                    if(c != size - 1)
                        std::cout<<"-";
                }
            }
            std::cout<<std::endl;
        }
    }
    std::cout<<std::endl;
}

// size must be a perfect square
template<std::size_t size> constexpr Alphabet<size> list_alphabet()
{
    Alphabet<size> alphabet = {};

    std::size_t n = 0;
    for(char i = '1'; i <= '9' && n < size; ++i, ++n)
        alphabet[n] = i;
    for(char i = 'A'; i <= 'Z' && n < size; ++i, ++n)
        alphabet[n] = i;

    return alphabet;
}

// c must be 1-9 or A-Z
// returns c's index in list_alphabet
inline std::size_t get_alphabet_index(const char c)
{
    if(c >= '1' && c <= '9')
        return c - '1';
    else
        return 9 + c - 'A';
}

template <std::size_t size> Alphabet<size> possible_moves(const Board<size> & board,
    const std::size_t row, const std::size_t col)
{
    constexpr std::size_t box_size = sqrt(size);

    auto poss = list_alphabet<size>();

    // scan row & columns
    for(std::size_t i = 0; i < size; ++i)
    {
        if(board[row][i] != '0')
            poss[get_alphabet_index(board[row][i])] = '0';
        if(board[i][col] != '0')
            poss[get_alphabet_index(board[i][col])] = '0';
    }
    for(std::size_t r = (row / box_size) * box_size;
        r < (row / box_size) * (box_size + 1); ++r)
    {
        for(std::size_t c = (col / box_size) * box_size;
            c < (col / box_size) * (box_size + 1); ++c)
        {
            if(board[r][c] != '0')
                poss[get_alphabet_index(board[r][c])] = '0';
        }
    }

    return poss;
}

// recursive-backtracking solver
template <std::size_t size> bool solve_r(Board<size> & board, const std::size_t row = 0, const std::size_t col = 0)
{
    if(row > size - 1)
        return true;

    // get next pos
    std::size_t next_row = row;
    if(col == size - 1)
        ++next_row;

    std::size_t next_col = (col + 1) % size;

    // skip filled squares
    if(board[row][col] != '0')
        return solve_r(board, next_row, next_col);

    auto poss = possible_moves(board, row, col);

    for(std::size_t i = 0; i < size; ++i)
    {
        if(poss[i] == '0')
            continue;

        board[row][col] = poss[i];

        if(solve_r(board, next_row, next_col))
            return true;
    }
    board[row][col] = '0';

    return false;
}

// size must be a perfect square, and input_board must have dimensions of size x size
template <std::size_t size> bool solve(const Board_vec & input_board)
{
    Board<size> board;
    for(std::size_t r = 0; r < size; ++r)
    {
        std::copy(input_board[r].begin(), input_board[r].end(), board[r].begin());
    }

    disp_board<size>(board);

    // fill immediately fillable squares to speed things up
    const auto num_fill_passes = 2;
    for(auto pass = 0; pass < num_fill_passes; ++pass)
    {
        bool solved = true;
        for(std::size_t row = 0; row < size; ++row)
        {
            for(std::size_t col = 0; col < size; ++col)
            {
                if(board[row][col] != '0')
                    continue;

                solved = false;

                auto possible = possible_moves(board, row, col);

                char poss_value = '\0';
                for(auto & i: possible)
                {
                    if(i == '0')
                        continue;

                    if(poss_value)
                    {
                        poss_value = '\0';
                        break;
                    }

                    poss_value = i;
                }

                if(poss_value)
                    board[row][col] = poss_value;
            }
        }

        if(solved)
        {
            disp_board<size>(board);
            return true;
        }
    }

    bool solved = solve_r<size>(board);

    disp_board<size>(board);

    return solved;
}

// load and verify a board from file
bool load_board(std::ifstream & input, Board_vec & board)
{
    board.emplace_back();
    std::unordered_set<char> alphabet;

    char c;
    while(input.get(c))
    {
        if(std::isalnum(c))
        {
            if(std::islower(c))
                c = std::toupper(c);

            board.back().emplace_back(c);
            alphabet.insert(c);
        }
        else if(c == '\n' || c == '\r') // newline
        {
            if(!board.back().empty())
            {
                board.emplace_back();
            }
        }
        else if(c == '#') // comment
        {
            while(input.get(c))
            {
                if(c == '\n' || c == '\r') // read until newline
                {
                    input.unget();
                    break;
                }
            }
        }
        else if(!std::isblank(c) && c != ',' && c != ';' && c != '\'' && c != '"')
        {
            std::cerr<<"Error: illegal character ("<<c<<") in input."<<std::endl;
            return false;
        }
    }
    if(board.back().empty())
        board.pop_back();

    if(board.empty())
    {
        std::cerr<<"No input given"<<std::endl;
        return false;
    }

    std::size_t size = board.size();
    for(auto &r : board)
    {
        if(r.size() != size)
        {
            std::cerr<<"Board must be square"<<std::endl;
            return false;
        }
    }

    if(size > 25)
    {
        std::cerr<<"Board is too large (not enough symbols available). Max = 25"<<std::endl;
        return false;
    }

    if(!is_square(size))
    {
        std::cerr<<"Board size must be a perfect square"<<std::endl;
        return false;
    }

    auto valid_alphabet_arr = list_alphabet<25>();
    std::unordered_set<char> valid_alphabet;
    valid_alphabet.insert('0');
    valid_alphabet.insert(valid_alphabet_arr.begin(), valid_alphabet_arr.begin() + size);

    for(auto &c: alphabet)
    {
        if(!valid_alphabet.count(c))
        {
            std::cerr<<c<<" is not valid for a puzzle of size "<<size<<std::endl;
            return false;
        }
    }

    return true;
}

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cerr<<"Incorrect number of args.\nUsage: sudoku INPUT_FILE"<<std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream input(argv[1]);
    try
    {
        input.exceptions(std::ifstream::failbit | std::ifstream::badbit); // throw on error OR failure
    }
    catch(std::system_error & e)
    {
        std::cerr<<"Error opening "<<argv[1]<<": "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }

    try
    {
        input.exceptions(std::ifstream::badbit); // only throw on error

        Board_vec board;
        if(!load_board(input, board))
            return EXIT_FAILURE;

        auto start_time = std::chrono::high_resolution_clock::now();

        // stupid trick to use (hopefully) fast templated std::array solver
        bool solved = false;
        switch(board.size())
        {
        case 1:
            solved = solve<1>(board);
            break;
        case 4:
            solved = solve<4>(board);
            break;
        case 9:
            solved = solve<9>(board);
            break;
        case 16:
            solved = solve<16>(board);
            break;
        case 25:
            solved = solve<25>(board);
            break;
        default:
            std::cerr<<"Invalid board size: "<<board.size()<<std::endl;
            return EXIT_FAILURE;
        }

        if(!solved)
            std::cout<<"No solution found"<<std::endl;

        auto end_time = std::chrono::high_resolution_clock::now();
        std::cout<<"time: "<<std::chrono::duration<double>(end_time - start_time).count()<<"s"<<std::endl;
    }
    catch(std::system_error & e)
    {
        std::cerr<<"Error reading "<<argv[1]<<": "<<strerror(errno)<<std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
