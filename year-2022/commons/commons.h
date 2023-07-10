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

#define TRUE 1
#define FALSE 0

void error_exit(char message[]);
/* Print given message then exit with error code. */

int readline(char line[], int nmaxchar, int* eof);
void getlinex(char line[], int nmaxchar, int* eof);
int getlinen(char line[], int nmaxchar, int* eof);
/* Read line from standard input.

   IN:
   - nmaxchar: maximum length of the array line, including null character. Must
     be greater than one (unexpected behavior may happen otherwise).

   OUT:
   - line: the line read from standard input (truncated if necessary). These
     functions do NOT copy the end of line character ('\n'), but they do add
     the null character ('\0') at the end of the line (even if it were
     truncated).
   - eof: TRUE iff the end of file has been reached while reading the line.

   The function readline returns TRUE iff the line could be read without being
   truncated (and FALSE otherwise), whereas getlinex exits with error if the
   line could not be read without being truncated. The function getlinen works
   like getlinex but it returns the string's length.
*/

int prepend_c_to_string(char c, char s[], int nmaxchar);
int append_c_to_string(char c, char s[], int nmaxchar);
/* Add character c to the beginning or end of character string s.

   IN:
   - c: the character that must be added to s.
   - nmaxchar: maximum number of elements in the character array s, including
     null character.

   OUT:
   - s: the updated character string.

   RETURN: TRUE iff the character could be added successfully.
 */

int min2i(int i, int j);
int max2i(int i, int j);
/* Return the minimum or maximum of i and j. */

void mini(int vector[], int nvalues, int *min, int* index);
void maxi(int vector[], int nvalues, int *max, int* index);
/* Seek the minimum or maximum value (and its index) within a vector.

   IN:
   - vector: array of values to search.
   - nvalues: number of values in vector.

   OUT:
   - min/max: value of the minimum/maximum.
   - index: index of the minimum/maximum.

   If there are more than one minima/maxima, these functions return the first
   one.
 */

int sumi(int vector[], int nvalues);
/* Return the sum of vector, which contains nvalues. */

int digittoi(char digit);
/* Return given digit as an integer (exit with error if it is not a digit). */

int parse_ints(char* s, char sep, int array[], int nmax);
/* Parse the list of integers described by character string s and fill out
   array accordingly.

   IN:
   - s and sep: s is a pointer to a valid, but possibly empty, character string
     that represents a sep-separated list of integers. If the separator is not
     a spacing character (space, tab, newline, etc.), then the separator must
     not be repeated between numbers, and there must not be a separator before
     the first number or after the last one. If the separator is a spacing
     character, then it can be repeated between numbers and it can be present
     before the first number and after the last one. In both cases, spacing
     around separators is ignored. The following examples are valid for s:
         "10, 20, 30, 40, 50" (sep = ',')
         " 10 ; 20 ; 30 ; 40 ; 50 " (sep = ';')
         "10 \n 20 \n 30 \n 40 \n 50 \n" (sep = '\n')
     - nmax: the maximum number of values that 'array' can hold.

    OUT:
    - array: the array into which the parsed integers are put.

    RETURN: the number of values added to array.
*/
