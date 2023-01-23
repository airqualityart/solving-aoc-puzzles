/* Common ressources for my solutions to Advent of Code (r) 2022 puzzles.

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

*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "commons.h"

void error_exit(char message[]) {
    fprintf(stderr, "\n\nExiting with error (%s)...\n\n", message);
    exit(EXIT_FAILURE);
}

int readline(char line[], int nmaxchar, int* eof) {
    int c, i;
    for (i = 0; i < nmaxchar && (c = getchar()) != '\n' && c != EOF; i++)
        line[i] = c;
    *eof = (c == EOF);
    if (i == nmaxchar) {
        line[i-1] = '\0';
        return FALSE;
    } else {
        line[i] = '\0';
        return TRUE;
    }
}

void getline(char line[], int nmaxchar, int* eof) {
    if (!readline(line, nmaxchar, eof))
        error_exit("Could not read entire line.");
}

int getlinen(char line[], int nmaxchar, int* eof) {
    getline(line, nmaxchar, eof);
    return strlen(line);
}

int prepend_c_to_string(char c, char s[], int nmaxchar) {
    int n = strlen(s), i;
    if (nmaxchar <= 1 || n == nmaxchar-1) return FALSE;
    for (i = n; i >= 0; i--) s[i+1] = s[i];
    s[0] = c;
    return TRUE;
}

int append_c_to_string(char c, char s[], int nmaxchar) {
    int n = strlen(s);
    if (nmaxchar <= 1 || n == nmaxchar-1) return FALSE;
    s[n] = c;
    s[n+1] = '\0';
    return TRUE;
}

void mini(int vector[], int nvalues, int *min, int* index) {
    int i;
    *index = 0;
    *min = vector[0];
    for (i = 1; i < nvalues; i++)
        if (vector[i] < *min) {
            *min = vector[i];
            *index = i;
        }
}

void maxi(int vector[], int nvalues, int *max, int* index) {
    int i;
    *index = 0;
    *max = vector[0];
    for (i = 1; i < nvalues; i++)
        if (vector[i] > *max) {
            *max = vector[i];
            *index = i;
        }
}

int sumi(int vector[], int nvalues) {
    int i, sum = 0;
    for (i = 0; i < nvalues; i++) sum += vector[i];
    return sum;
}

int digittoi(char digit) {
    char digit_str[2] = {'.', '\0'};
    if (!isdigit(digit))
        error_exit("This is not a valid digit.");
    digit_str[0] = digit;
    return atoi(digit_str);
}

int parse_ints(char* s, char sep, int array[], int nmax) {
    int sepspace = isspace(sep);
    while (isspace(*s))
        s++;
    if (*s == '\0')
        return 0;
    if (nmax < 1)
        error_exit("Not enough room in the array.");
    array[0] = 0;
    do
        array[0] = array[0]*10 + digittoi(*s);
    while (isdigit(*++s));
    while (isspace(*s) && (!sepspace || *s != sep))
        s++;
    if (*s == sep) {
        s++;
        while (isspace(*s))
            s++;
        if (!sepspace && *s == '\0')
            error_exit("Expecting number after separator.");
    } else if (*s != '\0')
        error_exit("Unexpected character.");
    return 1 + parse_ints(s, sep, ++array, nmax-1);
}
