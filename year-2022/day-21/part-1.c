/* Solution for part 1 of day 21 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/21 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define MAX_MONKEYS 2000
#define MAX_NAME_LENGTH 4

typedef struct struct_monkey {
    char name[MAX_NAME_LENGTH+1];
    int has_value;
    long int value;
    char operator;
    char monkey1[MAX_NAME_LENGTH+1];
    char monkey2[MAX_NAME_LENGTH+1];
} Monkey;

static Monkey monkeys[MAX_MONKEYS];
static int n_monkeys = 0;

int compare_monkeys(const void *mk1, const void *mk2) {
    /* Monkey comparison function for qsort and bsearch. */
    return strcmp(((Monkey*)mk1)->name, ((Monkey*)mk2)->name);
}

void parse_input(void) {
    /* Parse input data and set array of monkeys, sorted by name. */
    int eof = FALSE;
    char line[MAX_LINE_LENGTH], *c, *subline;
    Monkey *mk = monkeys;
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0) continue;
        if (++n_monkeys > MAX_MONKEYS) error_exit("Not enough room in array.");
        if ((c = strchr(line, ':')) - line > MAX_NAME_LENGTH)
            error_exit("Monkey's name is too long.");
        line[c++ - line] = '\0';
        strcpy(mk->name, line);
        mk->has_value = (sscanf((subline = ++c), "%ld", &mk->value) == 1);
        if (! mk->has_value) {
            if ((c = strchr(subline, ' ')) - subline > MAX_NAME_LENGTH)
                error_exit("Monkey #1's name is too long.");
            subline[c++ - subline] = '\0';
            strcpy(mk->monkey1, subline);
            mk->operator = *(c++);
            if (strlen(++c) > MAX_NAME_LENGTH)
                error_exit("Monkey #2's name is too long.");
            strcpy(mk->monkey2, c);
        }
        mk++;
    }
    qsort(monkeys, n_monkeys, sizeof(Monkey), *compare_monkeys);
}

Monkey* find_monkey(char name[MAX_NAME_LENGTH+1]) {
    /* Return pointer to monkey given its name. */
    Monkey mk;
    strcpy(mk.name, name);
    return (Monkey*)bsearch(&mk, monkeys, n_monkeys,
                            sizeof(Monkey), *compare_monkeys);
}

long int calc_monkey(char name[MAX_NAME_LENGTH+1]) {
    /* Return value of monkey given its name (calculate it if needed). */
    Monkey *mk = find_monkey(name);
    long int v1, v2;
    if (! mk->has_value) {
        v1 = calc_monkey(mk->monkey1);
        v2 = calc_monkey(mk->monkey2);
        switch (mk->operator) {
        case '+':
            mk->value = v1 + v2; break;
        case '-':
            mk->value = v1 - v2; break;
        case '*':
            mk->value = v1 * v2; break;
        case '/':
            mk->value = v1 / v2; break;
        default:
            error_exit("Unknown operator."); break;
        }
        mk->has_value = TRUE;
    }
    return mk->value;
}

int main(void) {
    /* Print the number yelled by the root monkey. */
    parse_input();
    printf("%ld\n", calc_monkey("root"));
    return TRUE;
}
