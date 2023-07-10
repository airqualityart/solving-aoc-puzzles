/* Solution for part 1 of day 10 of Advent of Code (r) 2022.

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
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 10

static int reg_value = 1;
static int current_cycle = 1;
static int sum_signal_strength = 0;

void check_cycle(void) {
    /* Check whether the current cycle is a checkpoint. If it is, add the
       corresponding signal strength to the total. */
    static int checkpoints[] = {20, 60, 100, 140, 180, 220}, i_chkp = 0;
    static const int n_chkp = sizeof(checkpoints) / sizeof(checkpoints[0]);
    if (i_chkp < n_chkp && current_cycle == checkpoints[i_chkp]) {
        sum_signal_strength += current_cycle * reg_value;
        i_chkp++;
    }
}

int main(void) {
    /* Print the sum of the strengths of the checkpoint signals. */
    int eof, addx;
    char line[MAX_LINE_LENGTH];
    do {

        if (!readline(line, MAX_LINE_LENGTH, &eof))
            error_exit("Could not read line");

        if (strlen(line) == 0)
            continue;
        else if (sscanf(line, "addx %d", &addx) == 1) {
            check_cycle();
            current_cycle++;
            check_cycle();
            reg_value += addx;
            current_cycle++;
        } else if (strcmp(line, "noop") == 0) {
            check_cycle();
            current_cycle++;
        } else
            error_exit("Unknown command.");

    } while (!eof);
    printf("%d\n", sum_signal_strength);
    return TRUE;
}
