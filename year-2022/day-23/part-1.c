/* Solution for part 1 of day 23 of Advent of Code (r) 2022.

Copyright (c) 2024, Air Quality And Related Topics.

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

 - See https://adventofcode.com/2022/day/23 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 500
#define MAX_ROWS 200
#define MAX_COLS 200
#define ELF '#'
#define N_ROUNDS 10

static int elves[MAX_ROWS][MAX_COLS][2];
static int imin = 100, imax, jmin = 100, jmax;
static char directions[5] = "NSWE";
static int old = 0, new = 1;

void parse_input(void) {
    /* Initialize grid with input data. */
    int eof = FALSE, i, j, n;
    char line[MAX_LINE_LENGTH];
    for (i = 0; i < MAX_ROWS; i++)
        for (j = 0; j < MAX_COLS; j++)
            elves[i][j][old] = FALSE;
    imax = imin - 1;
    jmax = jmin;
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if ((n = strlen(line)) == 0)
            continue;
        else if (++imax >= MAX_ROWS || jmin + n > MAX_COLS)
            error_exit("Not enough room in array of elves.");
        for (j = 0; j < n; j++)
            if (line[j] == ELF)
                elves[imax][jmin+j][old] = TRUE;
        jmax = max2i(jmax, jmin + n - 1);
    }
}

char elf_direction(int i, int j, int which) {
    /* Return direction of move (N, S, E, or W) of elf at (i,j,which).

       NB: this function returns '0' if (i,j,which) is not occupied by an elf
       and 'x' if it is occupied but the elf cannot move or chooses not to move
       (an elf chooses not to move when there is no one around him/her/they).

     */
    int d, north = 0, south = 0, east = 0, west = 0;
    if (!elves[i][j][which]) return '0';
    for (d = -1; d <= 1; d++) {
        north += elves[i-1][j+d][which] ? 1 : 0;
        south += elves[i+1][j+d][which] ? 1 : 0;
        east += elves[i+d][j+1][which] ? 1 : 0;
        west += elves[i+d][j-1][which] ? 1 : 0;
    }
    if (north + south + east + west == 0) return 'x';
    for (d = 0; d <= 4; d++) {
        if (d == 4) return 'x';
        if (directions[d] == 'N' && north == 0)
            break;
        else if (directions[d] == 'S' && south == 0)
            break;
        else if (directions[d] == 'E' && east == 0)
            break;
        else if (directions[d] == 'W' && west == 0)
            break;
    }
    return directions[d];
}

void play_round(void) {
    /* Play round of movements and update direction order for next round. */
    int i_min = imin, i_max = imax, j_min = jmin, j_max = jmax;
    int i, j, inew, jnew;
    char d;
    if (imin < 2 || imax > MAX_ROWS-3 || jmin < 2 || jmax > MAX_COLS-3)
        error_exit("Please extend the grid.");
    for (i = 0; i < MAX_ROWS; i++)
        for (j = 0; j < MAX_COLS; j++)
            elves[i][j][new] = FALSE;
    imin = jmin = max2i(MAX_ROWS, MAX_COLS);
    imax = jmax = -1;
    for (i = i_min; i <= i_max; i++)
        for (j = j_min; j <= j_max; j++) {
            if ((d = elf_direction(inew = i, jnew = j, old)) == '0')
                continue;
            else if (d == 'N')
                inew -= elf_direction(i-2, j, old) == 'S' ? 0 : 1;
            else if (d == 'S')
                inew += elf_direction(i+2, j, old) == 'N' ? 0 : 1;
            else if (d == 'E')
                jnew += elf_direction(i, j+2, old) == 'W' ? 0 : 1;
            else if (d == 'W')
                jnew -= elf_direction(i, j-2, old) == 'E' ? 0 : 1;
            else if (d != 'x')
                error_exit("Unknown move.");
            elves[inew][jnew][new] = TRUE;
            imin = min2i(imin, inew);
            imax = max2i(imax, inew);
            jmin = min2i(jmin, jnew);
            jmax = max2i(jmax, jnew);
        }
    new = ((old = new) + 1) % 2;
    directions[4] = directions[0];
    for (i = 0; i < 4; i++) directions[i] = directions[i+1];
}

int count_empty_tiles(int which) {
    /* Return the number of empty tiles among the elves. */
    int i, j, sum = 0;
    for (i = imin; i <= imax; i++)
        for (j = jmin; j <= jmax; j++)
            sum += elves[i][j][which] ? 0 : 1;
    return sum;
}

int main(void) {
    /* Print the number of empty tiles among the elves after all rounds. */
    int i;
    parse_input();
    for (i = 0; i < N_ROUNDS; i++) play_round();
    printf("%d\n", count_empty_tiles(old));
    return TRUE;
}
