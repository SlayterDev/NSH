/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           fileIO.c
    Created by:     Fritts and Slayter
    Last edit on:   12/04/2013 (Slayter)
	Description:	Functions for reading files, finding paths.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <pthread.h>
#include "ListFunctions.h"
#include "memoryHandler.h"
#include "defs.h"
#include "nsh6_gid.h"
#include "dataParsing.h"
#include "fileIO.h"


void setPathVar(void)
{
    struct passwd *pw = getpwuid(getuid());
    char *pathName = (char *)nshMalloc(5); // hard coded because why not
    strcpy(pathName, "Path");
    nshInsert(&envHead, pathName, pw->pw_dir, envVarCount);
    envVarCount++;
}

// user input through files
void openResourceFile(void)
{
    char *pathName = "Path";
    char *filePathPtr = nshFind(envHead, pathName); // get ptr of home dir

    char *filePath = (char *)nshMalloc(strlen(filePathPtr)+1);
    strcpy(filePath, filePathPtr);

    char *fileName = "/.nshrc";
    strcat(filePath, fileName);                     // append resource file name

    FILE *fp;
    fp = fopen(filePath, "r");                      // attempt open

    if (fp)
        processScript(fp);                          // process file
    else
        printf("Error opening %s\n", filePath);
}

void processScript(FILE *fp) { // fp already open
    char givenCommand[USER_INPUT_SIZE] = {'\0'}; // string for input
    while(fgets(givenCommand, USER_INPUT_SIZE, fp) != NULL) { // read file line by line til EOF
        if(*(givenCommand+0) == '\n')   //fixes seg fault for lines with no characters.
            continue;
        //printf("nsh4> %s", givenCommand);
        tokenizeGivenStream(givenCommand);
    }

    fclose(fp);
}

void runBackgroundProcess(char **givenData) {
    DEBUG printf("executeExternalCommand() called\n");

    int i=0, j=0; // return values for system cmds
    char tempData[100] = {0};
    char *dataHere = tempData;
    strcpy(tempData,givenData[0]);

    char *complexStringStart = strchr(tempData,'{');
    char *complexStringEnd = strchr(tempData,'}');

    //check that they are both present
        //and set complexString array to the complex string within the {}
    if(complexStringStart != NULL && complexStringEnd != NULL)
    {
        if(complexStringStart < complexStringEnd)   //verify that they are in the right order
        {
            complexStringStart++;
            dataHere = complexStringStart;
            *(complexStringEnd) = 0;
        }
    }

    DEBUG printf("Given : %s\n",dataHere);
    
    char **commandSplit;             //pointer for split command

    commandSplit = (char **)nshMalloc(sizeof(char *)*20);
    //commandSplit[0] = (char *)nshMalloc(6);
    //strcpy(commandSplit[0], "nohup");

    char *temp = strtok(dataHere, " ");         //temp pointer for tok
    int cmdChunks = 0;

    int z;
    for(z=1;z<8;z++)
    {
        while (temp != NULL)                         //split the char array command into sperate chunks
        {
            commandSplit[cmdChunks] = (char *)nshMalloc(strlen(temp)+1);
            strcpy(commandSplit[cmdChunks++], temp);
            temp = strtok (NULL, " ");
        }
        temp = givenData[z];
    }

    DEBUG printf("After TOK : %s, %s, %s, %s, %s\n", commandSplit[0], commandSplit[1], commandSplit[2],commandSplit[3],commandSplit[4]);

    // ****** Begin new thread code *********
    // commandSplit has commands

    int threadResult;
    pthread_t theThread;
    threadResult = pthread_create(&theThread, NULL, threadCall, commandSplit);

    if (threadResult != 0) {
        printf("Failed to create background process.\n");
    }
}

void *threadCall(void *givenCmds) {
    char **givenData = givenCmds;

    //fclose(stdout);

    DEBUG printf("execvp(%s,double pointer to givenData)\n",givenData[0]);

    int i = -1;
    /*while (givenData[++i] != NULL);
    givenData[i] = (char *)nshMalloc(2);
    strcpy(givenData[i], "&");
    givenData[++i] = NULL;*/

    DEBUG printf("Cmds to pass %s, %s, %s, %s, %s\n", givenData[0], givenData[1], givenData[2], givenData[3], givenData[4]);

    i = fork();
    if (i == 0) {
        fclose(stdout);
        setpgid(0, 0);

        DEBUG printf("Still displaying output\n");

        i = execvp(givenData[0], givenData);

        if (i == -1) {
            exit(1);
        }
    } else {
        //int j;
        //waitpid(i, &j, WNOHANG);
        return NULL;
    }

    return NULL;
}

// run external command in child process
void executeExternalCommand(char **givenData)
{
	DEBUG printf("executeExternalCommand() called\n");

    int i=0, j=0; // return values for system cmds
    char tempData[100] = {0};
    char *dataHere = tempData;
    strcpy(tempData,givenData[0]);

    char *complexStringStart = strchr(tempData,'{');
    char *complexStringEnd = strchr(tempData,'}');

    //check that they are both present
        //and set complexString array to the complex string within the {}
    if(complexStringStart != NULL && complexStringEnd != NULL)
    {
        if(complexStringStart < complexStringEnd)   //verify that they are in the right order
        {
        	complexStringStart++;
        	dataHere = complexStringStart;
            *(complexStringEnd) = 0;
        }
    }

    DEBUG printf("Given : %s\n",dataHere);
    
    char *commandSplit[20] = {NULL};             //pointer for split command
    char *temp = strtok(dataHere, " ");         //temp pointer for tok
    int cmdChunks = 0;

    int z;
    for(z=1;z<8;z++)
    {
	    while (temp != NULL)                         //split the char array command into sperate chunks
	    {
	        commandSplit[cmdChunks++] = temp;
	        temp = strtok (NULL, " ");
	    }
	    temp = givenData[z];
	}

    DEBUG printf("After TOK : %s, %s, %s, %s, %s\n", commandSplit[0], commandSplit[1], commandSplit[2],commandSplit[3],commandSplit[4]);

    i = fork(); // fork the process
    if (i == 0)
    {
    	DEBUG printf("execvp(%s,double pointer to givenData)\n",givenData[0]);
        i = execvp(commandSplit[0], commandSplit); // attempt to execute cmd

        if (i == -1) { // command failed to rum
            printf("%s failed to run\n", commandSplit[0]);
            exit(1);
        }
    } else {
        j = wait(NULL); // wait for child to finish
    }
}