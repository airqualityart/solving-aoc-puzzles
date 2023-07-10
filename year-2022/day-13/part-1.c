/* Solution for part 1 of day 13 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/13 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_LINE_LENGTH 300
#define RIGHT_ORDER -1
#define SAME_ORDER 0
#define WRONG_ORDER 1

typedef struct struct_packet {
    int islist;
    int value;
    struct struct_packet *values;
    struct struct_packet *next;
} Packet;

Packet *alloc_packet(void) {
    /* Allocate new packet (exit with error if allocation fails). */
    Packet *packet;
    if ((packet = (Packet*)malloc(sizeof(Packet))) == NULL)
        error_exit("Could not allocate memory");
    return packet;
}

Packet *parse_packet(char *s) {
    /* Parse character string into packet, and return pointer to the result. */
    int count;
    char *p, substr[MAX_LINE_LENGTH];
    Packet *packet;
    if (*s == '\0')
        return NULL;
    packet = alloc_packet();
    if ((packet->islist = (*s == '['))) {
        for (p = s+1, count = 0; *p != ']' || count != 0; p++)
            count += (*p == '[') - (*p == ']');
        strncpy(substr, s+1, p-s-1);
        substr[p-s-1] = '\0';
        packet->values = parse_packet(substr);
        s = p + 1;
    } else if (isdigit(*s))
        for (packet->value = 0; isdigit(*s); s++)
            packet->value = packet->value*10 + digittoi(*s);
    else
        error_exit("unexpected character");
    packet->next = parse_packet(s + (*s == ','));
    return packet;
}

void free_packet(Packet *packet) {
    /* De-allocate entire packet. */
    Packet *p, *next;
    if (packet->islist)
        for (p = packet->values; p != NULL; p = next) {
            next = p->next;
            free_packet(p);
        }
    free(packet);
}

Packet *listify(Packet *packet) {
    /* Transform given packet that represents an integer into a packet that
       represents a list containing only this integer, and returns the
       (unmodified) pointer to said packet. */
    packet->islist = TRUE;
    packet->values = alloc_packet();
    packet->values->islist = FALSE;
    packet->values->value = packet->value;
    packet->values->next = NULL;
    return packet;
}

int order(Packet *left, Packet *right) {
    /* Compare packets and return one of SAME_ORDER, RIGHT_ORDER, or
       WRONG_ORDER accordingly. */
    int ord;
    if (left == NULL && right == NULL)
        return SAME_ORDER;
    else if (left == NULL)
        return RIGHT_ORDER;
    else if (right == NULL)
        return WRONG_ORDER;
    else if (left->islist && right->islist)
        if ((ord = order(left->values, right->values)) == SAME_ORDER)
            return order(left->next, right->next);
        else
            return ord;
    else if (!left->islist && !right->islist)
        if (left->value == right->value)
            return order(left->next, right->next);
        else
            return (left->value < right->value) ? RIGHT_ORDER : WRONG_ORDER;
    else if (!left->islist)
        return order(listify(left), right);
    else
        return order(left, listify(right));
}

int main(void) {
    /* Print sum of the indices of all properly-ordered pairs of packets. */
    int eof = FALSE, sum = 0, index = 0;
    char line[MAX_LINE_LENGTH];
    Packet *left, *right;
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        index++;
        left = parse_packet(line);
        getlinex(line, MAX_LINE_LENGTH, &eof);
        right = parse_packet(line);
        sum += (order(left, right) == RIGHT_ORDER) ? index : 0;
        free_packet(left);
        free_packet(right);
    }
    printf("%d\n", sum);
    return TRUE;
}
