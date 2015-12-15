/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           dataParsing.c
    Created by:     Fritts and Slayter
    Last edit on:   12/04/2013 (Slayter)
    Description:    A list of functions that are designed to tokenize, parse,
                    reconstruct, manipulate data to extract information and store
                    data for the user.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dataParsing.h"
#include "fileIO.h"
#include "ListFunctions.h"
#include "memoryHandler.h"
#include "defs.h"
#include "nsh6_gid.h"



parseAgain_t firstInput = {{0},{0}};
parseAgain_t paBlank = {{0},{0}};
//if command is alias newAlias {set bat}
// you now type (newAlias foo)
// you would get (set bat foo)
//
char temp[100] = {0};
//char dataToFinallyParseForCommands[100] = {0};

char** getCommandWithinACommand(char** givenData)
{
    int command_count = 0;  //the current count of commands leaving
    firstInput = paBlank; //empty out struct.
    int i = 0;

    //loop through all commands
    while(givenData[i] != NULL)
    {
        char *varValuePtr = nshFindAlias(cmdHead, givenData[i]);
        if(varValuePtr != NULL)    //if command is a valid alias
        {
            char *dataToFinallyParseForCommands = (char*)nshMalloc(100);
            //copy into a temp buffer
            strcpy(temp,varValuePtr);
            //find if they are both present
            char *complexStringStart = strchr(temp,'{');
            char *complexStringEnd = strchr(temp,'}');
            //check if both are valid and in the right order
            if(complexStringStart != NULL && complexStringEnd != NULL)
            {
                //ya data is valid
                if(complexStringStart < complexStringEnd)   //verify that they are in the right order
                {
                    //copy the data into a array that removes the {} ready for tok
                    memcpy(dataToFinallyParseForCommands,complexStringStart+1, (complexStringEnd-1) - (complexStringStart) ); // Fixed end index, off by 1 - Brad
                    //printf("[DEBUG] dataToFinallyParse: %s\n", dataToFinallyParseForCommands);
                    firstInput.cs_present[i] = true;
                }
            }
            //if complex string is found break that command dowm again.
            if(firstInput.cs_present[i])
            {
                //char *commandSplit[20] = {NULL};                     //pointer for split command
                char *currentTok = strtok(dataToFinallyParseForCommands, " ");         //temp pointer for tok
                //int cmdChunks = 0;

                while (currentTok != NULL)                            //split the char array command into sperate chunks
                {
                    firstInput.execArgs[command_count++] = currentTok;
                    //commandSplit[cmdChunks++] = temp;
                    currentTok = strtok(NULL, " ");
                }
                firstInput.execArgs[command_count] = currentTok;
                i++;
                if(i > 2){break;}
                continue;
            }
            else
            {
                firstInput.execArgs[command_count++] = givenData[i];
                i++;
                if(i > 2){break;}
                continue;
            }
        }
        //not a valid alias
        firstInput.execArgs[command_count++] = givenData[i];
        i++;
        if(i > 2){break;}
    }

    //i = 1;
    while(givenData[i] != NULL)
    {
        firstInput.execArgs[command_count++] = givenData[i];
        i++;
    }
    return firstInput.execArgs;
}


char** tokenizeEnvVar(char *givenVar, char **values) {
    //values = {NULL};
    char *varCopy = (char *)nshMalloc(strlen(givenVar)+1);
    strcpy(varCopy, givenVar); // copy string to prevent data manipulation

    char *temp = strtok(varCopy, "!");
    int i = 0;

    while (temp != NULL) {
        values[i++] = temp;
        temp = strtok(NULL, "!");
    }
    values[i] = (char *)NULL;

    return values;
}

int getNumberOfValues(char *string) {
    int i = strlen(string+1), j;
    char copy[i];
    strcpy(copy, string);

    i = 0;
    for (j = 0; copy[j]; j++)
        if (copy[j] == '!') { i++; }

    return i + 1;
}

//used for multiple commands
char tempCommand[USER_INPUT_SIZE] = {0};
bool multipleLineCommands = false;
bool secondCommandWithinSameString = false;
char *extraCommand = NULL;


//pass in a pointer to the data you want to get parsed. (File IO, stdin, etc.)
void tokenizeGivenStream(char *givenData)
{
    //get rid of comments
    char* temp3 = strchr(givenData,'~');//find the comments
    if(temp3 != NULL){*(temp3) = 0;}    //essentially just removes the comments
    if(temp3 == &givenData[0]){return;} //if just a comment get out of here

    //handle multiple line commands by the user
    if(multipleLineCommands)
    {
        char* temp8 = strchr(givenData,'\n');   //remove the pesky \n before merge
        if(temp8 != NULL){*(temp8) = 0;}
        strcpy(tempCommand+strlen(tempCommand),givenData);  //put commands together
    }

    //detect if multiple commands..
    char* temp4 = strchr(givenData,'$');
    if(temp4 != NULL)   //if character found = multiple commands
    {
        if( *(temp4-1) == ' '){ *(temp4) = 0; } //fixes formatting issues
        else
        {
            *(temp4) = ' ';
            *(temp4+1) = 0;
        }
        strcpy(tempCommand,givenData);  //copy current passed data into temp buffer
        multipleLineCommands = true;    //flag set high for next call of this function
        return;
    }
    else
    {
        if(!multipleLineCommands)   //just clear out just to make sure.
            memset(tempCommand,0,strlen(tempCommand)+1);
        multipleLineCommands = false;
    }

    //get command ready for strtok!
    char* temp2 = strchr(givenData,'\n');   //formatting help for removing pesky \n
    if(temp2 != NULL){*(temp2) = ' ';}


    //if a command is in the tempCommand reset the pointer to the new data location
    if(strlen(tempCommand))
        givenData = &tempCommand[0];

    //find if a second command is available..
    char* secondCommandLocation = strchr(givenData,'?');

    //set location to 0 so functions below dont see the next commands. (null terminate)
    //and increment pointer for recursive call at end.
    if(secondCommandLocation != NULL)
    {
        *(secondCommandLocation) = 0;
        secondCommandLocation++;
    }

    //tokenize data to get ready for parseCommand()
    char *commandSplit[20] = {NULL};             //pointer for split command
    char *temp = strtok(givenData, " ");         //temp pointer for tok
    int cmdChunks = 0;

    while (temp != NULL)                         //split the char array command into sperate chunks
    {
        commandSplit[cmdChunks++] = temp;
        temp = strtok (NULL, " ");
    }
    commandSplit[cmdChunks] = 0;

    parseCommand(commandSplit);     //Do the commands bidding


    //if second command is found do a recursive call with its location.
    if(secondCommandLocation != NULL)
        tokenizeGivenStream(secondCommandLocation);
}


char* extendEnvironmentVar(char *varValue)
{
    int i, j = 0, k = 0, l = strlen(varValue); // counters for loop, varName, and varValue, and length of varValue
    char input[201] = {0}, varName[101] = {0}, *newVarValue; // string for complete input, name of envVar, and new Value
    bool readingVar = false; // flag for reading var or not

    newVarValue = NULL; // initialize newVarValue ptr

    for (i = 0; i <= l; i++) {
        if (varValue[k] == 0) // break if we are reading null
            break;

        if (varValue[k] == '@') { // start reading environment variable name
            readingVar = true;
            k++;
            if (i > 0)            // decrement insert point to account for '@'
                i--;
        }

        if (readingVar == true) {
            varName[j] = varValue[k]; // read into varName
            varName[j+1] = '\0'; // null terminate to prevent garbage
            j++;
            k++;
                //allocates 2 bytes and copies a !0 into it.
            char *varToCheck = (char *)nshMalloc(strlen(varName)+1); // allocate space for varName
            strcpy(varToCheck, varName);

            if (containsEnvVar(envHead, varToCheck)) { // if list contains variable
                newVarValue = nshFind(envHead, varToCheck);
                input[i] = '\0'; // null terminate current input to make it clean

                if (i > 0)
                    i -= strlen(varName);
                l -= strlen(varName);
                i += strlen(newVarValue);  // offset insert point in input string by var length
                l += strlen(newVarValue);
                strcat(input, newVarValue); // add to current input
                readingVar = false;
            }
            nshFree(varToCheck);
        } else if (!readingVar) {        // read into input string
            input[i] = varValue[k];
            k++;
        }
    }
    char *word = (char *)nshMalloc(strlen(input)+1); // malloc space for entire value
    strcpy(word, input);                             // copy input into it

    return word;

}



//pass in full command tokenized.
    //EX: set foo
    //EX: ll     <- with ll being a complex string (set foo)
void parseCommand(char **givenCommands)
{
    DEBUG printf("parseCommand() called\n");
    DEBUG printf("Given before: %s, %s, %s, %s, %s\n", givenCommands[0], givenCommands[1], givenCommands[2],givenCommands[3],givenCommands[4]);

    //checks if a command alias is set and if so point to the actual command
    while( containsCmdAliasByAliasName(cmdHead, givenCommands[0]) )
    {
        char *varValuePtr = nshFindAlias(cmdHead, givenCommands[0]);
        if(varValuePtr != NULL)    //if command is a valid alias
        {
           givenCommands[0] = varValuePtr;
        }
        else
            break;
    }

    DEBUG printf("Given stage1: %s, %s, %s, %s, %s\n", givenCommands[0], givenCommands[1], givenCommands[2],givenCommands[3],givenCommands[4]);

    if(givenCommands[1] != NULL && givenCommands[2] == NULL)
    {
        char *varValuePtr = nshFindAlias(cmdHead, givenCommands[1]);
        if(varValuePtr == NULL)
        {
            if(strcmp(givenCommands[0],"alias"))
                givenCommands = getCommandWithinACommand(givenCommands);
            //DEBUG printf("NULL\n");
        }
        //DEBUG printf("!NULL\n");
    }
    else
    {
        //DEBUG printf("else\n");
        if(strcmp(givenCommands[0],"alias"))
            givenCommands = getCommandWithinACommand(givenCommands);
    } 

    DEBUG printf("Given stage2: %s, %s, %s, %s, %s\n", givenCommands[0], givenCommands[1], givenCommands[2],givenCommands[3],givenCommands[4]);

    //searches through user commands for a match and if so run the function
    int i = -1;
    bool background = false;

    if (givenCommands[0][0] == '+') { // want to run process in background
        background = true;

        if (givenCommands[0][1] != '0' || givenCommands[0][1] != ' ')
            givenCommands[0] = givenCommands[0]+1;                    // shift first argument forward one char
        else
            givenCommands = givenCommands+1;                          // shift array forward one element
    }

    while(userCommands[(++i)].command != NULL)
    {
        //compare the set of user commands to the one given
        if( !strcmp((const char*)userCommands[i].command,(const char*)givenCommands[0]) )
        {
            //printf("\n\nCommand matched: %s\nIs:%s\n",userCommands[i].command,givenCommands[0]);
            userCommands[i].func(givenCommands+1);      //run the commands function
            return;
        }
    }

    if (!background)
        executeExternalCommand(givenCommands);
    else
        runBackgroundProcess(givenCommands);

    return;
}


//takes in the passed data (after the "set cat") and reconstructs message to figure out info.
void complexCommand(char **givenData)
{
    scs = scsEmpty; //empty out struct.
    //reconstruct the string
    int i = 0;
    while(givenData[i] != 0)
    {
        strcat(scs.cs,givenData[i]);
        strcat(scs.cs," ");
        i++;
    }
    scs.cs[strlen(scs.cs)-1] = 0;   //remove last space added

    //find the comments if they exist
        //and just remove them.
    char* comment = NULL;
    comment = strchr(scs.cs,'~');
    if(comment != NULL)
    {
        memset(comment,0,strlen(comment)+1);        //remove comment
        if(*(comment-1) == ' ') //remove space in front of comment
            *(comment-1) = 0;
    }

    char *complexStringStart = strchr(scs.cs,'{');
    char *complexStringEnd = strchr(scs.cs,'}');

    //check that they are both present
        //and set complexString array to the complex string within the {}
    if(complexStringStart != NULL && complexStringEnd != NULL)
    {
        if(complexStringStart < complexStringEnd)   //verify that they are in the right order
        {
            memcpy(scs.cs,complexStringStart, (++complexStringEnd) - complexStringStart );
            scs.cs_present = true;
        }
    }
}











































