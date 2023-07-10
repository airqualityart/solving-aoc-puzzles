/* Solution for part 2 of day 05 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/5 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define MAX_STACKS 20
#define MAX_STACK_HEIGHT 100

int main(void) {
    int eof, i, j, parsing_stacks = TRUE, n, from, to;
    char c, line[MAX_LINE_LENGTH], stacks[MAX_STACKS][MAX_STACK_HEIGHT];
    for (i = 0; i < MAX_STACKS; i++) stacks[i][0] = '\0';
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof)) {
            printf("Could not read line\n");
            return FALSE;
        }
        /* In the input, a blank line separates the description of the initial
           state of the stacks from the list of moves */
        if (strlen(line) == 0) {
            parsing_stacks = FALSE;
            continue;
        }

        if (parsing_stacks) {
            /* Parse the stacks */
            for (i = 0; (c = line[i]) != '\0'; i++) {
                if (isupper(c)) {
                    j = (i - 1) / 4;
                    if (!prepend_c_to_string(c, stacks[j], MAX_STACK_HEIGHT)) {
                        printf("Could not add crate to stack (1).\n");
                        return FALSE;
                    }
                }
            }
        } else {
            /* Move the crates */
            sscanf(line, "move %d from %d to %d", &n, &from, &to);
            if ((j = strlen(stacks[from-1])) < n) {
                printf("There are not enough crates on this stack\n.");
                return FALSE;
            }
            for (i = n; i > 0; i--) {
                c = stacks[from-1][j-i];
                stacks[from-1][j-i] = '\0';
                if (!append_c_to_string(c, stacks[to-1], MAX_STACK_HEIGHT)) {
                    printf("Could not add crate to stack (2).\n");
                    return FALSE;
                }
            }
        }

    } while (!eof);
    /* Print the answer */
    for (i = 0; i < MAX_STACKS; i++) {
        if ((n = strlen(stacks[i])) == 0) continue;
        printf("%c", stacks[i][n-1]);
    }
    printf("\n");
    return TRUE;
}
