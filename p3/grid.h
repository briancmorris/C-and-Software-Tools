/**
    @file grid.h
    @author Brian Morris (bcmorri3)

    The grid.h header file declares and describes the functions needed to display a
    crossword puzzle with words oriented in a horizontal or vertical manner. Error
    checking is dependant upon the function caller.
 */

/**
    The writeHorizontal function stores the provided word in the two-dimensional grid
    array with a horizontal orientation beginning at the starting row rpos, and the
    starting column cpos.

    @param rpos the starting row of the word.
    @param cpos the starting column of the word.
    @param word the word to be stored in the grid.
    @param rows the number of rows in the grid.
    @param cols the number of columns in the grid.
    @param grid the two-dimensional char array used to store the provided word.
 */
void writeHorizontal( int rpos, int cpos, char word[], int rows, int cols, char grid[rows][cols] );

/**
    The writeVertical function stores the provided word in the two-dimensional grid
    array with a vertical orientation beginning at the starting row rpos, and the
    starting column cpos.

    @param rpos the starting row of the word.
    @param cpos the starting column of the word.
    @param word the word to be stored in the grid.
    @param rows the number of rows in the grid.
    @param cols the number of columns in the grid.
    @param grid the two-dimensional char array used to store the provided word.
 */
void writeVertical( int rpos, int cpos, char word[], int rows, int cols, char grid[rows][cols] );

/**
    The printGrid function prints the provided two-dimensional board array with a
    space between each character except the final character on a row.

    @param row the number of rows in the board.
    @param cols the number of columns in the board.
    @param board the board to be printed.
 */
void printGrid ( int rows, int cols, char board[ rows ][ cols ] );
