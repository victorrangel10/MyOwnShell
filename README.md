MSH
===
Msh is a simple Unix Shell implementation written in C.

It abstracts the idea of how a shell works in 4 steps, which are:read, parse, fork, exec, and wait.
Since its built under a learning purpose, its scope is limited and there are some restrictions to usability:

* Commands must be on a single line.
* Arguments must be separated by whitespace.
* No quoting arguments or escaping whitespace.
* No piping or redirection.
* Only builtins are: `cd`, `help`, `exit`.

Running
-------

Use `gcc -o msh shell.c` to compile, and then `./msh` and you will be good to go.

Credits
--------
Msh is actually my own implementation of LSH, a shell tutorial article that can be found at [Stephen Brennan's website](https://brennan.io/2015/01/16/write-a-shell-in-c/). Check it out yourself!

