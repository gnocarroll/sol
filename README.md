# sol

Programming Language

Eventually want to make a programming language, have had some trouble staying
motivated to make real progress on it.

In this repo I want to try developing a programming language by making
end-to-end programs from the beginning rather than working by e.g. creating
a scanner then a parser then type checking etc.

Will begin by working on a C++ program that will take expressions as input
on stdin and then, treating each line as a separate expression, will print
the results on separate lines.

e.g.

With input of:

```
1
1 + 1
3 * 3
```

Output should be:

```
1
2
9
```

Currently, this basic functionality seems to be working.

Next will move on to basic script with integer variables and PRINT function.

```
a := 1 + 1
b := 3

a = a + b

PRINT a
```

Should print 5. Once I get going may rework syntax a little
