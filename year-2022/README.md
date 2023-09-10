I had not used C in a long time, so I decided to use this language to solve the
2022 puzzles.

Notes:

* My solutions read the puzzles' inputs from stdin. The script
  [run-solution.bash](https://github.com/airqualityart/solving-aoc-puzzles/blob/main/year-2022/run-solution.bash)
  takes care of compiling the code and running the corresponding binary while
  feeding it the proper input.

* To keep the solutions as short as reasonably possible, I try to keep defensive
  programming to a minimum. For example, the solutions rely on the assumption
  that the input data files provided by _Advent of Code_ are well formed and
  that there is always a unique solution to each problem. I mostly use defensing
  programming when parsing input data (I cannot stop myself to do so).

* Starting with day 09's puzzle, I started using static external variables for
  the data that must be accessed by several functions, instead of passing these
  data as function arguments. Why? Because I feel that this approah is more
  KISS in this case.

* Here are the 2022 puzzles that I found most interesting, along with the
  specific programming concepts or techniques they involved or that I chose to
  use:

  - day 11, part 2:

     * being aware of issues related to overflow and inexactitude of floating
       point calculations (this issue may or may not arise depending on the
       programming language, compiler, and host platform).

     * modulo arithmetic.

  - day 12: optimal path-finding.

  - day 13, part 1: nested linked lists and recursion.

  - day 15: part 1: ordered linked lists with insertions and merges.

  - day 16:

     * part 1: recursion and shortest path.

     * part 2: combinatorics.

* Here are basic style guidelines that I aim to follow:

  - Indentations are 4 spaces, no tabs.

  - Maximum 80 characters per line.

  - It is okay to write one-liner if statements only if there is no `else if` or
    `else` clause.
