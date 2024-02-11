/* Solution for part 1 of day 22 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/22 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_LINE_LENGTH 6000
#define MAX_ROWS 250
#define MAX_COLS 250
#define OUTSIDE ' '
#define FREE '.'
#define WALL '#'

enum {RIGHT, DOWN, LEFT, UP, N_FACINGS};
static char grid[MAX_ROWS][MAX_COLS];
static int n_rows = 0, n_cols = 0, i_now = 0, j_now = 0, facing = RIGHT;
static char instructions[MAX_LINE_LENGTH];

void parse_input(void) {
    /* Initialize grid with input data, and set initial position. */
    int eof = FALSE, i, j;
    for (i = 0; i < MAX_ROWS; i++)
        for (j = 0; j < MAX_COLS; j++)
            grid[i][j] = OUTSIDE;
    while (!eof) {
        getlinex(instructions, MAX_LINE_LENGTH, &eof);
        if ((i = strlen(instructions)) == 0)
            continue;
        else if (isdigit(instructions[0]))
            break;
        else if (i > MAX_COLS || n_rows > MAX_ROWS-1)
            error_exit("Not enough room in grid.");
        else {
            strncpy(grid[n_rows++], instructions, i);
            n_cols = max2i(n_cols, i);
        }
    }
    while (grid[i_now][j_now] == OUTSIDE) j_now++;
}

void rotate_current_facing(char direction) {
    /* Update current facing according to given direction. */
    if (direction == 'R')
        facing = (facing + 1) % N_FACINGS;
    else if (direction == 'L')
        facing = (facing == 0 ? N_FACINGS : facing) - 1 ;
    else
        error_exit("Invalid direction for rotation.");
}

void advance_current_position(int n) {
    /* Move current position by n forward (wrap around and stop at walls). */
    int i_old, j_old;
    for (; n > 0; n--) {
        i_old = i_now;
        j_old = j_now;
        if (facing == UP) {
            if (i_now == 0 || grid[--i_now][j_now] == OUTSIDE)
                while(i_now < n_rows-1 && grid[i_now+1][j_now] != OUTSIDE)
                    i_now++;
        } else if (facing == DOWN) {
            if (i_now == n_rows-1 || grid[++i_now][j_now] == OUTSIDE)
                while(i_now > 0 && grid[i_now-1][j_now] != OUTSIDE)
                    i_now--;
        } else if (facing == LEFT) {
            if (j_now == 0 || grid[i_now][--j_now] == OUTSIDE)
                while(j_now < n_cols-1 && grid[i_now][j_now+1] != OUTSIDE)
                    j_now++;
        } else if (facing == RIGHT) {
            if (j_now == n_cols-1 || grid[i_now][++j_now] == OUTSIDE)
                while(j_now > 0 && grid[i_now][j_now-1] != OUTSIDE)
                    j_now--;
        } else
            error_exit("Invalid facing value.");
        if (grid[i_now][j_now] == WALL) {
            i_now = i_old;
            j_now = j_old;
            return;
        }
    }
}

void process_instructions(void) {
    /* Process all instructions (update position and facing accordingly). */
    char *c = instructions, *d = instructions, digits[MAX_LINE_LENGTH];
    while (*c != '\0') {
        while (isdigit(*d)) d++;
        if (c == d)
            rotate_current_facing(*d++);
        else {
            strncpy(digits, c, d-c);
            digits[d-c] = '\0';
            advance_current_position(atoi(digits));
        }
        c = d;
    }
}

int main(void) {
    /* Print the password to the force field. */
    parse_input();
    process_instructions();
    printf("%d\n", 1000*(i_now+1) + 4*(j_now+1) + facing);
    return TRUE;
}
