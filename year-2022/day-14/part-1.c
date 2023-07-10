/* Solution for part 1 of day 14 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/14 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 300
#define MAX_ROWS 500
#define MAX_COLS 600
#define AIR 0
#define ROCK 1
#define SAND 2
#define ISOURCE 0
#define JSOURCE 500

static int cave[MAX_ROWS][MAX_COLS], nrows = 0, ncols = 0;

void add_rock(int istart, int jstart, int iend, int jend) {
    /* Add line of rock to cave from (istart,jstart) through (iend,jend). */
    int is = min2i(istart, iend), ie = max2i(istart, iend);
    int js = min2i(jstart, jend), je = max2i(jstart, jend), k;
    if (is < 0 || ie >= MAX_ROWS || js < 0 || je >= MAX_COLS)
        error_exit("negative index or cave array is too small");
    if (is == ie)
        for (k = js; k <= je; k++)
            cave[is][k] = ROCK;
    else if (js == je)
        for (k = is; k <= ie; k++)
            cave[k][js] = ROCK;
    else
        error_exit("diagonal path");
}

void parse_cave(void) {
    /* Parse input data and set cave, nrows, and ncols accordingly. */
    int eof = FALSE;
    char line[MAX_LINE_LENGTH], bigsep[] = "->", smallsep = ',', *start, *end;
    int xy[2], n = strlen(bigsep), first, istart, jstart;
    while (!eof) {
        if (getlinen(line, MAX_LINE_LENGTH, &eof) == 0)
            continue;
        first = TRUE;
        for (start = line; start != NULL; start = end + n*(end!=NULL)) {
            if ((end = strstr(start, bigsep)) != NULL)
                *end = '\0';
            if (parse_ints(start, smallsep, xy, 2) != 2)
                error_exit("was expecting two values");
            if (first)
                first = FALSE;
            else
                add_rock(istart, jstart, xy[1], xy[0]);
            nrows = max2i(nrows, (istart=xy[1]) + 1);
            ncols = max2i(ncols, (jstart=xy[0]) + 1);
        }
    }
    /* The source must be within the bounds of the array */
    if (ISOURCE < 0 || ISOURCE >= nrows || JSOURCE < 0 || JSOURCE >= ncols)
        error_exit("source is out of bounds");
}

int drop_sand(void) {
    /* Drop unit of sand until it comes to rest (return FALSE) or flows out
       (return TRUE).*/
    int i = ISOURCE, j = JSOURCE, moved;
    do {
        if (++i >= nrows)
            return TRUE;
        else if (cave[i][j] == AIR)
            moved = TRUE;
        else if (--j < 0)
            return TRUE;
        else if (cave[i][j] == AIR)
            moved = TRUE;
        else if ((j += 2) >= ncols)
            return TRUE;
        else if (cave[i][j] == AIR)
            moved = TRUE;
        else {
            cave[--i][--j] = SAND;
            moved = FALSE;
        }
    } while(moved);
    return FALSE;
}

int main(void) {
    /* Print number of sand units that stop moving before sand flows out. */
    int nrest = 0;
    parse_cave();
    while (!drop_sand())
        nrest++;
    printf("%d\n", nrest);
    return TRUE;
}
