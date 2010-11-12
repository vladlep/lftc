#include <stdio.h>
#include "alex.h"
//http://aplawrence.com/Linux/c_compiling_linux.html
//http://www.cs.caltech.edu/courses/cs11/material/c/mike/misc/compiling_c.html
/*
important. Nu includ headerul si in main ca da eror de duplicare.
gcc -c alex.c

 gcc -c main_prg.c

 gcc -lm -o main main_prg.o alex.o

*/
int main(int argc, char *argv[])
{
   
    openFile("in.txt");
    sapareteAtoms();
    //printf("%c",getCharUrm());
    closeFile();
    printfAtoms();
    //checkProject(first);
   return 0;
}

