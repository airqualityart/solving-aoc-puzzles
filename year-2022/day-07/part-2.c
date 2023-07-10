/* Solution for part 2 of day 07 of Advent of Code (r) 2022.

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

 - See https://adventofcode.com/2022/day/7 for the description of the puzzle.

*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "commons.h"

#define MAX_STR_LENGTH 200
#define MAX_DIR 1000
#define ERROR -1
#define DISK_CAPACITY 70000000
#define SPACE_NEEDED 30000000

void cd_dot_dot(char dirname[]) {
    /* Update dirname so that it becomes its parent directory. If called on the
       root directory ("/"), returns en empty character string.

       For example: "/home/user/" becomes "/home/".
    */
    int n = strlen(dirname);
    if (n == 0)
        return;
    else if (n == 1 && dirname[0] == '/')
        dirname[0] = '\0';
    else {
        for (n = n-2; n >= 0 && dirname[n] != '/'; n--)
            ;
        dirname[n+1] = '\0';
    }
}

int process_cd(char line[MAX_STR_LENGTH], char wd[MAX_STR_LENGTH]) {
    /* Process given cd command and update the working directory
       accordingly. Returns TRUE iff successful. */
    if (line[5] == '/')
        strcpy(wd, line+5);
    else if (strcmp(line+5, "..") == 0 && strcmp(wd, "/") != 0)
        cd_dot_dot(wd);
    else if (strlen(wd) + strlen(line+5) + 2 <= MAX_STR_LENGTH) {
        strcat(wd, line+5);
        strcat(wd, "/");
    } else
        return FALSE;
    return TRUE;
}

int index_of_dir(char dirname[MAX_STR_LENGTH],
                 char dirnames[MAX_DIR][MAX_STR_LENGTH]) {
    /* Returns index of given directory in the list of directories (append the
       directory to the list if not already there). */
    int i;
    for (i = 0; i < MAX_DIR && strlen(dirnames[i]) > 0; i++)
        if (strcmp(dirname, dirnames[i]) == 0) return i;
    if (i == MAX_DIR) return ERROR;
    strcpy(dirnames[i], dirname);
    return i;
}

int add_size_of_file(char line[MAX_STR_LENGTH],
                     char wd[MAX_STR_LENGTH],
                     char dirnames[MAX_DIR][MAX_STR_LENGTH],
                     int sizes[MAX_DIR]) {
    /* Add size of file to size of current directory and all its
       parents. Returns TRUE iff successful. */
    char dirname[MAX_STR_LENGTH];
    int size, i;
    strcpy(dirname, wd);
    sscanf(line, "%d", &size);
    while (strlen(dirname) > 0) {
        if ((i = index_of_dir(dirname, dirnames)) == ERROR) return FALSE;
        sizes[i] += size;
        cd_dot_dot(dirname);
    }
    return TRUE;
}

int main(void) {
    /*

      Since we are exploring a file system listing, we could use some kind of
      tree to represent the data. This approach seems overkill though, in light
      of what is asked of us. Here, we use the following method, which seems
      more straight-forward to me:

      - we keep track of which directory we are in (wd = working directory).

      - when we encounter a file, we add its size to the size of the working
        directory, as well as to the size of each of its parent directories.

      We make the assumption that we browsed the file system intelligently,
      meaning that we did not list the same directory more than once. This
      assumption is verified by the code below.

    */
    int eof, i, n_listed = 0;
    char line[MAX_STR_LENGTH];
    char wd[MAX_STR_LENGTH] = "";
    char dirnames[MAX_DIR][MAX_STR_LENGTH], listed[MAX_DIR][MAX_STR_LENGTH];
    int sizes[MAX_DIR];
    int space_used, need_to_free, deleted_size = DISK_CAPACITY;

    /* Initialize arrays  */
    for (i = 0; i < MAX_DIR; i++) {
        dirnames[i][0] = '\0';
        listed[i][0] = '\0';
        sizes[i] = 0;
    }

    do {

        if (!readline(line, MAX_STR_LENGTH, &eof)) {
            printf("Could not read line\n");
            return FALSE;
        }

        if (strlen(line) == 0) {
            continue;

        } else if (strncmp(line, "$ cd ", 5) == 0) {
            /* We found a cd command, so we update the current directory  */
            if (!process_cd(line, wd)) {
                printf("Could not parse cd command: %s\n", line);
                return FALSE;
            }

        } else if (isdigit(line[0])) {
            /* We found a file */
            if (!add_size_of_file(line, wd, dirnames, sizes)) {
                printf("Could not process file listing: %s\n", line);
                return FALSE;
            }

        } else if (strncmp(line, "dir ", 4) == 0) {
            continue;

        } else if (strcmp(line, "$ ls") == 0) {
            /* We make sure we have not listed the same directory twice */
            if (index_of_dir(wd, listed) != n_listed) {
                printf("Directory already listed: %s\n", wd);
                return FALSE;
            }
            n_listed++;

        } else {
            printf("Cannot parse line: %s\n", line);
            return FALSE;
        }

    } while (!eof);

    /* We are looking for the smallest directory that can be deleted to give us
       enough free space */

    space_used = sizes[index_of_dir(strcpy(wd, "/"), dirnames)];
    need_to_free = SPACE_NEEDED - (DISK_CAPACITY - space_used);
    for (i = 0; i < MAX_DIR && strlen(dirnames[i]) > 0; i++)
        if (sizes[i] >= need_to_free && sizes[i] < deleted_size)
            deleted_size = sizes[i];
    printf("%d\n", deleted_size);

    return TRUE;

}
