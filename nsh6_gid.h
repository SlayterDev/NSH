/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
	File: 			nsh5_gid.h
	Created by:		Fritts and Slayter
	Last edit on: 	12/04/2013 (Slayter)
	Description:	A list of functions for:
						-getting data from a file or user and running these commands
						-callback functions for these commands
						-parsing data given by the user
						-helper functions for making parsing data easier
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/


/*Includes.                                           -------------------------------------------*/

#include <stdbool.h>
#include <stdint.h>




/*Data Structures.                                    -------------------------------------------*/

/*A pair of command and callback for a given command.  EX: "set",&userCommand_L1_set()*/
typedef struct
{
    char *command;
    void (* func)(char**);
}command_L1_t;

extern command_L1_t userCommands[];

/*A struct that will hold a complex string if the user gives one.*/
typedef struct
{
    bool cs_present;
    char cs[1024];
}simpleComplexString_t;

extern simpleComplexString_t scs;
extern simpleComplexString_t scsEmpty;




/*Getting data from user.                             -------------------------------------------*/

/*parseCommand function was called
	input:  		N/A
	output:			N/A
	description:	Takes the input from the user using stdio and places that data in a buffer
					for later analysis.*/
extern void getUserInput(void);




/*Callback functions for all of the major commands.   -------------------------------------------*/

/*SET function was called
	input:  		double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to set a env variable this function is called.*/
void userCommand_L1_set(char **givenData);

/*TES function was called
	input:  		double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to delete a env variable this function is called.*/
void userCommand_L1_tes(char **givenData);

/*ALIAS function was called
	input:			double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to set a alias for anything this function is called.*/
void userCommand_L1_alias(char **givenData);

/*SAILA function was called
	input:			double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to delete an alias that was created this function is called.*/
void userCommand_L1_saila(char **givenData);

/*EXIT function was called
	input:			dont care (not used)
	output:			N/A
	description:	When a user wants to exit the program this function is called and exit(0) is called.*/
void userCommand_L1_exit(char **givenData);

/*HELP function was called
	input:			dont care (not used)
	output:			N/A
	description:	When a user wants wants to see the help menu this function is called.
		displays descriptions for user.*/
void userCommand_L1_help(char **givenData);

/*ECHO function was called
	input:			double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to echo the given input this function is called.*/
void userCommand_L1_echo(char **givenData);

/*WHERE function was called
	input:			double pointer of data that procedes the command given.
	output:			N/A
	description:	When a user wants to run the where command this function is called.*/
void userCommand_L1_where(char **givenData);
