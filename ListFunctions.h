/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           ListFunctions.h
    Created by:     Fritts and Slayter
    Last edit on:   11/21/2013 (Fritts)
	Description:	A list of functions for adding, deleteing, searching, and
					replacing data in linked lists.
					Data structures for each node of the linked list as well as
					the data that is within the node.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/




/*Data Structures.                                    -------------------------------------------*/

/*A structure that holds the two parts of the environmental variables. name and variable.*/
struct envVariable {
	char *name;
	char *value;
};
typedef struct envVariable envVariable; // Define struct as a type

/*A structure that is used for the ll part with a pointer to the data struct.*/
struct envNode {
	envVariable *variable;

	struct envNode *next;
	struct envNode *previous;
};
//typedef struct envNode envNode;

/*A structure that holds the two parts of the command alias. command and alias.*/
struct cmdAlias {
	char *cmd;   // original command
	char *alias; // new alias
};
typedef struct cmdAlias cmdAlias; // Define struct as type

/*A structure that is used for the ll part with a pointer to the data struct.*/
struct cmdNode{
	cmdAlias *command;

	struct cmdNode *next;
	struct cmdNode *previous;
};

//extern struct envNode *cmdHead; // Head for cmd alias list


// Add head ptrs
extern struct envNode *envHead; // Head for environment variable list
extern int envVarCount;

extern struct envNode *cmdHead; // Head for cmd alias list
extern int cmdVarCount;




/*Linked list functions.                              -------------------------------------------*/

/*init_ll function was called
	input:  		N/A
	output:			N/A
	description:	Initializes some global variables for use by other programs. Pointers for
					env variables and cmd variables.*/
extern void init_ll(void);

/*nshInsert function was called
	input:  		(p1,p2,p3,p4)
					p1. double pointer to envNode struct which points to next, previous, and data.
					p2. data that you want to store as the name in the struct.
					p3. data that you want to store as the value in the struct.
					p4. used for issues when ll is new.
	output:			N/A
	description:	Allocates a node in a linked list and allocates space in that node for data
					that is passed in by the user. Copies this data to list for access later.*/
void nshInsert(struct envNode**, char*, char*, int);

/*containsEnvVar function was called
	input:  		(p1,p2)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for.
	output:			status of function
	description:	Searches through enviromental variables based on the data passed in by the
					user. Return status.(1 or 0)*/
int containsEnvVar(struct envNode*, char*);

/*nshFind function was called
	input:  		(p1,p2)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. 		
	output:			char pointer of value
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (name) Returns the pointer to the (value) if found.*/
char* nshFind(struct envNode*, char*);

/*nshFindAlias function was called
	input:  		(p1,p2)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. 
	output:			char pointer of value
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (value) Returns the pointer to the (name) if found.*/
char* nshFindAlias(struct envNode*, char*);

/*nshUpdate function was called
	input:  		(p1,p2,p3)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. (name)
					p3. char pointer to the data you want to replace. (new name)
	output:			N/A
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (name) When found replace with p3 the new name.*/
void nshUpdate(struct envNode*, char*, char*);

/*nshDisplayOnce function was called
	input:  		(p1,p2)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. (name)
	output:			N/A
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (name) When found print out to screen. If not give error.*/
void nshDisplayOne(struct envNode*, char*);

/*nshDisplayAll function was called
	input:  		pointer to envNode struct which points to next, previous, and data.
	output:			N/A
	description:	Loop through whole ll and print out all its contents.*/
void nshDisplayAll(struct envNode*);

/*nshRemove function was called
	input:  		(p1,p2)
					p1. double pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. (name)
	output:			N/A
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (name) If found remove from the ll.*/
void nshRemove(struct envNode**, char*, bool);

/*containsCmdAliasByAliasName function was called
	input:  		(p1,p2)
					p1. pointer to envNode struct which points to next, previous, and data.
					p2. char pointer to the data you are searching for. (value)
	output:			status of search
	description:	Searches through ll of passed in pointer based on the data passed in by the
					user. (value) Return status if found or not (0 or 1)*/
int containsCmdAliasByAliasName(struct envNode*, char*);




/*Other.                                              -------------------------------------------*/

//void pushCmdAlias(struct cmdNode**, char*, char*, int);
//int containsCmdAlias(struct cmdNode*, char*);
//void nshNext(struct envNode**, char*);
//void updateCmdAlias(struct cmdNode*, char*, char*);
//void displayCmdAlias(struct cmdNode*, char*);
//void displayCmdAliases(struct cmdNode*);
//void clearCmdAlias(struct cmdNode**, char*);
//char* returnAliasCmd(struct cmdNode*, char*);









