/* Solution for part 2 of day 12 of Advent of Code (r) 2022.

Copyright (c) 2023, Air Quality And Related Topics.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

  (1) Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

  (2) Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

  (3) The name of the author may not be used to endorse or promote products
  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

Notes :

 - This work is in no way a promotion for or an endorsement of Advent of Code
   or any of its products or assets. I have no ties whatsoever with Advent of
   Code. I just solve its puzzles for fun. Advent of Code is a registered
   trademark in the United States of America. See the Advent of Code website
   (https://adventofcode.com) for more information.

 - See https://adventofcode.com/2022/day/12 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_ROWS 200
#define MAX_COLS 200
#define NO_DIST -1
#define CHAR_START 'S'
#define CHAR_END 'E'

typedef struct {int i; int j;} Location;

static int map[MAX_ROWS][MAX_COLS], nrows, ncols;
static Location end;

int calc_elevation(char c) {
    /* Return the elevation (a=1 to z=26) associated with given character.

       Note: here we use a platform-dependent implementation, because we assume
       that the characters a-z are contiguous and in that order in the
       platform's character set.

     */
    if (c == CHAR_START)
        return calc_elevation('a');
    else if (c == CHAR_END)
        return calc_elevation('z');
    else
        return c - 'a' + 1;
}

void read_map(void) {
    /* Read map from standard input. */
    int eof = FALSE, j;
    char line[MAX_COLS+1], c;
    nrows = 0;
    while (!eof) {
        getlinex(line, MAX_COLS+1, &eof);
        if (strlen(line) == 0)
            continue;
        ncols = strlen(line);
        if (nrows >= MAX_ROWS)
            error_exit("not enough room in map array");
        for (j = 0; j < ncols; j++) {
            map[nrows][j] = calc_elevation(c = line[j]);
            if (c == CHAR_END) {
                end.i = nrows;
                end.j = j;
            }
        }
        nrows++;
    }
}

Location *check_neighbors(int distances[MAX_ROWS][MAX_COLS],
                          Location loc, Location* last) {
    /* Check all cells adjacent to given location, and, if needed:

       - update their distance value.
       - add them to the list of cells that remain to be checked.

       Argument "last" points to the last valid element of the list of cells
       that currently remain to be checked.

       This function returns a pointer that points to the last valid element of
       the list of cells that remain to be checked (this function might have
       added cells to the list).
    */
    int i = loc.i, j = loc.j, e = map[i][j], next_dist = distances[i][j] + 1;
    if (i-1 >= 0 && distances[i-1][j] == NO_DIST && e - map[i-1][j] <= 1) {
        distances[i-1][j] = next_dist;
        (++last)->i = i - 1;
        last->j = j;
    }
    if (i+1 < nrows && distances[i+1][j] == NO_DIST && e - map[i+1][j] <= 1) {
        distances[i+1][j] = next_dist;
        (++last)->i = i + 1;
        last->j = j;
    }
    if (j-1 >= 0 && distances[i][j-1] == NO_DIST && e - map[i][j-1] <= 1) {
        distances[i][j-1] = next_dist;
        (++last)->i = i;
        last->j = j - 1;
    }
    if (j+1 < ncols && distances[i][j+1] == NO_DIST && e - map[i][j+1] <= 1) {
        distances[i][j+1] = next_dist;
        (++last)->i = i;
        last->j = j + 1;
    }
    return last;
}

void calc_distances(int distances[MAX_ROWS][MAX_COLS], Location from) {
    /* Fill out the array with shortest distances to given location, walking
       backward. */
    int i, j;
    Location cells_to_check[MAX_ROWS*MAX_COLS];
    Location *first = cells_to_check, *last = cells_to_check;
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
            distances[i][j] = NO_DIST;
    distances[from.i][from.j] = 0;
    *first = from;
    while (first <= last)
        last = check_neighbors(distances, *first++, last);
}

int main(void) {
    /* Print length of shortest path between any position 'a' and end. */
    int distances[MAX_ROWS][MAX_COLS];
    int i, j, elevation = calc_elevation('a'), min = MAX_ROWS*MAX_COLS;
    read_map();
    calc_distances(distances, end);
    for (i = 0; i < nrows; i++)
        for (j = 0; j < ncols; j++)
            if (map[i][j] == elevation
                && distances[i][j] != NO_DIST && distances[i][j] < min)
                min = distances[i][j];
    printf("%d\n", min);
    return TRUE;
}
