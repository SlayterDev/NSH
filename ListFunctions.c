/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           ListFunctions.c
    Created by:     Fritts and Slayter
    Last edit on:   11/29/2013 (Fritts)
	Description:	Functions for adding, deleteing, searching, and
					replacing data in linked lists.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "ListFunctions.h"
#include "memoryHandler.h"
#include "defs.h"



// Add head ptrs
struct envNode *envHead; // Head for environment variable list
int envVarCount;

struct envNode *cmdHead; // Head for cmd alias list
int cmdVarCount;

void init_ll(void)
{
	envHead = NULL;
	cmdHead = NULL;
	envVarCount = 0;
	cmdVarCount = 0;
}


// *************** ENVIRONMENT VARIABLE FUNCTIONS ***************

void nshInsert(struct envNode **envHead, char *name, char *value, int envVarCount) {
	struct envNode *node = (struct envNode *)nshMalloc(sizeof(struct envNode)); // create a new node

	envVariable *variable = (envVariable *)nshMalloc(sizeof(envVariable));

	variable->name =  (char *)nshMalloc(sizeof(char)*(strlen(name)+1));  // malloc space for name and value
	variable->value = (char *)nshMalloc(sizeof(char)*(strlen(value)+1));

	strcpy(variable->name, name);
	strcpy(variable->value, value);

    node->next = NULL;              //preset to NULL needed
    node->previous = NULL;          //preset to NULL needed
	node->variable = variable;      //set the node variable pointer to malloced var pointer above

	if (envHead == NULL || envVarCount == 0) { // If head is null, create it
		node->next = NULL;
		(*envHead) = node;
		//printf("creating head\n");
	} else { // find proper place to insert
		//printf("push on head\n");
		struct envNode *current = *envHead;
		while (current != NULL) {
			if (strcmp(node->variable->name, current->variable->name) > 0) { // if the string is greater, keep going
				if (current->next != NULL) {
					current = current->next;
				} else { // push onto tail
					current->next = node;
					node->previous = current;
					break;
				}
			} else {
				if (current->previous != NULL) { // Insert in middle of list
					current = current->previous; // This block links up the nodes
					node->next = current->next;
					current->next = node;
					node->next->previous = node;
					node->previous = current;

					current = node;
				} else {						 // Insert at head
					node->next = current;
					current->previous = node;
				}
				break;
			}
		}

		// make sure head is updated
		while (current != NULL) {
			if (current->previous == NULL) {
				(*envHead) = current;
				break;
			}
			current = current->previous;
		}
	}
}

int containsEnvVar(struct envNode *envHead, char *varName) {
	struct envNode *current = envHead;

	while (current != NULL) {
		if (strcmp(current->variable->name, varName) == 0) {		// variable found
			return 1;
		} /*else if (strcmp(current->variable->name, varName) > 0) {  // we've gone further than where the var would be
			return 0;												// abort search
		}*/

		current = current->next;
	}

	return 0;
}

//given name return value
char* nshFind(struct envNode *envHead, char *varName) {
	struct envNode *current = envHead;

	while (current != NULL) {
		if (strcmp(current->variable->name, varName) == 0) {		// variable found
			return current->variable->value;
		} /*else if (strcmp(current->variable->name, varName) > 0) {	// we've gone further than where the var would be
			return (char *)NULL;									// abort search
		}*/

		current = current->next;
	}

	return (char *)NULL;
}

//given value return name
char* nshFindAlias(struct envNode *envHead, char *varName) { // same as nshFind, but returns cmd alias wants to run
	struct envNode *current = envHead;

	while (current != NULL) {
		if (strcmp(current->variable->value, varName) == 0) {
			return current->variable->name;
		} /*else if (strcmp(current->variable->name, varName) > 0) {
			return (char *)NULL;
		}*/

		current = current->next;
	}

	return (char *)NULL;
}

void nshUpdate(struct envNode *envHead, char *varName, char *varValue)
{
	DEBUG printf("nshUpdate() called with\t NAME: %s\t VALUE:%s\n",varName,varValue);
	struct envNode *current = envHead;

	while (current != NULL)	// same search code as above
	{
		DEBUG printf("comparing\t %s\t%s\n",current->variable->name,varName);
		if (strcmp(current->variable->name, varName) == 0)
		{
			break;
		}
		current = current->next;
	}

	DEBUG printf("nshFree(%s)\n",current->variable->value);
	nshFree(current->variable->value); // free the value
	DEBUG printf("nshMalloc(%zu)\n",(strlen(varValue)+1));
	current->variable->value = (char *)nshMalloc(strlen(varValue)+1); // malloc and set new value
	DEBUG printf("strcpy(%s,%s)\n",current->variable->value, varValue);
	strcpy(current->variable->value, varValue);
}


void nshDisplayOne(struct envNode *envHead, char *varName) { // display one variable/alias by name
	struct envNode *current = envHead;

	while (current != NULL) {
		if (strcmp(current->variable->name, varName) == 0) {
			printf("%s = %s\n", varName, current->variable->value);
			return;
		}

		current = current->next;
	}

	printf("%s: Variable not found\n", varName);
}

void nshDisplayAll(struct envNode *envHead) { // display all variables/alias
	struct envNode *current = envHead;

	while (current != NULL) {
		printf("%s = %s\n", current->variable->name, current->variable->value);

		current = current->next;
	}
}

void nshRemove(struct envNode **envHead, char *varName, bool isAlias) {
	struct envNode *current = *envHead;
	struct envNode *temp;

	//printf("[DEBUG] Clearing var\n");

	while (current != NULL) { // find variable to clear
		if (strcmp(current->variable->name, varName) == 0 && !isAlias) {
			break;
		} else if (strcmp(current->variable->value, varName) == 0) {
			break;
		}

		current = current->next;
	}

	if (current->next == NULL && current->previous == NULL) { 		  // node is the only one in the list
		current = (struct envNode *)NULL;
	} else if (current->next == NULL && current->previous != NULL) {  // node is at the tail of list
		current = current->previous;
		current->next = (struct envNode *)NULL;
	} else if (current->next != NULL && current->previous == NULL) {  // node is the head of list
		current = current->next;
		current->previous = (struct envNode *)NULL;
	} else if (current->next != NULL && current->previous != NULL)	{ // node is the middle of the list
		temp = current->next;
		current = current->previous;
		current->next = temp;
	}

	if (current != NULL) {
		while (current->previous != NULL) { // place current back at the head
			current = current->previous;
		}
	}

	(*envHead) = current; // update the list

}

void nshNext(struct envNode **envHead) { // progress through list
	*envHead = (*envHead)->next;
}

int containsCmdAliasByAliasName(struct envNode *cmdHead, char *aliasName) { // returns 1 if command alias exists
	struct envNode *current = cmdHead;										// check using alias name

	while (current != NULL) {
		if (strcmp(current->variable->value, aliasName) == 0) {
			return 1;
		}

		current = current->next;
	}

	return 0;
}
