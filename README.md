# avdv-point-group
Write an executable that calculates the point group operations of an arbitrary 2D lattice.
Lattice vectors must be inputted through a file, which contains the *a* and *b* vectors as columns.
Your program should print out the size of the point group, followed by the Cartesian matrix representations for each operation.

The `Eigen` library is provided as a submodule, so be sure to clone the repository properly and make use of it!
It provides linear algebra routines that you will need.

## Example
A file named `triangular.txt` contains the lattice vectors for a triangular lattice:
```
1.0        -0.5      
0.0         0.8660254
```

The program to find the point group has been compiled as `avdv-find-point-group`.
Running
```
avdv-find-point-group triangular.txt
```
outputs (truncated):
```
The 12 operations of the lattice are:


 1.0 0.0
 0.0 1.0
 ------------------

 0.5       -0.8660254
 0.8660254  0.5      
 ------------------

-0.5       -0.8660254
 0.8660254 -0.5      
 ------------------

-1.0  0.0
 0.0 -1.0
 ------------------

-0.5        0.8660254
-0.8660254 -0.5      
 ------------------

 0.5        0.8660254
-0.8660254  0.5      
 ------------------

 1.0  0.0
 0.0 -1.0
 ------------------
...

```

## Requirements
- [x] There was an attempt
- [x] Written in c++
- [x] Final product available through git
- [x] Provide a Makefile or bash script that will compile your program
- [x] Works for any 2D lattice, inputted as a file
- [x] Prints number of operations in point group
- [x] Prints the matrix of each point group operation
- [x] Written by you (no sharing code)

## Bonus points
- [x] Well structured code (e.g. delegating tasks to functions or classes)
- [x] Documentation, or better yet, readable code
- [x] `const` correctness
- [x] Use STL
- [x] Small, incremental commits, not one jumbo commit at the end!
- [x] Prove results are correct by showing group is closed
- [ ] Print multiplication table of point group

## Pitfalls to look out for
* Floating point comparisons, think well before using `==` operator
* Undefined behavior because you forgot to return the value at the end of your function
* You don't have the `Eigen` library in because you forgot to clone with `--recursive`
* Your compiler can't find the `Eigen` headers because you don't know what preprocessor flags are
