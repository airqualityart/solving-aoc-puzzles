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

int readline(char line[], int nmaxchar, int* eof);
/* Reads line from standard input and returns TRUE iff it is successful.

   IN:
   - nmaxchar: maximum length of the array line, including null character. Must
     be greater than one (unexpected behavior may happen otherwise).

   OUT:
   - line: the line read from standard input (truncated if necessary). This
     function does NOT copy the end of line character ('\n'), but it does add
     the null character ('\0') at the end of the line.
   - eof: TRUE iff the end of file has been reached while reading the line.

   RETURNS: TRUE iff the line could be read without being truncated.
*/

int prepend_c_to_string(char c, char s[], int nmaxchar);
int append_c_to_string(char c, char s[], int nmaxchar);
/* Adds character c to the beginning or end of character string s.

   IN:
   - c: the character that must be added to s.
   - nmaxchar: maximum number of elements in the character array s, including
     null character.

   OUT:
   - s: the updated character string.

   RETURNS: TRUE iff the character could be added successfully.
 */

void mini(int vector[], int nvalues, int *min, int* index);
void maxi(int vector[], int nvalues, int *max, int* index);
/* Seeks the minimum or maximum value (and its index) within a vector.

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
/* Returns the sum of vector, which contains nvalues. */
