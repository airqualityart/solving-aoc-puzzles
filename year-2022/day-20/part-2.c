/* Solution for part 2 of day 20 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/20 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 10
#define MAX_NUMBERS 5000
#define DECRYPTION_KEY 811589153
#define N_MIXINGS 10
#define ITH_X 1000
#define ITH_Y 2000
#define ITH_Z 3000

typedef struct struct_number {
    long int value;
    struct struct_number *prev;
    struct struct_number *next;
} Number;

static Number numbers[MAX_NUMBERS];
static int n_numbers = 0, i0;

void parse_input(void) {
    /* Parse input data and set values of all static variables. */
    int eof = FALSE;
    char line[MAX_LINE_LENGTH];
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (n_numbers >= MAX_NUMBERS)
            error_exit("Not enough room in array.");
        else if (sscanf(line, "%ld", &numbers[n_numbers].value) != 1)
            error_exit("Could not parse number.");
        if (numbers[n_numbers].value == 0) i0 = n_numbers;
        numbers[n_numbers++].value *= DECRYPTION_KEY;
    }
}

void link_numbers(void) {
    /* Link numbers together (ie. set values of prev and next). */
    int i;
    for (i = 1; i < n_numbers-1; i++) {
        numbers[i].prev = numbers + (i-1);
        numbers[i].next = numbers + (i+1);
    }
    numbers[0].prev = numbers + (n_numbers-1);
    numbers[0].next = numbers + 1;
    numbers[n_numbers-1].prev = numbers + (n_numbers-2);
    numbers[n_numbers-1].next = numbers;
}

void mix_numbers(void) {
    /* Mix numbers. */
    long int i, n;
    Number *number, *before;
    for (number = numbers; number < numbers + n_numbers; number++) {
        if (number->value == 0) continue;
        /* Remove number from the sequence */
        (number->prev)->next = number->next;
        (number->next)->prev = number->prev;
        /* Find its new spot */
        n = (number->value*(number->value < 0 ? -1 : 1)) % (n_numbers-1);
        for (i = 0, before = number; i < n; i++)
            before = (number->value > 0) ? before->next : before->prev;
        if (number->value < 0) before = before->prev;
        /* Insert it there */
        number->prev = before;
        number->next = before->next;
        before->next = before->next->prev = number;
    }
}

int main(void) {
    /* Print the sum of the grove's coordinates. */
    int i, n;
    long int sum = 0;
    Number *current;
    parse_input();
    link_numbers();
    for (i = 0; i < N_MIXINGS; i++) mix_numbers();
    n = max2i(ITH_X, max2i(ITH_Y, ITH_Z));
    for (i = 1, current = numbers + i0; i <= n; i++) {
        current = current->next;
        if (i == ITH_X || i == ITH_Y || i == ITH_Z)
            sum += current->value;
    }
    printf("%ld\n", sum);
    return TRUE;
}
