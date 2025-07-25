# sol

Programming Language

Eventually want to make a programming language, have had some trouble staying
motivated to make real progress on it.

In this repo I want to try developing a programming language by making
end-to-end programs from the beginning rather than working by e.g. creating
a scanner then a parser then type checking etc.

Currently you can write extremely basic programs, and a signed integer and
boolean type are supported. You can do basic operations and print out results.

e.g.

```
a := 1 + 1
b := 3

a = a + b

PRINT a
PRINT a < b
```

(Should print 5, false on separate lines, currently bugged)

I want to develop if, while statements nexts. To support that, I will add a
notion of scoped to the language e.g. the following should work:

```
a := 1

block
    a := 2

    PRINT a
end block

PRINT a
```

should print 1, 2
