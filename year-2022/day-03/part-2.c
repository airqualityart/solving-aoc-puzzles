/* Solution for part 2 of day 03 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/3 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define N_LINES 3
#define ERROR '0'

char common_char(char lines[][MAX_LINE_LENGTH], int n_lines) {
    /* Returns the character that is common to all of the n_lines lines.

       Note: I did not optimize this function.

     */
    char c;
    int i, j, common;
    for (j = 0; (c = lines[0][j]) != '\0'; j++) {
        common = TRUE;
        for (i = 1; i < n_lines; i++) {
            common = common && strchr(lines[i], c) != NULL;
        }
        if (common) return c;
    }
    return ERROR;
}

int value(char c) {
    /* Returns the priority value associated with given character.

       Note: here we use a platform-dependent implementation, because we assume
       that the characters a-z are contiguous and in that order in the
       platform's character set (same assumption for the characters A-Z).

     */
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 1;
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 27;
    else
        return ERROR;
}

int main() {
    int eof, i, sum = 0, empty;
    char lines[N_LINES][MAX_LINE_LENGTH];
    do {
        empty = TRUE;
        for (i = 0; i < N_LINES; i++) {
            if (!readline(lines[i], MAX_LINE_LENGTH, &eof)) {
                printf("Could not read line\n");
                return FALSE;
            }
            empty = empty && strlen(lines[i]) == 0;
        }
        if (!empty)
            sum += value(common_char(lines, N_LINES));
    } while (!eof);
    printf("%d\n", sum);
    return TRUE;
}
