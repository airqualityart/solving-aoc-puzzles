/* Solution for part 2 of day 02 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/2 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 4
#define ROCK 0
#define PAPER 1
#define SCISSORS 2
#define LOSS 'X'
#define DRAW 'Y'
#define WIN 'Z'
#define SHAPE_ERROR -1

int shapechar2int(char shape) {
    /* Converts shape from 'A', 'B', 'C', 'X', 'Y', 'Z' to integer. */
    switch (shape) {
    case 'A':
        return ROCK;
    case 'B':
        return PAPER;
    case 'C':
        return SCISSORS;
    default:
        return SHAPE_ERROR;
    }
}

int shape_score(int shape) {
    /* Returns the score corresponding to given shape. */
    switch (shape) {
    case ROCK:
        return 1;
    case PAPER:
        return 2;
    case SCISSORS:
        return 3;
    default:
        return SHAPE_ERROR;
    }
}

int score_player1(int p1, int p2) {
    /* Returns the score of player 1 given both players' shapes. */
    int score = shape_score(p1);
    if (p1 == p2) {
        score += 3;
    } else if ((p1 == ROCK && p2 == SCISSORS) ||
               (p1 == PAPER && p2 == ROCK) ||
               (p1 == SCISSORS && p2 == PAPER)) {
        score += 6;
    }
    return score;
}

int predict_shape(int shape, int outcome) {
    /* Returns shape that other player has to play to lead to given outcome. */
    if (outcome == DRAW)
        return shape;
    else if (outcome != LOSS && outcome != WIN)
        return SHAPE_ERROR;
    else if (shape == ROCK)
        return (outcome == LOSS) ? SCISSORS : PAPER;
    else if (shape == PAPER)
        return (outcome == LOSS) ? ROCK : SCISSORS;
    else if (shape == SCISSORS)
        return (outcome == LOSS) ? PAPER : ROCK;
    else
        return SHAPE_ERROR;
}

int main() {
    int eof, shape1, shape2, score = 0;
    char line[MAX_LINE_LENGTH], shape2char, outcome;
    do {
        if (!readline(line, MAX_LINE_LENGTH, &eof)) {
            printf("Could not read line\n");
            return FALSE;
        }
        if (strlen(line) == 0) continue;
        sscanf(line, "%c %c", &shape2char, &outcome);
        shape2 = shapechar2int(shape2char);
        shape1 = predict_shape(shape2, outcome);
        if (shape1 == SHAPE_ERROR || shape2 == SHAPE_ERROR) {
            printf("Could not convert shape to integer\n");
            return FALSE;
        }
        score += score_player1(shape1, shape2);
    } while (!eof);
    printf("%d\n", score);
    return TRUE;
}
