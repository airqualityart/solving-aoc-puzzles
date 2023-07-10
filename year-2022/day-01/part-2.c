/* Solution for part 2 of day 01 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/1 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 10
#define N_TOP_CARRIERS 3

void update_top_carriers(int new_value, int old_values[], int nvalues) {
    /* Replace the minimum value in old_values by new_value if the latter is
       greater than the minimum. */
    int min, imin;
    mini(old_values, nvalues, &min, &imin);
    if (new_value > min) old_values[imin] = new_value;
}

int main(void) {
    int number, sum = 0, top_carriers[N_TOP_CARRIERS], eof, i;
    char line[MAX_LINE_LENGTH];
    for (i = 0; i < N_TOP_CARRIERS; i++) top_carriers[i] = 0;
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof)) {
            printf("Could not read line\n");
            return FALSE;
        } else if (strlen(line) == 0) {
            update_top_carriers(sum, top_carriers, N_TOP_CARRIERS);
            sum = 0;
        } else if (sscanf(line, "%d", &number) == 1)
            sum += number;
        else {
            printf("Could not convert line\n");
            return FALSE;
        }
    } while (!eof);
    printf("%d\n", sumi(top_carriers, N_TOP_CARRIERS));
    return TRUE;
}
