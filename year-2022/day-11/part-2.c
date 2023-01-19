/* Solution for part 2 of day 11 of Advent of Code (r) 2022.

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
#define N_ROUNDS 10000
#define USE_OLD -1

/*

   In this part of the puzzle, the worry values become too large to be
   represented as any of the int types (the values do not even fit in unsigned
   long ints). Using variables of type double or long double is not an option
   either, because we need exact arithmetic (for example, with doubles, if "a"
   is a very large positive integer and if "b" is a very small positive
   integer, it is possible for "a + b" to yield "a" as a result).

   But in fact, we do not need to keep track of the actual worry values, we
   just need to know whether they are divisible by the monkeys' "test
   values". So we just need to keep track of the worry values' reminders in the
   Euclidean divisions with the monkeys' test values, and to perform
   appropriate modulo arithmetic when monkeys inspect items.

 */

typedef struct {
    int index;
    int items[MAX_ITEMS][MAX_MONKEYS];
    int n_items;
    char operator;
    int operand;
    int divisible_by;
    int throw[2];
    unsigned long int n_inspected;
} Monkey;

static Monkey monkeys[MAX_MONKEYS];
static int n_monkeys = 0;

void parse_monkeys(void) {
    /* Parse monkeys and update array of monkeys accordingly.

    ** For simplicity, we assume that the input is perfectly formatted. **

    */
    int eof = FALSE, items[MAX_ITEMS], i, j;
    char line[MAX_LINE_LENGTH];
    Monkey *mk = monkeys;
    while (!eof) {
        getline(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (n_monkeys >= MAX_MONKEYS)
            error_exit("there is not enough room for all monkeys");
        sscanf(line+7, "%d", &mk->index);
        if (mk->index != n_monkeys)
            error_exit("Bad monkey index in input data.");
        getline(line, MAX_LINE_LENGTH, &eof);
        mk->n_items = parse_ints(line+18, ',', items, MAX_ITEMS);
        /* For now, we store the actual worry values in all the spots; the
           simplified values will be calculated later */
        for (i = 0; i < mk->n_items; i++)
            for (j = 0; j < MAX_MONKEYS; j++)
                mk->items[i][j] = items[i];
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
    /* Calculate all the initial simplified worry values */
    for (mk = monkeys; mk < monkeys+n_monkeys; mk++)
        for (i = 0; i < mk->n_items; i++)
            for (j = 0; j < n_monkeys; j++)
                mk->items[i][j] %= monkeys[j].divisible_by;
}

void copy_item(int from[], int to[]) {
    /* Copy an item into another. */
    int i;
    for (i = 0; i < n_monkeys; i++)
        to[i] = from[i];
}

void update_item(int *item, char operator, int operand) {
    /* Update item according to given operator and operand. */
    int i, value;
    for (i = 0; i < n_monkeys; i++) {
        value = (operand == USE_OLD) ? item[i] : operand;
        if (operator == '+')
            item[i] += value;
        else if (operator == '*')
            item[i] *= value;
        else
            error_exit("Unknown operator.");
        item[i] %= monkeys[i].divisible_by;
    }
}

void monkey_throws(Monkey *mk) {
    /* Make given monkey throw all of its items. */
    Monkey *target;
    int i;
    for (i = 0; mk->n_items > 0; mk->n_items--, i++) {
        update_item(mk->items[i], mk->operator, mk->operand);
        mk->n_inspected++;
        target = monkeys + mk->throw[mk->items[i][mk->index] == 0];
        if (target->n_items >= MAX_ITEMS)
            error_exit("Target monkey has no more room for items.");
        copy_item(mk->items[i], target->items[target->n_items++]);
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
    int i;
    unsigned long int first_max = 0, second_max = 0;
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
    printf("%lu\n", first_max * second_max);
    return TRUE;
}
