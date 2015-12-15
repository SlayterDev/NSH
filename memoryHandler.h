/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
	File:			memoryHandler.h
	Created by:		Fritts and Slayter
	Last edit on: 	12/01/2013 (Fritts)
	Description:	A list of functions for initializing memory subsystem,
					dynamically allocating and reallocationg static memory, and
					freeing this memory.
					Data structures that handle keeping track of how the data is layed
					out in memory for access and deletion.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/


/*Includes.                                           -------------------------------------------*/

#include "defs.h"




/*Data Structures.                                    -------------------------------------------*/

/*used to keep track of every member in buffer[]*/
typedef struct
{
    buf_t       *head;
    buf_t       *tail;
    buf_len_t   length;
}buffer_alloc_t;
buffer_alloc_t  masterBufferChunks[BUF_CHUNKS];
buffer_alloc_t  tempChunk;

/*used to keep track of the whole buffer[] (for insert,remove,etc.)*/
typedef struct
{
    buf_t       *head;
    buf_t       *tail;
    buf_t       *end;
    buf_len_t   left;
}buffer_master_t;
buffer_master_t  masterControl;




/*Functions for memory handling.                      -------------------------------------------*/

/*init_masterControl function was called
	input:  		pointer to buffer_master_t struct
	output:			N/A
	description:	initializes masterControl for buffer[] handling.*/
extern void init_masterControl(buffer_master_t *master);

/*theNshMalloc function was called
	input:  		buf_len_t data (requested data amount.)
	output:			pointer to the section of memory that has been allocated.
	description:	Dynamically allocates memory based on a pool of static memory (buffer[]) */
extern buf_t* theNshMalloc(buf_len_t sizeRequested);

/*theNshCalloc function was called
	input:  		(p1,p2) p1. Number of elements you want to allocate. p2. Size if elements.
	output:			pointer to the section of memory that has been allocated.
	description:	Dynamically allocates memory based on a pool of static memory (buffer[])
					and zeros out the allocated section of memory.*/
extern buf_t* theNshCalloc(buf_len_t givenCount,buf_len_t givenSize);


/*theNshFree function was called
	input:  		pointer to the data that has previosly been allocated with nshMalloc,nshCalloc
	output:			N/A
	description:	Dynamically frees memory based on a pool of static memory (buffer[])*/
extern void theNshFree(buf_t *dataToRemove);

/*theNshRealloc function was called
	input:  		(p1,p2) p1. Pointer to data previously allocated. p2. Number of bytes wanted now.
	output:			pointer to the section of memory that has been reallocated.
	description:	Dynamically reallocates memory based on a pool of static memory (buffer[])*/
extern buf_t* theNshRealloc(buf_t *givenPointer,buf_len_t givenLength);




/*Macros that map theNsh functions to nsh functions or C stdlib malloc  -------------------------*/

/*theNsh functions renamed to nsh functions*/
#define nshMalloc(data)             theNshMalloc((buf_len_t)(data))
#define nshCalloc(data1,data2)      theNshCalloc((buf_len_t)(data1),(buf_len_t)(data2))
#define nshRealloc(data1,data2)     theNshRealloc((buf_t*)(data1),(buf_len_t)(data2))
#define nshFree(data)\
do{theNshFree((buf_t*)data);\
data=NULL;}while(0)

/*For ruling out my memoryHandler functions as the problem.*/
/*#define nshMalloc(a)             malloc(a)
#define nshCalloc(a,b)             calloc(a,b)
#define nshRealloc(a,b)            realloc(a,b)
#define nshFree(a)               free(a)*/