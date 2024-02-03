/* Solution for part 2 of day 18 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/18 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 20
#define MAX_X 25
#define MAX_Y 25
#define MAX_Z 25

enum {OUTSIDE, LAVACUBE, UNKNOWN};
static int grid[MAX_X][MAX_Y][MAX_Z];

void init_grid(void) {
    /* Initialize grid. */
    int x, y, z;
    for (x = 0; x < MAX_X; x++)
        for (y = 0; y < MAX_Y; y++)
            for (z = 0; z < MAX_Z; z++)
                grid[x][y][z] = UNKNOWN;
}

void add_cube(int x, int y, int z) {
    /* Add cube at position (x,y,z) to the grid.

       My solution relies on the fact that the "enveloppe" of the grid is free
       of lava cubes, so we arrange and check for that here.

     */
    if (++x < 1 || x > MAX_X-2) error_exit("Grid too small (x).");
    if (++y < 1 || y > MAX_Y-2) error_exit("Grid too small (y).");
    if (++z < 1 || z > MAX_Z-2) error_exit("Grid too small (z).");
    grid[x][y][z] = LAVACUBE;
}

void spread_status(int status, int x, int y, int z) {
    /* Set and spread given status, starting at given position. */
    if (status == UNKNOWN) error_exit("Cannot spread UNKNOWN status.");
    if (grid[x][y][z] == status)
        return;
    else if (grid[x][y][z] == UNKNOWN)
        grid[x][y][z] = status;
    else
        return;
    if (x > 0 && grid[x-1][y][z] == UNKNOWN)
        spread_status(status, x-1, y, z);
    if (x < MAX_X-1 && grid[x+1][y][z] == UNKNOWN)
        spread_status(status, x+1, y, z);
    if (y > 0 && grid[x][y-1][z] == UNKNOWN)
        spread_status(status, x, y-1, z);
    if (y < MAX_Y-1 && grid[x][y+1][z] == UNKNOWN)
        spread_status(status, x, y+1, z);
    if (z > 0 && grid[x][y][z-1] == UNKNOWN)
        spread_status(status, x, y, z-1);
    if (z < MAX_Z-1 && grid[x][y][z+1] == UNKNOWN)
        spread_status(status, x, y, z+1);
}

int count_outfacing_faces() {
    /* Return the number of faces exposed to the outside. */
    int x, y, z, sum = 0;
    for (x = 0; x < MAX_X; x++)
        for (y = 0; y < MAX_Y; y++)
            for (z = 0; z < MAX_Z; z++) {
                if (grid[x][y][z] != LAVACUBE) continue;
                if (grid[x-1][y][z] == OUTSIDE)
                    sum += 1;
                if (grid[x+1][y][z] == OUTSIDE)
                    sum += 1;
                if (grid[x][y-1][z] == OUTSIDE)
                    sum += 1;
                if (grid[x][y+1][z] == OUTSIDE)
                    sum += 1;
                if (grid[x][y][z-1] == OUTSIDE)
                    sum += 1;
                if (grid[x][y][z+1] == OUTSIDE)
                    sum += 1;
                }
    return sum;
}

int main(void) {
    /* Print the outfacing surface area of the droplet. */
    int eof = FALSE, coords[3];
    char line[MAX_LINE_LENGTH];
    init_grid();
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (parse_ints(line, ',', coords, 3) != 3)
            error_exit("Could not parse coordinates.");
        add_cube(coords[0], coords[1], coords[2]);
    }
    spread_status(OUTSIDE, 0, 0, 0);
    printf("%d\n", count_outfacing_faces());
    return TRUE;
}
