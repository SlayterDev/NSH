#-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==
#Project 6
#	File:			makefile
#	Created by:		Fritts and Slayter
#	Last edit on: 	12/01/2013 (Fritts)
#	Description: 	Compiling each part of the project using gcc into a executable.
#					Also the option to clean. (delete all .o and nsh6 executable)
#==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-==-

nsh6: main.o nsh6_gid.o ListFunctions.o memoryHandler.o dataParsing.o fileIO.o
	gcc main.o nsh6_gid.o ListFunctions.o memoryHandler.o dataParsing.o fileIO.o -lpthread -o nsh6

memoryHandler.o: memoryHandler.c
	gcc -c memoryHandler.c

ListFunctions.o: ListFunctions.c
	gcc -c ListFunctions.c

dataParsing.o: dataParsing.c
	gcc -c dataParsing.c

fileIO.o: fileIO.c
	gcc -c fileIO.c -lpthread

nsh6_gid.o: nsh6_gid.c
	gcc -c nsh6_gid.c

main.o: main.c
	gcc -c main.c

clean:
	rm *.o nsh6
