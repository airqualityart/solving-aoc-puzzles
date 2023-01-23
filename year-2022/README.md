I had not used C in a long time, so I decided to use this language to solve the
2022 puzzles.

Notes:

* My solutions read the puzzles' inputs from stdin. The script
  [run-solution.bash](https://github.com/airqualityart/solving-aoc-puzzles/blob/main/year-2022/run-solution.bash)
  takes care of compiling the code and running the corresponding binary while
  feeding it the proper input.

* To keep the solutions as short as reasonably possible, I try to keep
  defensive programming to a minimum. For example, the solutions rely on the
  assumption that the input data files provided by _Advent of Code_ are well
  formed.

* Starting with day 09's puzzle, I started using static external variables for
  the data that must be accessed by several functions, instead of passing these
  data as function arguments. Why? Because I feel that this approah is more
  KISS in this case.

* Here are the 2022 puzzles that I found most interesting, along with the
  specific programming concepts or techniques they involved (which exlain why I
  found the puzzles interesting):

  - day 11, part 2:

     * being aware of issues related to overflow and inexactitude of floating
       point calculations (this issue may or may not arise depending on the
       programming language, compiler, and host platform).

     * modulo arithmetic.

  - day 12: optimal path-finding.

  - day 13, part 1: nested linked lists (at least that is the data
    representation I chose), and recursion.
