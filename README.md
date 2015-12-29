# myFileSystem
файловая система на базе fuse, по-моему мнению, с непрерывным размещением)))


CC = gcc
FLAGS = -lfuse

fs: filesystem.o actions.o
	$(CC) -o fs filesystem.o actions.o $(FLAGS)

filesystem.o: filesystem.c actions.h
	$(CC) -c filesystem.c

fsactions.o: actions.c actions.h
	$(CC) -c actions.c
