/* Solution for part 2 of day 08 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/8 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "commons.h"

#define MAX_COLS 150
#define MAX_ROWS 150
#define OUT_OF_GRID -1

void initialize_tree_grid(int trees[MAX_ROWS][MAX_COLS]) {
    /* Initializes the grid of trees. */
    int i, j;
    for (i = 0; i < MAX_ROWS; i++)
        for (j = 0; j < MAX_COLS; j++)
            trees[i][j] = OUT_OF_GRID;
}

int read_tree_grid(int trees[MAX_ROWS][MAX_COLS], int* n_rows, int* n_cols) {
    /* Reads the grid of trees and returns TRUE iff successful. Arguments
       n_rows and n_cols are set to the actual numbers of trees per row and per
       column, respectively. */
    int eof, *pos;
    char line[MAX_COLS], digit[2] = "0";
    initialize_tree_grid(trees);
    *n_rows = 0;
    do {
        if (!readline(line, MAX_COLS, &eof)) return FALSE;
        if (strlen(line) == 0) continue;
        pos = trees[(*n_rows)++];
        for (*n_cols = 0; line[*n_cols] != '\0'; (*n_cols)++) {
            *pos++ = atoi(strncpy(digit, line+*n_cols, 1));
        }
    } while (!eof);
    return TRUE;
}

int scenic_score(int trees[MAX_ROWS][MAX_COLS], int i, int j,
                 int n_rows, int n_cols) {
    /* Returns the scenic score of tree (i,j). */
    int height = trees[i][j], k;
    int score_left = 0, score_right = 0, score_up = 0, score_down = 0;

    /* Look left */
    for (k = j-1; k >= 0 && trees[i][k] < height; k--)
        score_left++;
    if (k >= 0) score_left++;

    /* Look right */
    for (k = j+1; k < n_cols && trees[i][k] < height; k++)
        score_right++;
    if (k < n_cols) score_right++;

    /* Look up */
    for (k = i-1; k >= 0 && trees[k][j] < height; k--)
        score_up++;
    if (k >= 0) score_up++;

    /* Look down */
    for (k = i+1; k < n_rows && trees[k][j] < height; k++)
        score_down++;
    if (k < n_rows) score_down++;

    /* Final score */
    return score_left * score_right * score_up * score_down;
}

int main(void) {
    int i, j, n_rows, n_cols, score, max_score = 0;
    int trees[MAX_ROWS][MAX_COLS];
    if (!read_tree_grid(trees, &n_rows, &n_cols)) {
        printf("Could not read the grid of trees.\n");
        return FALSE;
    }
    for (i = 0; i < n_rows; i++)
        for (j = 0; j < n_cols; j++) {
            score = scenic_score(trees, i, j, n_rows, n_cols);
            if (score > max_score) max_score = score;
        }
    printf("%d\n", max_score);
    return TRUE;
}
