# BrainFuck-Interpreter
BrainFuck Interpreter and Compiler


After cloning the repo run 

```
make
```

to build the interpreter/compiler. Once done you can interpret BF programs with

```
./bf i program.b
```

where `program.b` is the name of your program. To compile you can run

```
./bf c program.b ; gcc -O3 a.c ; ./a.out
```

The bf compiler translates BF code into C and you can use a C compiler to create an executable.