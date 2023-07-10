/* Solution for part 1 of day 09 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/9 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_LINE_LENGTH 5
#define MAX_LOCATIONS 10000

static int xhead = 0, yhead = 0, xtail = 0, ytail = 0;
static int visited[MAX_LOCATIONS][2], n_visited = 0;

void initialize_visited(void) {
    /* Initialize resources that keep track of locations visited by the tail.

       We assume that the head and tail start at (0,0), so the tail has
       necessarily visited this location. */
    visited[0][0] = visited[0][1] = 0;
    n_visited++;
}

void update_visited(void) {
    /* Add current tail location to array "visited" if not already there. */
    int i;
    for (i = 0; i < n_visited; i++)
        if (visited[i][0] == xtail && visited[i][1] == ytail) return;
    if (i == MAX_LOCATIONS)
        error_exit("Not enough space in array \"visited\"");
    visited[i][0] = xtail;
    visited[i][1] = ytail;
    n_visited++;
}

void move_tail(void) {
    /* Move the tail if needed, and update the array "visited" accordingly. */
    int dx = xtail-xhead, dy = ytail-yhead, moved = TRUE;
    if (dx == 2 && abs(dy) <= 1) {
        ytail = yhead;
        xtail = xhead + 1;
    } else if (dx == -2 && abs(dy) <= 1) {
        ytail = yhead;
        xtail = xhead - 1;
    } else if (dy == 2 && abs(dx) <= 1) {
        xtail = xhead;
        ytail = yhead + 1;
    } else if (dy == -2 && abs(dx) <= 1) {
        xtail = xhead;
        ytail = yhead - 1;
    } else if (abs(dx) <= 1 && abs(dy) <= 1) {
        moved = FALSE;
    } else
        error_exit("Tail is too far away from head");
    if (moved)
        update_visited();
}

void move_head_and_tail(char direction, int n) {
    /* Move head by n steps in given direction, move tail accordingly, and
       update the array "visited" if necessary. */
    int i;
    for(i = 0; i < n; i++) {
        if (direction == 'U')
            yhead++;
        else if (direction == 'D')
            yhead--;
        else if (direction == 'L')
            xhead--;
        else if (direction == 'R')
            xhead++;
        else
            error_exit("Unknown direction");
        move_tail();
    }
}

int main(void) {
    /* Prints the number of locations that have been visited by the tail. */
    int eof, n;
    char line[MAX_LINE_LENGTH], direction;
    initialize_visited();
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof))
            error_exit("Could not read line");
        if (strlen(line) == 0)
            continue;
        sscanf(line, "%c %d", &direction, &n);
        move_head_and_tail(direction, n);
    } while (!eof);
    printf("%d\n", n_visited);
    return TRUE;
}
