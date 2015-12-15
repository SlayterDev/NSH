/*-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
Project 6
	File:			defs.h
	Created by:		Fritts and Slayter
	Last edit on: 	12/01/2013 (Fritts)
	Description:	Debug defines for whole program + more.
==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-*/




/*Defines.                                            -------------------------------------------*/

//general define
#define USER_INPUT_SIZE 1024

//static memory allocation defines
#define BUF_SIZE    32768        		//buffer[] size
#define BUF_CHUNKS  200         		//number of struct chunks used to keep track of members
#define buf_t       unsigned char		//type of data used for buffer
#define buf_len_t   int 				//type of data used for determining length




/*Debug Stuff.                                        -------------------------------------------*/


/*  Ghetto Debugger v0.1 
Level:
	0 = NO debug
	1 = NORMAL debug
	2 = DETAILED debug
*/
#define DEBUG_SETTING 0
//#define DEBUG_COMPILE_WARNING

#ifdef DEBUG_SETTING
	#ifdef DEBUG_COMPILE_WARNING
	#warning "DEBUG LEVEL SET"
	#endif
	#if DEBUG_SETTING == 0
		#ifdef DEBUG_COMPILE_WARNING
			#warning "DEBUG OFF"
		#endif
 		#define DEBUG for(;0;)
	#elif DEBUG_SETTING == 1
		#ifdef DEBUG_COMPILE_WARNING
			#warning "DEBUG NORMAL"
		#endif
 		#define DEBUG
	#elif DEBUG_SETTING == 2
		#ifdef DEBUG_COMPILE_WARNING
			#warning "DEBUG DETAILED"
		#endif
		#if __GNUC__
 			#define DEBUG printf("%s(%d): ",__FILE__,__LINE__);
		#else
			#ifdef DEBUG_COMPILE_WARNING
				#warning "DETAILED NOT SUPPORTED NOW FOR COMPILER"
				#warning "DUBUG NORMAL"
			#endif
			#define DEBUG
		#endif
	#endif
#else
	#ifdef DEBUG_COMPILE_WARNING
		#warning "DEBUG LEVEL UNSET"
		#warning "DEFAULT TO DEBUG OFF"
	#endif
	#define DEBUG for(;0;)
#endif

