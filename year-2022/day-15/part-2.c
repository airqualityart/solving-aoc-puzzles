/* Solution for part 2 of day 15 of Advent of Code (r) 2022.

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
#define TEMPLATE_INPUT "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d"

typedef struct struct_range {
    int start;
    int end;
    struct struct_range *next;
} Range;

Range *new_range(int start, int end, Range *next) {
    /* Create new range with given boundaries and next value. */
    Range *range;
    if ((range = (Range*)malloc(sizeof(Range))) == NULL)
        error_exit("Could not allocate memory for individual range.");
    range->start = start;
    range->end = end;
    range->next = next;
    return range;
}

void add_range(int start, int end, Range **ranges) {
    /* Update given linked list of existing ranges with new range. */
    Range *range;
    if (*ranges == NULL || end < (*ranges)->start-1)
        *ranges = new_range(start, end, *ranges);
    else if (start > (*ranges)->end+1)
        add_range(start, end, &((*ranges)->next));
    else {
        /* If we reach this point then we need to fuse the new interval into
           existing one(s) */
        (*ranges)->start = min2i(start, (*ranges)->start);
        while ((*ranges)->next != NULL && end >= (*ranges)->next->start-1) {
            (*ranges)->end = (*ranges)->next->end;
            range = (*ranges)->next;
            (*ranges)->next = range->next;
            free(range);
        }
        (*ranges)->end = max2i(end, (*ranges)->end);
    }
}

int dist(int x1, int y1, int x2, int y2) {
    /* Return the Manhattan distance between points (x1, y1) and (x2, y2). */
    return abs(x1-x2) + abs(y1-y2);
}

int find_distress_location(Range *ranges, int xmax) {
    /* Return distress beacon x-location if in row, negative number otherwise.

       This function assumes that there is at most 1 possible location in row.

     */
    if (ranges->start == 0 && ranges->end == xmax)
        return -1;
    else if (ranges->start == 1)
        return 0;
    else if (ranges->end == xmax-1)
        return xmax;
    else
        return ranges->end + 1;
}

void free_ranges(Range *ranges) {
    /* Free the given linked list of ranges. */
    if (ranges == NULL)
        return;
    free_ranges(ranges->next);
    free(ranges);
}

int main(void) {
    /* Print the tuning frequency of the distress beacon.

       NB:

       - The extent of the search space (xmax, ymax) is read from the first line
         of input data.

       - Here "ranges" is an array whose elements represent rows of the search
         space, each row being represented as a linked list of ranges.

     */
    int eof = FALSE, xmax, ymax, xs, ys, xb, yb, i, d, delta;
    char line[MAX_LINE_LENGTH];
    Range **ranges;

    /* Read extent of search space from input and allocate array of ranges */
    getlinex(line, MAX_LINE_LENGTH, &eof);
    if (sscanf(line, "%d %d", &xmax, &ymax) != 2)
        error_exit("Could not get the extent of the search space.");
    if ((ranges = (Range**)calloc(ymax+1, sizeof(Range*))) == NULL)
        error_exit("Could not allocate memory for array of ranges.");
    for (i = 0; i <= ymax; i++) ranges[i] = NULL;

    /* Create the linked list of ranges for each row */
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        else if (sscanf(line, TEMPLATE_INPUT, &xs, &ys, &xb, &yb) != 4)
            error_exit("Could not parse line.");
        d = dist(xs, ys, xb, yb);
        for (i = max2i(ys-d, 0); i <= min2i(ys+d, ymax) ; i++) {
            delta = d - dist(xs, ys, xs, i);
            add_range(max2i(xs-delta, 0), min2i(xs+delta, xmax), ranges+i);
        }
    }

    /* Find the location of the distress beacon (free memory as we go) */
    xb = -1;
    for (i = 0; i <= ymax; i++) {
        if (xb < 0 && (xb = find_distress_location(ranges[i], xmax)) >= 0)
            yb = i;
        free_ranges(ranges[i]);
    }
    free(ranges);

    /* Compute and show the distress beacon's tuning frequency (warning: it can
       be a very large number, hence the cast to long int) */
    printf("%ld\n", xb*(long int)4000000 + yb);
    return TRUE;

}
