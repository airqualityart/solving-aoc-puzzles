/* Solution for part 1 of day 03 of Advent of Code (r) 2022.

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
#define ERROR '0'

char repeated_char(char s[]) {
    /* Returns the character that is common to both the first and the second
       half of given character string. */
    int i, j, n = strlen(s), half = n/2;
    for (i = 0; i < half; i++) {
        for (j = half; j < n; j++) {
            if (s[i] == s[j]) {
                return s[i];
            }
        }
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

int main(void) {
    int eof, sum = 0;
    char line[MAX_LINE_LENGTH];
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof)) {
            printf("Could not read line\n");
            return FALSE;
        }
        if (strlen(line) != 0)
            sum += value(repeated_char(line));
    } while (!eof);
    printf("%d\n", sum);
    return TRUE;
}
