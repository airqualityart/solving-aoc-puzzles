/* Solution for part 1 of day 18 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/18 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 20
#define MAX_X 25
#define MAX_Y 25
#define MAX_Z 25

enum {LEFT, RIGHT, FRONT, BACK, BOTTOM, TOP, N_FACES};
static int grid[MAX_X][MAX_Y][MAX_Z][N_FACES];

void init_grid(void) {
    /* Initialize grid (make it empty). */
    int x, y, z, face;
    for (x = 0; x < MAX_X; x++)
        for (y = 0; y < MAX_Y; y++)
            for (z = 0; z < MAX_Z; z++)
                for (face = 0; face < N_FACES; face++)
                    grid[x][y][z][face] = 0;
}

void add_cube(int x, int y, int z) {
    /* Add cube at position (x,y,z) to the grid. */
    int face;
    if (x < 0 || x >= MAX_X) error_exit("Invalid x coord or grid too small.");
    if (y < 0 || y >= MAX_Y) error_exit("Invalid y coord or grid too small.");
    if (z < 0 || z >= MAX_Z) error_exit("Invalid z coord or grid too small.");
    for (face = 0; face < N_FACES; face++) grid[x][y][z][face] = 1;
    /* Check for adjacent cubes in the x-direction (left/right) */
    if (x > 0 && grid[x-1][y][z][RIGHT] >= 1)
        grid[x][y][z][LEFT] = grid[x-1][y][z][RIGHT] = 2;
    if (x < MAX_X-1 && grid[x+1][y][z][LEFT] >= 1)
        grid[x][y][z][RIGHT] = grid[x+1][y][z][LEFT] = 2;
    /* Check for adjacent cubes in the y-direction (front/back) */
    if (y > 0 && grid[x][y-1][z][BACK] >= 1)
        grid[x][y][z][FRONT] = grid[x][y-1][z][BACK] = 2;
    if (y < MAX_Y-1 && grid[x][y+1][z][FRONT] >= 1)
        grid[x][y][z][BACK] = grid[x][y+1][z][FRONT] = 2;
    /* Check for adjacent cubes in the z-direction (bottom/top) */
    if (z > 0 && grid[x][y][z-1][TOP] >= 1)
        grid[x][y][z][BOTTOM] = grid[x][y][z-1][TOP] = 2;
    if (z < MAX_Z-1 && grid[x][y][z+1][BOTTOM] >= 1)
        grid[x][y][z][TOP] = grid[x][y][z+1][BOTTOM] = 2;
}

int count_exposed_faces(void) {
    /* Return the number of exposed faces. */
    int x, y, z, face, sum = 0;
    for (x = 0; x < MAX_X; x++)
        for (y = 0; y < MAX_Y; y++)
            for (z = 0; z < MAX_Z; z++)
                for (face = 0; face < N_FACES; face++)
                    if (grid[x][y][z][face] == 1) sum += 1;
    return sum;
}

int main(void) {
    /* Print the total surface area of the droplet. */
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
    printf("%d\n", count_exposed_faces());
    return TRUE;
}
