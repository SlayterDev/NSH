/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
	File:			dataParsing.h
	Created by:		Fritts and Slayter
	Last edit on: 	12/01/2013 (Fritts)
    Description:    A list of functions that are designed to tokenize, parse,
                    reconstruct, manipulate data to extract information and store
                    data for the user.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/




/*Data Structures.                                    -------------------------------------------*/

/*A struct that holds data while parsing for commands with a command.*/
typedef struct
{
    bool cs_present[20];
    char *execArgs[20]; //allocate a double pointer for new data
}parseAgain_t;




/*Functions for memory handling.                      -------------------------------------------*/

/*tokenizeEnvVar funcion was called
	input:			pointer to value of an environment variable
	output:			double pointer to array of values pulled from initial pointer
	description:	When an evironment variable value with multiple values seperated by "!" are passed
					to this fuction (/usr/bin!..!.), the function will then split the given value
					into an array of char*'s to be used as seperate values.*/
extern char** tokenizeEnvVar(char *givenVar, char **values);

/*getNumberOfValues function was called
	input:			pointer to string value
	output:			int count of values in a "!" seperated env value
	description:	When a char* to an evironment variable is passed in, this function will count
					the number of values contained in the pointer seperated by the "!" character.*/
extern int getNumberOfValues(char *string);

/*tokenizeGivenStream function was called
	input:  		pointer to data that is a command that is either from a File IO, stdin, etc.
	output:			N/A
	description:	When a raw message is sent from the user or file this is the first function
					to start parsing the data.  It takes care of most of the heavy lifting of 
					interpreting a command and packaging commands up. (Comments, multiple commands
					on one line, multiple lines, formatting for later functions and more.*/
extern void tokenizeGivenStream(char *givenData);

/*getCommandWithinACommand function was called
	input:  		double pointer of data that is a tokenized input from any source.
	output:			double pointer of data that is tokenized and just a modification of the source
					based on a complex alias.
	description:	Loops through each chunk of tokenized data and reallocates the pointer if it
					finds a alias moving the other pointers down.  Can handle complex aliases.
					Returns a double pointer with the modified data based on the alias.*/
extern char** getCommandWithinACommand(char** givenData);

/*parseCommand function was called
	input:  		double pointer of data that is a tokenized input from any source.
	output:			N/A
	description:	Loops through each chunk of tokenized data and reallocates the pointer if it
					finds a alias.  Second it tries to find a command based on the one supported
					and if so runs the callback functions listed below.*/
extern void parseCommand(char **givenCommands);

/*extendEnvironmentVar function was called
	input:  		pointer to env var that needs to be extended
	output:			pointer to new env var that was extended
	description:	Goes through input given by file or user and parses that data allowing for
					the extension of env variables based on other variables and just text etc.*/
extern char* extendEnvironmentVar(char *varValue);

/*complexCommand function was called
	input:  		double pointer to data in question if its complex. (tokenized)
	output:			N/A
	description:	Looks into data in double pointer and finds if it contains a complex
					command.  Because the data is tokenized the pointers need to be reallocated
					to the new reconstructed command.*/
extern void complexCommand(char **givenData);





