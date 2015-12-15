/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           main.c
    Created by:     Fritts and Slayter
    Last edit on:   12/4/2013 (Slayter)
    Description:    takes command line arguments from the user, initializes static
                    memory being dynamically set, runs the main program in a loop.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "nsh6_gid.h"
#include "fileIO.h"
#include "dataParsing.h"
#include "memoryHandler.h"
#include "ListFunctions.h"


int main(int argc, char **argv)
{
    init_masterControl(&masterControl);
    init_ll();
    // set path var
    setPathVar();

    // process resource file
    openResourceFile();

    if(argc > 1)    //if data is comming from the command line.
    {
        printf("Called %s from the command line: (data below)\n", argv[1]);
        
        FILE *fp;
        fp = fopen(argv[1], "r"); // attempt to open first argument

        if (fp) {
            processScript(fp);    // process file
        } else {
            int i;
            i = execvp(argv[1], argv+1); // run external program
            if (i == -1)                 // if -1 is returned
                printf("Error\n");       // the program failed to run
        }
    }
    else
    {
        while(1)
        {
            getUserInput();     //gets the user input.
        }
    }
    return 0;
}
