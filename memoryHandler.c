/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
    File:           memoryHandler.c
    Created by:     Fritts and Slayter
    Last edit on:   11/21/2013 (Fritts)
    Description:    Functions for initializing memory subsystem,
                    dynamically allocating and reallocationg static memory, and
                    freeing this memory.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "memoryHandler.h"
#include "defs.h"

buf_t buffer[BUF_SIZE];                         //buffer for dynamically allocation of memory on runtime
buf_t bufferTemp[BUF_SIZE];                     //buffer to help with reAlloc
buffer_alloc_t  clearedChunk = {NULL,NULL,-1};  //defined struct to help with clearing.
static uint16_t allocPartCount = 0;             //keeps track of total members of chunks are allocated.




/*Initializes the masterControl struct which keeps track of buffer[]*/
void init_masterControl(buffer_master_t *master)
{
    master->head = &buffer[0];
    master->tail = &buffer[0];
    master->end  = &buffer[BUF_SIZE-1];
    master->left = BUF_SIZE;
}

/*Allocates space for users pointer given size in buffer[]*/
buf_t* theNshMalloc(buf_len_t sizeRequested)
{
    if(sizeRequested<masterControl.left)        //if enough space in buffer[]
    {
        int choice = 0, i;                         //struct chuck selected
        for(i=0; i<BUF_CHUNKS; i++)         //find a empty chuck struct member to fill up.
            if(masterBufferChunks[i].head == NULL){choice=i;break;}

        //assign struct members based on master and size requested
        masterBufferChunks[choice].head = masterControl.tail;
        masterBufferChunks[choice].tail = (masterControl.tail+sizeRequested);
        masterBufferChunks[choice].length = sizeRequested;

        //set new values for the masterControl to keep track of buffer[]
        masterControl.left -= (sizeRequested+1);
        masterControl.tail += (sizeRequested+1);
        allocPartCount++;
        return masterBufferChunks[choice].head; //return pointer to struct so user can point to it.
    }

    //not enough space!
    printf("out of dynamically static memory! (nshAlloc)");
    return NULL;
}


/*Allocates space for users pointer given size in buffer[] and sets all values to 0*/
buf_t* theNshCalloc(buf_len_t givenCount,buf_len_t givenSize)
{
    buf_t *temp = nshMalloc(givenCount);         //gets the pointer
    memset(temp,0, givenCount*sizeof(buf_t) );  //sets all to 0
    return temp;
}


/*Free up space from buffer[] given users pointer which points to a struct chunk member*/
void theNshFree(buf_t *dataToRemove)
{
    int i;
    for(i=0; i<BUF_CHUNKS; i++)             //loop through all struct members.
    {
        if(masterBufferChunks[i].head == NULL){return;}//zxz possible not at end
        if(masterBufferChunks[i].head == dataToRemove)  //if pointers match we locked on to some data!
        {
            //printf("found the data in masterBufferChunks[%d].head\n",i);

            //check if data is at the end of buffer If at the end simply remove and return.
            if(masterBufferChunks[i+1].head == NULL && masterBufferChunks[i+1].length == 0)
            {
                //printf("data is at the end.\n");
                masterControl.left += masterBufferChunks[i].length;
                masterControl.tail -= masterBufferChunks[i].length;
                masterBufferChunks[i] = clearedChunk;
                return;
            }

            //so sadly the data is not at the end of the buffer. Do aton of stuff below.
            masterControl.left += (masterBufferChunks[i].length+1); //keep track with buffer[]
            masterControl.tail -= (masterBufferChunks[i].length+1); //keep track with buffer[]
            tempChunk = masterBufferChunks[i];                      //assign to temp for later use
            masterBufferChunks[i] = clearedChunk;                   //clear the struct chunk member

            //loop through rest of struct chunks to get count
            buf_len_t totalAmountToMove = 0;
            int z;
            for(z=(i+1); z<allocPartCount; z++)
                totalAmountToMove += masterBufferChunks[z].length;

            //loop through rest of struct chunks and move all members pointer down
            for(z=(i+1); z<allocPartCount; z++)
            {
                masterBufferChunks[z].head -= (tempChunk.length+1);
                masterBufferChunks[z].tail -= (tempChunk.length+1);
            }

            //move all of the data over in the buffer
			memcpy(tempChunk.head,tempChunk.tail,(sizeof(buf_t)*totalAmountToMove) );
            return;
        }
    }
}


/*Reallocate an already allocated memory location.*/
buf_t* theNshRealloc(buf_t *givenPointer,buf_len_t sizeRequested)
{
    int choice = -1, i;


    for(i=0; i<BUF_CHUNKS; i++)             //loop through to find a matched pointer
        if(masterBufferChunks[i].head == givenPointer){choice=i;break;}

    if(choice != -1)                            //if a member was found copy over to temp buffer
        memcpy(&bufferTemp[0],masterBufferChunks[choice].head,(sizeof(buf_t)*masterBufferChunks[choice].length) );
    else                                        //no member was found
        return NULL;

    tempChunk = masterBufferChunks[choice];     //copy for later use
    theNshFree(givenPointer);                   //free the current memory location

    if(sizeRequested<masterControl.left)        //if enough data is left in the buffer for the new alloc
    {
        //does almost the same thing as alloc but with a predefined struct chunk member
        masterBufferChunks[choice].head = masterControl.tail;
        masterBufferChunks[choice].tail = (masterControl.tail+sizeRequested);
        masterBufferChunks[choice].length = sizeRequested;
        masterControl.left -= (sizeRequested+1);
        masterControl.tail += (sizeRequested+1);
        memcpy(masterBufferChunks[choice].head,&bufferTemp[0],(sizeof(buf_t)*sizeRequested) );
        return masterBufferChunks[choice].head; //return the pointer to the new location in memory
    }
    printf("out of dynamically static memory! (nshRealloc)");
    return NULL;
}
