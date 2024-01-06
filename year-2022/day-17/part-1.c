/* Solution for part 1 of day 17 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/17 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 12000
#define CHIMNEY_WIDTH 7
#define CHIMNEY_HEIGHT 5000
#define MAX_ROCK_SIZE 4
#define N_ROCK_TYPES 5
#define FREE 0
#define OCCUPIED 1
#define LEFT '<'
#define RIGHT '>'
#define DOWN 'v'

typedef struct struct_rock {
    int shape[MAX_ROCK_SIZE][MAX_ROCK_SIZE];
    int width;
    int height;
} Rock;

static Rock rocks[N_ROCK_TYPES];
static int chimney[CHIMNEY_WIDTH+2][CHIMNEY_HEIGHT+1];
static char pattern_of_moves[MAX_LINE_LENGTH];
static int y_top = 0;

void init_chimney(void) {
    /* Initialize empty chimney (making bottom and side walls out of rock). */
    int i, j;
    for (i = 0; i < CHIMNEY_WIDTH+2; i++) chimney[i][0] = OCCUPIED;
    for (j = 1; j < CHIMNEY_HEIGHT+1; j++) {
        chimney[0][j] = chimney[CHIMNEY_WIDTH+1][j] = OCCUPIED;
        for (i = 1; i < CHIMNEY_WIDTH+1; i++) chimney[i][j] = FREE;
    }
}

void define_rocks(void) {
    /* Define the different types of rocks (in the order they appear). */
    int i, j;
    Rock *rock;
    /* Fill shapes with free spots */
    for (rock = rocks; rock < rocks + N_ROCK_TYPES; rock++)
        for (i = 0; i < MAX_ROCK_SIZE; i++)
            for (j = 0; j < MAX_ROCK_SIZE; j++)
                rock->shape[i][j] = FREE;
    /* Type 0: horizontal bar */
    for (rock = rocks, i = 0; i < 4; i++) rock->shape[i][0] = OCCUPIED;
    /* Type 1: plus sign */
    for (rock++, i = 0; i < 3; i++) rock->shape[i][1] = OCCUPIED;
    rock->shape[1][0] = rock->shape[1][2] = OCCUPIED;
    /* Type 2: L-shape */
    for (rock++, i = 0; i < 3; i++) rock->shape[i][0] = OCCUPIED;
    rock->shape[2][1] = rock->shape[2][2] = OCCUPIED;
    /* Type 3: vertical bar */
    for (rock++, j = 0; j < 4; j++) rock->shape[0][j] = OCCUPIED;
    /* Type 4: square */
    for (rock++, i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            rock->shape[i][j] = OCCUPIED;
    /* Calculate the width and height of each rock */
    for (rock = rocks; rock < rocks + N_ROCK_TYPES; rock++) {
        rock->width = rock->height = -1;
        for (i = 0; i < MAX_ROCK_SIZE; i++)
            for (j = 0; j < MAX_ROCK_SIZE; j++)
                if (rock->shape[i][j] == OCCUPIED) {
                    rock->width = max2i(rock->width, i+1);
                    rock->height = max2i(rock->height, j+1);
                }
    }
}

int collision(int rock_type, int x, int y) {
    /* Return TRUE iff given rock at (x, y) collides with rock or wall. */
    int i, j;
    Rock *rock = rocks + rock_type;
    for (i = 0; i < rock->width; i++)
        for (j = 0; j < rock->height; j++)
            if (rock->shape[i][j] == OCCUPIED && chimney[x+i][y+j] == OCCUPIED)
                return TRUE;
    return FALSE;
}

char* make_rock_fall(int rock_type, char *moves) {
    /* Make one rock fall into chimney, return remaining moves. */
    int x = 3, y = y_top + 4, stop = FALSE, i, j;
    char move = DOWN;
    Rock *rock = rocks + rock_type;
    if (y + rock->height - 1 > CHIMNEY_HEIGHT)
        error_exit("The chimney is too small.");
    while (! stop) {
        move = (move == DOWN) ? *moves++ : DOWN;
        if (move == '\0') move = (moves = pattern_of_moves+1)[-1];
        if (move == LEFT)
            x -= (collision(rock_type, x-1, y)) ? 0 : 1;
        else if (move == RIGHT)
            x += (collision(rock_type, x+1, y)) ? 0 : 1;
        else if (move == DOWN)
            y -= (stop = collision(rock_type, x, y-1)) ? 0 : 1;
        else
            error_exit("Invalid move.");
    }
    for (i = 0; i < rock->width; i++)
        for (j = 0; j < rock->height; j++)
            if (rock->shape[i][j] == OCCUPIED)
                chimney[x+i][y+j] = OCCUPIED;
    y_top = max2i(y_top, y + rock->height - 1);
    return moves;
}

int main(void) {
    /* Print the height of the tower of rocks.

       NB: the first line of input defines the number of falling rocks.

     */
    int eof, n_rocks, rock_type, i;
    char *moves;
    getlinex(pattern_of_moves, MAX_LINE_LENGTH, &eof);
    sscanf(pattern_of_moves, "%d", &n_rocks);
    getlinex(pattern_of_moves, MAX_LINE_LENGTH, &eof);
    init_chimney();
    define_rocks();
    for (i = rock_type = 0, moves = pattern_of_moves; i < n_rocks; i++) {
        moves = make_rock_fall(rock_type, moves);
        rock_type = (rock_type + 1) % N_ROCK_TYPES;
    }
    printf("%d\n", y_top);
    return TRUE;
}
