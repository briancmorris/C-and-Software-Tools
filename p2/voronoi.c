/**
   This program outputs a 100x100 pixel voronoi diagram by accepting
   6 double values from standard input representing the (x, y) coordinates
   of 3 points on the diagram and fills in the remaining coordinates with the
   appropriate colors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/** The valid number of coordinate tokens that must be read in by scanf. */
#define VALID_COUNT 6

/** The exit status of the program if invalid input is encountered. */
#define INVALID_INPUT 100

/** The number of pixels in the image. */
#define NUM_PIXELS 100

/** The maximum intensity of an RGB value. */
#define MAX_INTENSITY 255

/** The minimum intensity of an RGB value. */
#define MIN_INTENSITY 0

/** The power required to square values. */
#define SQUARE 2

/** The x coordinate of the first point. */
double p1x;

/** The y coordinate of the first point. */
double p1y;

/** The x coordinate of the second point. */
double p2x;

/** The y coordinate of the second point. */
double p2y;

/** The x coordinate of the third point. */
double p3x;

/** The y coordinate of the third point. */
double p3y;

/**
   This function returns true if the provided pixel (col, row) is the
   closest point in the diagram to the rounded integer coordinates of
   the given input point (x, y).
   @param col the column number of the pixel
   @param row the row number of the pixel
   @param x the x coordinate of the input point
   @param y the y coordinate of the input point
   @return True if the provided pixel is the closest point in the diagram
           to the rounded integer coordinates of the given input point.
 */
bool nearestPoint( int col, int row, double x, double y )
{
    // Rounded x coordinate for comparison.
    double roundedX = round( x );
    // Rounded y coordinate for comparison.
    double roundedY = round( y );
    // If the provided pixel is equal to the rounded coordinates, return true.
    if ( (double) col == roundedX && (double) row == roundedY ) {
        return true;
    } else {
        return false;
    }
}

/**
   This function accepts the provided pixel (col, row) and prints the pixel with the
   appropriate color. If it's closest to one of the 3 provided points, it becomes white,
   if it's closest to the first point, it becomes red, if it's closest to the second point it
   becomes green, and if it's closest to the third point it becomes blue.
   @param col the pixel column
   @param row the pixel row
 */
void chooseColor ( int col, int row )
{
    // Determine if the given pixel column and row are closest to any of the given 3
    // points. If so, print the pixel with white RGB values.
    bool nearP1 = nearestPoint( col, row, p1x, p1y );
    bool nearP2 = nearestPoint( col, row, p2x, p2y );
    bool nearP3 = nearestPoint( col, row, p3x, p3y );
    if ( nearP1 || nearP2 || nearP3 ) {
        printf("%3d %3d %3d ", MAX_INTENSITY, MAX_INTENSITY, MAX_INTENSITY );
    } else {
        // Determine distance between the 3 points.
        double p1Distance = sqrt( pow( (p1x - col), SQUARE ) + pow( (p1y - row), SQUARE ) );
        double p2Distance = sqrt( pow( (p2x - col), SQUARE ) + pow( (p2y - row), SQUARE ) );
        double p3Distance = sqrt( pow( (p3x - col), SQUARE ) + pow( (p3y - row), SQUARE ) );
        if ( (p1Distance < p2Distance) && (p1Distance < p3Distance) ) {
            // Print pixel closest to point 1 in red.
            printf( "%3d %3d %3d ", MAX_INTENSITY, MIN_INTENSITY, MIN_INTENSITY );
        } else if ( (p2Distance < p1Distance) && (p2Distance < p3Distance) ) {
            // Print pixel closest to point 2 in green.
            printf( "%3d %3d %3d ", MIN_INTENSITY, MAX_INTENSITY, MIN_INTENSITY );
        } else {
            // Print pixel closest to point 3 in blue.
            printf( "%3d %3d %3d ", MIN_INTENSITY, MIN_INTENSITY, MAX_INTENSITY );
        }
    }
}

/**
   The starting point of the program. Accepts 3 points from standard input in (x, y) coordinates
   and creates a 100x100 pixel voronoi diagram based on the position of the 3 points. Returns the
   exit status of the program.
   @return Exit status of the program.
 */
int main()
{
    // Determine if 6 point coordinates were found in standard input and assign them to globals.
    int read_count = scanf( "%lf %lf %lf %lf %lf %lf", &p1x, &p1y, &p2x, &p2y, &p3x, &p3y );

    // If the count is invalid, exit with error code 100.
    if ( read_count != VALID_COUNT ) {
        printf("Invalid input\n");
        return INVALID_INPUT;
    }

    // Print header.
    printf("P3\n%d %d\n%d\n", NUM_PIXELS, NUM_PIXELS, MAX_INTENSITY );
    // Print diagram pixels.
    for ( int i = 0; i < NUM_PIXELS; i++ ) {
        for (int j = 0; j < NUM_PIXELS; j ++ ) {
            chooseColor( j, i );
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
