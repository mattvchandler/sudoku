# Sudoku solver

Just a simple brute-force sudoku solver.

Can solve any 'standard' n x n puzzle, where n is a perfect square, although it
may be really slow for anything above 9x9.

## Input file format
Input is a simple text file, with the following properties:

* Each line contains a row
* Each row contains a number or letter for each filled square
    * 1-9 and A-Z are allowed, depending on the size of the puzzle
    * 0 is used to mark a empty square
* Each number in a row is separated with one or more of the following:
    * space
    * tab
    * comma
    * semicolon
    * quote
    * nothing

Most forms of CSV or other delimited text files will work fine
