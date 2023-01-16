/* Solution for part 2 of day 09 of Advent of Code (r) 2022.

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
#define N_KNOTS 10

static int knots[N_KNOTS][2];
static int visited[MAX_LOCATIONS][2], n_visited = 0;

void initialize(void) {
    /* Initialize resources that keep track of locations of all the knots and
       of locations visited by the tail.

       We assume that all the knots start at (0,0), so the tail has necessarily
       visited this location. */
    int i;
    for (i = 0; i < N_KNOTS; i++)
        knots[i][0] = knots[i][1] = 0;
    visited[0][0] = visited[0][1] = 0;
    n_visited++;
}

void update_visited(void) {
    /* Add current tail location to array "visited" if not already there. */
    int i, xtail = knots[N_KNOTS-1][0], ytail = knots[N_KNOTS-1][1];
    for (i = 0; i < n_visited; i++)
        if (visited[i][0] == xtail && visited[i][1] == ytail) return;
    if (i == MAX_LOCATIONS)
        error_abort("Not enough space in array \"visited\"");
    visited[i][0] = xtail;
    visited[i][1] = ytail;
    n_visited++;
}

void move_other_knots(void) {
    /* Move all the knots (except the head) if needed, and update the array
       "visited" accordingly. */
    int i, dx, dy, moved, *head, *tail;
    for (i = 1; i < N_KNOTS; i++) {
        head = knots[i-1];
        tail = knots[i];
        dx = tail[0] - head[0];
        dy = tail[1] - head[1];
        if (abs(dx) > 2 || abs(dy) > 2)
            error_abort("Knots are too far away from each other");
        if (abs(dx) <= 1 && abs(dy) <= 1) {
            moved = FALSE;
            continue;
        }
        if (abs(dx) == 2 && abs(dy) == 2) {
            tail[0] = head[0] + dx/abs(dx);
            tail[1] = head[1] + dy/abs(dy);
        } else if (abs(dx) == 2) {
            tail[0] = head[0] + dx/abs(dx);
            tail[1] = head[1];
        } else if (abs(dy) == 2) {
            tail[0] = head[0];
            tail[1] = head[1] + dy/abs(dy);
        }
        moved = TRUE;
    }
    if (moved)
        update_visited();
}

void move_rope(char direction, int n) {
    /* Move head by n steps in given direction, move the rest of the rope
       accordingly, and update the array "visited" if necessary. */
    int i;
    for(i = 0; i < n; i++) {
        if (direction == 'U')
            knots[0][1] += 1;
        else if (direction == 'D')
            knots[0][1] -= 1;
        else if (direction == 'L')
            knots[0][0] -= 1;
        else if (direction == 'R')
            knots[0][0] += 1;
        else
            error_abort("Unknown direction");
        move_other_knots();
    }
}

int main() {
    /* Prints the number of locations that have been visited by the tail. */
    int eof, n;
    char line[MAX_LINE_LENGTH], direction;
    initialize();
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof))
            error_abort("Could not read line");
        if (strlen(line) == 0)
            continue;
        sscanf(line, "%c %d", &direction, &n);
        move_rope(direction, n);
    } while (!eof);
    printf("%d\n", n_visited);
    return TRUE;
}
