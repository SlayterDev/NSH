/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           fileIO.h
    Created by:     Fritts and Slayter
    Last edit on:   12/04/2013 (Slayter)
	Description:	Functions for reading files, finding paths.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/




/*File operation functions                            -------------------------------------------*/

/*openResourceFile function was called
	input:  		N/A
	output:			N/A
	description:	Tries to open the resource file (.nshrc) in the home directory. Gets the
					file pointer and passes it to processScript to run the commands in .nshrc. */
extern void openResourceFile(void);

/*processScript function was called
	input:  		N/A
	output:			N/A
	description:	Takes a file pointer and loops through it line by line sending the data to
					tokenizeGivenStream() which process each line.*/
extern void processScript(FILE *);

/*setPathVar function was called
	input:  		N/A
	output:			N/A
	description:	Find the home directory of the user.*/
extern void setPathVar(void);

/*runBackgroundProcess function was called
	input:  		double pointer to command that you want to execute.
	output:			N/A
	description:	take the input data and tokenize, parse, etc.  Once completed create a thread
					and execute the command in that new thread.*/
extern void runBackgroundProcess(char **givenData);

/*threadCall function was called
	input:  		void pointer to command that you want to execute.
	output:			N/A
	description:	take the input data, fork the process, execute the command*/
void *threadCall(void *givenCmds);

/*executeExternalCommand function was called
	input:  		double pointer to command that you want to execute.
	output:			N/A
	description:	take the input data and tokenize, parse, etc.  Once completed create a process
					and execute the command in that new process.  Print out the data for the user.*/
extern void executeExternalCommand(char **givenData);




