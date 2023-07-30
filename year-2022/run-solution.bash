#!/bin/bash
#
# This script compiles and runs the solution to puzzle $1, part $2.
#
# Copyright (c) 2023-now, Air Quality And Related Topics.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# (BSD "3-clause" license):
#
#   (1) Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.
#
#   (2) Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the distribution.
#
#   (3) The name of the author may not be used to endorse or promote products
#   derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# ----------------------
# How to use this script
# ----------------------
#
# For example, to run part 2 of the ninth day's puzzle:
#
# ./run-solution.bash 09 2
#
# Optionally, you can pass a third argument to specify the C compiler to use
# (either gcc or clang, the default being gcc). For example:
#
# ./run-solution.bash 09 2 clang
#
# Before running the solution on the actual puzzle input, this script first
# runs the solution on the test data, if any. Test data are files named:
#
# day-$1/part-$2_test-data-*.txt
#
# For each of the test files:
# - the first line contains the answer to the given test input data.
# - the rest of the file is the test input data.

# Useful function(s)
function message {
    # Pretty-print first argument, and exit with error code if given.
    #
    # Usage:
    #
    # message "some text" [error_code]
    echo ""
    echo ">>> $1"
    echo ""
    [[ $# > 1 ]] && exit $2
}

# Quality checks on inputs
if [ $# != 2 ] && [ $# != 3 ] ; then
    message "You must provide either two or three arguments." 1
fi

# Compiler and its options (we choose to be very strict with the code)
COMPILER="gcc"
[ $# = 3 ] && COMPILER=$3
case $COMPILER in
    gcc)
        FLAGS="-ansi -Wall -Wextra -Wpedantic -Werror -pedantic-errors";;
    clang)
        FLAGS="-ansi";;
    *)
        message "Unknown or unsupported compiler: $COMPILER." 2;;
esac
if [ $COMPILER = gcc ] && [ $(/usr/bin/uname -s) = Darwin ] ; then
    # On MacOS, the default gcc binary (/usr/bin/gcc) is in fact the clang
    # compiler. We use gcc from Homebrew instead
    COMPILER=$(ls -1 /opt/homebrew/bin/gcc-* | grep "^/opt/homebrew/bin/gcc-[0-9][0-9]*$")
else
    COMPILER=$(which $COMPILER)
fi
LIBS="-lm"
message "Using compiler: $COMPILER"

# Directories and files
DIR_WORK="."
DIR_SRC="$DIR_WORK/day-$1"
DIR_INC="$DIR_WORK/commons"
FILE_PFX="part-$2"
FILE_SRC="$DIR_SRC/$FILE_PFX.c"
FILE_EXE="$DIR_SRC/$FILE_PFX.out"
FILE_INPUT="$DIR_SRC/${FILE_PFX}_input-data.txt"

# Compile
$COMPILER $FLAGS -I$DIR_INC $FILE_SRC $DIR_INC/commons.c $LIBS -o $FILE_EXE
if [ $? != 0 ] ; then
    message "There were errors during compilation." 3
else
    message "Compiled successfully, running program..."
fi

# Run the tests
message "Running the solution on test cases..."
for F in $DIR_SRC/${FILE_PFX}_test-data-*.txt; do
    if [ $F == "$DIR_SRC/${FILE_PFX}_test-data-*.txt" ]; then break; fi
    ANSWER_REAL=$(head -n 1 $F)
    ANSWER_CALC=$(awk 'NR > 1' $F | $FILE_EXE)
    if [ $ANSWER_CALC == $ANSWER_REAL ]; then
        echo "    $F: OK"
    else
        echo "    $F: test not passed ($ANSWER_CALC instead of $ANSWER_REAL)"
    fi
done
echo ""

# Run the solution on the actual puzzle input
message "Running the solution on the actual puzzle input data..."
if [ -f $FILE_INPUT ]; then
    ANSWER=$($FILE_EXE < $FILE_INPUT)
    echo "    Answer = $ANSWER"
else
    echo "    Could not find the input data file"
fi
echo ""
