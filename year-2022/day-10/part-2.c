/* Solution for part 2 of day 10 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/10 for the description of the puzzle.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 10
#define CRT_WIDTH 40
#define PIXEL_DARK ' '
#define PIXEL_LIT '#'

static int sprite = 1;

void draw_pixel(void) {
    /* Draw pixel according to current screen and sprite positions, and update
       screen position. */
    static int screen = 0;
    printf("%c", (abs(screen-sprite) <= 1) ? PIXEL_LIT : PIXEL_DARK);
    if (++screen == CRT_WIDTH) {
        printf("\n");
        screen = 0;
    }
}

int main() {
    /* Print the image described by the instructions. */
    int eof, addx;
    char line[MAX_LINE_LENGTH];
    printf("\n");
    do {

        if (!readline(line, MAX_LINE_LENGTH, &eof))
            error_exit("Could not read line");

        if (strlen(line) == 0)
            continue;
        else if (sscanf(line, "addx %d", &addx) == 1) {
            draw_pixel();
            draw_pixel();
            sprite += addx;
        } else if (strcmp(line, "noop") == 0) {
            draw_pixel();
        } else
            error_exit("Unknown command.");

    } while (!eof);
    return TRUE;
}
