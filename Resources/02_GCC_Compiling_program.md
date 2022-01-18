



---
```
title:  Compiling C program (Linking and Loading)
author: Sukhjit Singh Sehra
---
```



## Compiling a simple C program

> #include  <stdio.h> 
>
> int main (void) 
>
> { 
>
> printf ("Hello, world!\n");
>
>  return 0; 
>
> }

To compile the file ‘hello.c’ with gcc, use the following command:

> gcc -o hello hello.c

---



## Compiling multiple source files: 

1. In the following example we will split up the program Hello World into three files: ‘main.c’, ‘hello_fn.c’ and the header file ‘hello.h’. Here is the main program ‘main.c’: 

   >  #include "hello.h"
   >
   >  int main (void) 
   >
   > { 
   >
   > hello ("world"); 
   >
   > return 0; 
   >
   > }

2. The declaration in ‘hello.h’ is a single line specifying the prototype of the function hello:

   >  void hello (const char * name);

3. The definition of the function hello itself is contained in the file ‘hello_fn.c’: 

   > \#include <stdio.h> 
   >
   >  #include "hello.h" 
   >
   > void hello (const char * name) 
   >
   > {
   >
   >  printf ("Hello, %s!\n", name); 
   >
   > }

4. $ gcc -Wall main.c hello_fn.c -o newhello

---



## Compiling files independently:

1. The command-line option ‘-c’ is used to compile a source file to an object file. For example, the following command will compile the source file ‘main.c’ to an object file: 

   >  $ gcc -Wall -c main.c

2. This produces the object file ‘hello_fn.o’.

   > $ gcc -Wall -c hello_fn.c 

4.  Creating executables from object files: 

   >  $ gcc main.o hello_fn.o -o hello

---

## Link order of object files

The object file which contains the definition of a function should appear after any files which call that function. In this case, the file ‘hello_fn.o’ containing the function hello should be specified after ‘main.o’ itself, since main calls hello:

>  $ gcc main.o hello_fn.o -o hello (correct order) 

With some compilers or linkers the opposite ordering would result in an error, 

> $ gcc hello_fn.o main.o -o hello (incorrect order) 
>
> main.o: In function ‘main’: main.o(.text+0xf): undefined reference to ‘hello’

---



##  Linking with external libraries: 

To avoid the need to specify long paths on the command line, the compiler provides a short-cut option ‘-l’ for linking against libraries.

 For example, the following command:

>  $ gcc -Wall calc.c -lm -o calc 

is equivalent to:

> $ gcc -Wall calc.c /usr/lib/libm.a -o calc

Here The library ‘libm.a’ contains object files for all the mathematical functions, such as sin, cos, exp, log and sqrt. The linker searches through these to find the object file containing the sqrt function. 

In general, the compiler option ‘-lNAME’ will attempt to link object files with a library file ‘libNAME.a’ in the standard library directories.