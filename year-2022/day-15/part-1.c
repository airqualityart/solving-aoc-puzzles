/* Solution for part 1 of day 15 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/15 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define MAX_BEACONS 100
#define TEMPLATE_INPUT "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d"

typedef struct struct_range {
    int start;
    int end;
    struct struct_range *next;
} Range;

static int beacons[MAX_BEACONS], ibeacon = 0;

Range *new_range(int start, int end, Range *next) {
    /* Create new range with given boundaries and next value. */
    Range *range;
    if ((range = (Range*)malloc(sizeof(Range))) == NULL)
        error_exit("Could not allocate memory.");
    range->start = start;
    range->end = end;
    range->next = next;
    return range;
}

void add_range(int start, int end, Range **ranges) {
    /* Update given linked list of existing ranges with new range. */
    Range *range;
    if (*ranges == NULL || end < (*ranges)->start)
        *ranges = new_range(start, end, *ranges);
    else if (start > (*ranges)->end)
        add_range(start, end, &((*ranges)->next));
    else {
        /* If we reach this point then we need to fuse the new interval into
           existing one(s) */
        (*ranges)->start = min2i(start, (*ranges)->start);
        while ((*ranges)->next != NULL && end >= (*ranges)->next->start) {
            (*ranges)->end = (*ranges)->next->end;
            range = (*ranges)->next;
            (*ranges)->next = range->next;
            free(range);
        }
        (*ranges)->end = max2i(end, (*ranges)->end);
    }
}

int dist(int x1, int y1, int x2, int y2) {
    /* Return the Manhattan distance between point (x1, y1) and (x2, y2). */
    return abs(x1-x2) + abs(y1-y2);
}

void add_beacon(int x) {
    /* Add beacon (x-coordinate given by x) to the list of beacons. */
    if (ibeacon >= MAX_BEACONS)
        error_exit("Not enough room for beacons.");
    beacons[ibeacon++] = x;
}

Range *create_list_of_ranges(void) {
    /* Create list of intervals where soundings say beacons cannot be.

       This function also fills out variable "beacons", which keeps track of
       the beacons that are located on the target line.

     */
    int eof = FALSE, ytarget, xs, ys, xb, yb, delta;
    char line[MAX_LINE_LENGTH];
    Range *ranges=NULL;
    getlinex(line, MAX_LINE_LENGTH, &eof);
    if (sscanf(line, "%d", &ytarget) != 1)
        error_exit("Could not get target y value.");
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (sscanf(line, TEMPLATE_INPUT, &xs, &ys, &xb, &yb) != 4)
            error_exit("Could not parse line.");
        delta = dist(xs, ys, xb, yb) - dist(xs, ys, xs, ytarget);
        if (delta >= 0)
            add_range(xs-delta, xs+delta, &ranges);
        if (yb == ytarget && !int_in_array(xb, beacons, ibeacon))
            add_beacon(xb);
    }
    return ranges;
}

void free_ranges(Range *ranges) {
    /* Free the given linked-list of ranges. */
    if (ranges == NULL)
        return;
    free_ranges(ranges->next);
    free(ranges);
}

int main(void) {
    /* Print number of positions along target line where beacons cannot be.

       NB: the y-coordinate of target line is read from the first line of input
           data.

     */
    int i, n = 0;
    Range *ranges=create_list_of_ranges(), *range;
    range = ranges;
    while (range != NULL) {
        n += range->end - range->start + 1;
        for (i = 0; i < ibeacon; i++)
            if (beacons[i] >= range->start && beacons[i] <= range->end)
                n -= 1;
        range = range->next;
    }
    free_ranges(ranges);
    printf("%d\n", n);
    return TRUE;
}
