/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           nsh5_gid.c
    Created by:     Fritts and Slayter
    Last edit on:   12/01/2013 (Fritts)
    Description:    Functions that perform:
                        -getting data from a file or user and running these commands
                        -callback functions for these commands
                        -parsing data given by the user
                        -helper functions for making parsing data easier.
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
#include "nsh6_gid.h"
#include "ListFunctions.h"
#include "memoryHandler.h"
#include "dataParsing.h"
#include "defs.h"



/*Defines.                                            -------------------------------------------*/

/*A pair of command and callback for a given command.*/
command_L1_t userCommands[] = {\
    {"set",&userCommand_L1_set},\
    {"tes",&userCommand_L1_tes},\
    {"alias",&userCommand_L1_alias},\
    {"saila",&userCommand_L1_saila},\
    {"exit",&userCommand_L1_exit},\
    {"help",&userCommand_L1_help},\
    {"echo",&userCommand_L1_echo},\
    {"where",&userCommand_L1_where},\
    {NULL,&userCommand_L1_exit}\
    };

//used for complexCommand temp data
simpleComplexString_t scs = {0};
simpleComplexString_t scsEmpty = {0};




/*Callbacks                                           -------------------------------------------*/

// user gives echo command.
void userCommand_L1_echo(char **givenData) {
    int i = -1;
    while (givenData[i++] != NULL) {                           // cycle through data and print
        if (givenData[i] != NULL) {
            givenData[i] = extendEnvironmentVar(givenData[i]); // replace with env var value if needed
            printf("%s ", givenData[i]);
        }
    }
    printf("\n");
}

//user gives the set command.
void userCommand_L1_set(char **givenData)
{
    //Display all environment variables
    if(givenData[0] == NULL)
    {
         //var_alias_ll_print_list(&ll_var);
        nshDisplayAll(envHead);

    }
    //Display a variable (User gave "set bla" and "bla" was passed to this function.)
    else if(givenData[0] != NULL && givenData[1] == NULL)
    {

        //search for variable and print
        char *varValuePtr = nshFind(envHead, givenData[0]);
        if(NULL == varValuePtr)
        {
            printf("fail.\n");
        }
        else
        {
            printf("\n%s,%s\n",givenData[0],varValuePtr);
        }
    }
    else
    {
        //see if a complex command was sent.
        complexCommand(givenData+1);

        if(scs.cs_present)  //if complex command is present.
            givenData[1] = scs.cs;  //set the pointer to point now to the {asdf}

        //try to find if it was already added
        char *varValuePtr = nshFind(envHead, givenData[0]);

		givenData[1] = extendEnvironmentVar(givenData[1]);
        if(NULL == varValuePtr) //not already added
        {
            //not already in list
            nshInsert(&envHead, givenData[0], givenData[1], envVarCount);
            envVarCount++;
        }
            //already added so add to or whatever is needed
        else
        {
            //already in list (delete and remake)

            //nshUpdate(envHead, givenData[0], givenData[1]);
			nshRemove(&envHead, givenData[0], false);
			envVarCount--;
			nshInsert(&envHead, givenData[0], givenData[1], envVarCount);
			envVarCount++;
        }
    }
}

//user gives the where command.
// "Where type name" so "type name" is passed in here.
void userCommand_L1_where(char **givenData)
{

    if(givenData[0] == NULL || givenData[1] == NULL){return;}
    char *type = givenData[0];
    char *name = givenData[1];

    // check for alias
    char *aliasVal = nshFindAlias(cmdHead, name);
    if (aliasVal != NULL) // alias found
        strcpy(name, aliasVal);

    char *pathVar = nshFind(envHead, "Path"); // get path var
    int i = getNumberOfValues(pathVar);       // get number of values in path var

    char **pathsToSearch = (char **)nshMalloc(sizeof(char*) * i); // malloc memory for paths array
    int j;
    for (j = 0; j < i; j++)
        pathsToSearch[j] = (char *)nshMalloc(100);


    pathsToSearch = tokenizeEnvVar(pathVar, pathsToSearch); // fill paths array

    struct stat fileStats;
    int status = -1;
    i = -1;
    char *temp;
    bool permissionFound = false;
    while (status != 0 && (strcmp(name, "alias") != 0)) { // check for alias because it exists in /usr/bin already
        if (pathsToSearch[++i]) {
            temp = (char *)nshMalloc(strlen(pathsToSearch[i])+1); // copy path to search
            strcpy(temp, pathsToSearch[i]);

            if (temp[strlen(temp)-1] != '/') // add / if needed
                strcat(temp, "/");

            strcat(temp, name); // add filename

            status = stat(temp, &fileStats); // try to get status

            // check the right permission type
            if (status == 0) {
                if (!strcmp(type, "run") && fileStats.st_mode & S_IXUSR)
                    permissionFound = true;
                else if (!strcmp(type, "read") && fileStats.st_mode & S_IRUSR)
                    permissionFound = true;
                else if (!strcmp(type, "write") && fileStats.st_mode & S_IWUSR)
                    permissionFound = true;
                else if (!strcmp(type, "dir") && S_ISDIR(fileStats.st_mode))
                    permissionFound = true;
                else if (!strcmp(type, "file") && !S_ISDIR(fileStats.st_mode))
                    permissionFound = true;
                else if (!strcmp(type, "any") && fileStats.st_mode & S_IRUSR) {
                    if (fileStats.st_mode & S_IXUSR || fileStats.st_mode & S_IRUSR || fileStats.st_mode & S_IWUSR || S_ISDIR(fileStats.st_mode))
                        permissionFound = true;
                } else {
                    permissionFound = false;
                }
            }

            if (!permissionFound && status == 0) // make status -1 if file found but wrong permission
                status = -1;                     // correct file might be else where

            if (status != 0)
                temp = (char *)NULL;
        } else {
            break;
        }
    }

    if (status == 0 && permissionFound) { // file found, permission also found
            printf("%s\n", temp);
    } else { // check if we're looking for an internal command
        bool match = false;
        i = -1;
        if (!strcmp(type, "run")) {
            while (userCommands[(++i)].command != NULL) {
                if (!strcmp(userCommands[i].command, name)) { // compare against all internal commands
                    match = true;
                    printf("%s is an internal command.\n", name);
                    break;
                }
            }
        }
        
        if (!match) // not found
            printf("%s was not found.\n", name);
    }

    nshFree(temp); // free memory
    nshFree(pathsToSearch);
}


//Delete the variable if there is one.
void userCommand_L1_tes(char **givenData)
{
    //zxz    Set a variable or display a variable
    if( (givenData[0] != NULL) && (givenData[1] == NULL) )
    {
        char *varValuePtr = nshFind(envHead, givenData[0]);
        if( !(NULL == varValuePtr) ) {
            nshRemove(&envHead, givenData[0], false);
            envVarCount--;
        }
    }
}

//user gives the alias command.
void userCommand_L1_alias(char **givenData)
{
	DEBUG printf("L1_alias called with data\n");
	int i = -1;
	DEBUG while(givenData[i++] != NULL){printf("\n%s",givenData[i]);}
	DEBUG printf("\n");
    // Display all environment variables
    if(givenData[0] == NULL)
    {
    	DEBUG printf("display all\n");
        nshDisplayAll(cmdHead);
    }
    //Display a variable
    else if(givenData[0] != NULL && givenData[1] == NULL)
    {
    	DEBUG printf("searching for %s\n",givenData[0]);

        //given VALUE return NAME
        char *varValuePtr = nshFindAlias(cmdHead, givenData[0]);
        DEBUG printf("searching for VALUE: %s\t found NAME: %s\n",givenData[0],varValuePtr);
        if(NULL == varValuePtr)
        {
            printf("fail.\n");
        }
        else
        {
        	//DEBUG printf("pointer found %s\n",varValuePtr);
            printf("\n%s,%s\n",givenData[0],varValuePtr);
        }
    }
    else
    {
    	DEBUG printf("checking for complex command.\n");
        //see if a complex command was sent.
        complexCommand(givenData+1);
        if(scs.cs_present)
            givenData[1] = scs.cs;
        DEBUG printf("complexCommand(givenData+1) run\n");

        //given VALUE return NAME
        char *varValuePtr = nshFindAlias(cmdHead, givenData[0]);
        DEBUG printf("searching for VALUE: %s\t found NAME: %s\n",givenData[0],varValuePtr);
        if(NULL == varValuePtr)
        {
            //not already in list
            DEBUG printf("not already in list.\n");
            DEBUG printf("nshInsert(cmdHead,%s,%s,%d)\n",givenData[1],givenData[0],cmdVarCount);
            nshInsert(&cmdHead, givenData[1], givenData[0], cmdVarCount);
            cmdVarCount++;
        }
        else
        {
        	DEBUG printf("already in list. ()\n");
            //already in list (delete and remake)
            DEBUG printf("nshRemove(%s)\n",givenData[0]);
			nshRemove(&cmdHead, givenData[0], true);
			cmdVarCount++;
			DEBUG printf("nshInsert(cmdHead,%s,%s,%d)\n",givenData[1],givenData[0],envVarCount);
			nshInsert(&cmdHead, givenData[1], givenData[0], cmdVarCount);
			cmdVarCount++;
        }
        DEBUG printf("done dropping out.\n");
    }
}

//user gives the saila command.
void userCommand_L1_saila(char **givenData)
{
    //Delete the variable if there is one.
    DEBUG printf("Attempt delete of alias\n");
    if( (givenData[0] != NULL) && (givenData[1] == NULL) )
    {
        char *varValuePtr = nshFindAlias(cmdHead, givenData[0]);
        if( !(NULL == varValuePtr) ) {
        	DEBUG printf("Delteting %s\n", givenData[0]);
            nshRemove(&cmdHead, givenData[0], true);
            cmdVarCount--;
        }
    }
}

//user gives the exit command.
void userCommand_L1_exit(char **givenData){exit(0);}

//user gives the exit command.
void userCommand_L1_help(char **givenData)
{
    printf("\n\nProject 6\nCreated by: Fritts and Slayter\n");
    printf("\nCommands:\n");
    int i = 0;
    while(userCommands[i].command!=NULL)
        printf(" %s\n",userCommands[i++].command);
}




/*Getting data from user                              -------------------------------------------*/

//for user input through keyboard.
void getUserInput(void)
{
    char givenCommand[USER_INPUT_SIZE] = {'\0'};    //temp buffer for user input
    printf("\nnsh6> ");                                   //display prompt
    fgets(givenCommand, USER_INPUT_SIZE, stdin);    //get the input from the user
    //printf("\nWhole command: %s", givenCommand);

    if (strcmp(givenCommand, "\n") == 0) { return; } // If no input, do nothing

    tokenizeGivenStream(givenCommand);  //handles everything from here.
}





