/* Solution for part 2 of day 16 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/16 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include "commons.h"

#define MAX_LINE_LENGTH 100
#define TEMPLATE_INPUT "Valve %s has flow rate=%d;"
#define NCHAR_VALVE_NAME 2
#define MAX_VALVES 50
#define MAX_CONNECTIONS 5
#define MAX_COMBINATIONS 10000
#define VALVE_NOT_FOUND -1
#define START_LOCATION "AA"
#define TIMER 26

typedef struct struct_valve {
    char name[NCHAR_VALVE_NAME+1];
    int flowrate;
    int n_conn;
    char conn_names[MAX_CONNECTIONS][NCHAR_VALVE_NAME+1];
    int conn_idx[MAX_CONNECTIONS];
} Valve;

static int n_valves = 0;
static Valve valves[MAX_VALVES];
static int distances[MAX_VALVES][MAX_VALVES];
static int combinations[MAX_COMBINATIONS][MAX_VALVES/2];

int valve_index(char *name) {
    /* Return index of given valve in array "valves" (or VALVE_NOT_FOUND). */
    int i;
    for (i = 0; i < n_valves; i++)
        if (strcmp(valves[i].name, name) == 0)
            return i;
    return VALVE_NOT_FOUND;
}

void parse_input() {
    /* Parse input data and populate array "valves" accordingly. */
    int eof = FALSE, i;
    char line[MAX_LINE_LENGTH], *c;
    Valve *valve = valves;
    while (!eof) {
        getlinex(line, MAX_LINE_LENGTH, &eof);
        if (strlen(line) == 0)
            continue;
        /* Define new valve */
        if (valve == valves + MAX_VALVES)
            error_exit("Not enough room in array for new valve.");
        if (sscanf(line, TEMPLATE_INPUT, valve->name, &valve->flowrate) != 2)
            error_exit("Could not parse line.");
        if (valve_index(valve->name) != VALVE_NOT_FOUND)
            error_exit("Valve is already defined.");
        /* Parse the valves' connections */
        valve->n_conn = 0;
        if ((c = strchr(line, ',')) == NULL)
            c = strrchr(line, ' ') + 1;
        else
            c -= NCHAR_VALVE_NAME;
        while (*c != '\0') {
            if (valve->n_conn >= MAX_CONNECTIONS)
                error_exit("Not enough room in array for new connection.");
            strncpy(valve->conn_names[valve->n_conn], c, NCHAR_VALVE_NAME);
            valve->conn_names[valve->n_conn][NCHAR_VALVE_NAME] = '\0';
            valve->n_conn++;
            c += NCHAR_VALVE_NAME;
            while (*c == ',' || *c == ' ') c++;
        }
        /* On to the next valve... */
        n_valves++;
        valve++;
    }
    /* Now that all lines have been parsed, we can set the connection indices */
    for (valve=valves; valve < valves+n_valves; valve++)
        for (i = 0; i < valve->n_conn; i++)
            valve->conn_idx[i] = valve_index(valve->conn_names[i]);
}

int shortest_distance(int from, int to) {
    /* Return the length of the shortest path between the two given valves.

       For simplicity, this function assumes that such a path does exist.

     */
    int conn, i, head = 0, tail = 1, dist = -1;
    int queue_idx[MAX_VALVES], queue_dist[MAX_VALVES];
    Valve *valve;
    if (from == to) return 0;
    queue_idx[head] = from;
    queue_dist[head] = 0;
    while (dist == -1) {
        valve = valves + queue_idx[head];
        for (i = 0; i < valve->n_conn; i++) {
            if ((conn = valve->conn_idx[i]) == to) {
                dist = queue_dist[head] + 1;
                break;
            }
            if (int_in_array(conn, queue_idx, tail)) continue;
            queue_idx[tail] = conn;
            queue_dist[tail++] = queue_dist[head] + 1;
        }
        head++;
    }
    return dist;
}

void fill_distances() {
    /* Fill out array of shortest distances between pairs of valves.

       For efficiency reasons, we only calculate the distances that will be
       useful to us in the rest of the code.

     */
    int i, j, useful, start = valve_index(START_LOCATION);
    for (i = 0; i < n_valves; i++) {
        distances[i][i] = 0;
        for (j = 0; j < i; j++) {
            useful = (i == start && valves[j].flowrate > 0) ||
                     (j == start && valves[i].flowrate > 0) ||
                     (valves[i].flowrate > 0 && valves[j].flowrate > 0);
            if (useful)
                distances[i][j] = distances[j][i] = shortest_distance(i, j);
        }
    }
}

int fill_combinations(int n, int *array, int m, int row, int col) {
    /* Determine all ways of choosing n values among array of m values (m >= n).

       Order is not relevant e.g. choice (1,2,3) is equivalent to (2,1,3). In
       each combination, each element of the array can be chosen only once.

       This function writes the results into array "combinations" (one
       combination per line) and returns the number of such combinations.

       Parameters "row" and "col" are used for recursive calls and should be
       equal to 0 when this function is called non-recursively.

     */
    int i, j, added, total = 0;
    for (i = 0; i <= m-n; i++) {
        if ((added = n) > 1)
            added = fill_combinations(n-1, array+i+1, m-i-1, row, col+1);
        for (j = 0; j < added; j++) {
            if (row >= MAX_COMBINATIONS)
                error_exit("Not enough room in combination array.");
            combinations[row++][col] = array[i];
        }
        total += added;
    }
    return total;
}

int calc_max_flow_1player(int location, int *open_valves, int time_left) {
    /* Calculate maximum possible flow from given state (1 player only). */
    int flow = 0, new_flow, i, n;
    if (time_left < 2) return 0;
    for (i = 0; i < n_valves; i++) {
        if (open_valves[i] || valves[i].flowrate == 0)
            continue;
        if ((n = time_left - distances[location][i] - 1) > 0) {
            open_valves[i] = TRUE;
            new_flow = valves[i].flowrate * n;
            new_flow += calc_max_flow_1player(i, open_valves, n);
            open_valves[i] = FALSE;
            flow = max2i(flow, new_flow);
        }
    }
    return flow;
}

int calc_max_flow_2players() {
    /* Calculate maximum possible flow for two players. */
    int n, n_comb, i, j, valve, n_useful = 0;
    int start = valve_index(START_LOCATION), new_flow, flow = 0;
    int useful_valves[MAX_VALVES], valves1[MAX_VALVES], valves2[MAX_VALVES];
    for (i = 0; i < n_valves; i++) {
        valves1[i] = valves2[i] = TRUE;
        if (valves[i].flowrate > 0)
            useful_valves[n_useful++] = i;
    }
    /* We attribute some valves to player 1, the others to player 2, and we try
       all such possible combinations (we only work with useful valves) */
    for (n = 0; n <= n_useful/2; n++) {
        n_comb = fill_combinations(n, useful_valves, n_useful, 0, 0);
        for (i = 0; i < n_comb; i++) {
            for (j = 0; j < n_useful; j++) {
                valve = useful_valves[j];
                valves1[valve] = int_in_array(valve, combinations[i], n);
                valves2[valve] = ! valves1[valve];
            }
            new_flow = calc_max_flow_1player(start, valves1, TIMER) +
                       calc_max_flow_1player(start, valves2, TIMER);
            flow = max2i(flow, new_flow);
        }
    }
    return flow;
}

int main(void) {
    /* Print the maximum flow that we can obtain in given time (2 players). */
    parse_input();
    fill_distances();
    printf("%d\n", calc_max_flow_2players());
    return TRUE;
}
