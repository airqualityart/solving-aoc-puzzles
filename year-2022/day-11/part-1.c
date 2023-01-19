/* Solution for part 1 of day 11 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/11 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define MAX_MONKEYS 10
#define MAX_ITEMS 50
#define N_ROUNDS 20
#define USE_OLD -1

typedef struct {
    int index;
    int items[MAX_ITEMS];
    int n_items;
    char operator;
    int operand;
    int divisible_by;
    int throw[2];
    int n_inspected;
} Monkey;

static Monkey monkeys[MAX_MONKEYS];
static int n_monkeys = 0;

void parse_monkeys(void) {
    /* Parse monkeys and update array of monkeys accordingly.

    ** For simplicity, we assume that the input is perfectly formatted. **

    */
    int eof = FALSE;
    char line[MAX_LINE_LENGTH];
    Monkey *mk = monkeys;
    while (!eof) {
        getline(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (n_monkeys >= MAX_MONKEYS)
            error_exit("There is not enough room for all monkeys.");
        sscanf(line+7, "%d", &mk->index);
        getline(line, MAX_LINE_LENGTH, &eof);
        mk->n_items = parse_ints(line+18, ',', mk->items, MAX_ITEMS);
        getline(line, MAX_LINE_LENGTH, &eof);
        if (sscanf(line+23, "%c %d", &mk->operator, &mk->operand) == 1)
            mk->operand = USE_OLD;
        getline(line, MAX_LINE_LENGTH, &eof);
        sscanf(line+21, "%d", &mk->divisible_by);
        getline(line, MAX_LINE_LENGTH, &eof);
        sscanf(line+29, "%d", mk->throw+1);
        getline(line, MAX_LINE_LENGTH, &eof);
        sscanf(line+30, "%d", mk->throw);
        mk->n_inspected = 0;
        n_monkeys++;
        mk++;
    }
}

void monkey_throws(Monkey *mk) {
    /* Make given monkey throw all of its items. */
    Monkey *target;
    int *item, value;
    for (item = mk->items; mk->n_items > 0; mk->n_items--, item++) {
        value = (mk->operand == USE_OLD) ? *item : mk->operand;
        if (mk->operator == '+')
            value = *item + value;
        else if (mk->operator == '*')
            value = *item * value;
        else
            error_exit("Unknown operator.");
        mk->n_inspected++;
        value /= 3;
        target = monkeys + mk->throw[value % mk->divisible_by == 0];
        if (target->n_items >= MAX_ITEMS)
            error_exit("Target monkey has no more room for items.");
        target->items[target->n_items++] = value;
    }
}

void play_round(void) {
    /* Simulate one round of throwing objects, update monkeys accordingly. */
    int i;
    for (i = 0; i < n_monkeys; i++)
        monkey_throws(monkeys+i);
}

int main() {
    /* Print the product of the numbers of items inspected by the top two
       busiest monkeys. */
    int i, first_max = -1, second_max = -2;
    parse_monkeys();
    for (i = 0; i < N_ROUNDS; i++)
        play_round();
    for (i = 0; i < n_monkeys; i++)
        if (monkeys[i].n_inspected >= first_max) {
            second_max = first_max;
            first_max = monkeys[i].n_inspected;
        }
        else if (monkeys[i].n_inspected > second_max)
            second_max = monkeys[i].n_inspected;
    printf("%d\n", first_max * second_max);
    return TRUE;
}
