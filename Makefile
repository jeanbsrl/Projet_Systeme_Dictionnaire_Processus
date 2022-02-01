CC=gcc
EXEC=projet_1
CFLAGS=-Wall

all: clean $(EXEC)

DEBUG: clean table.o request.o funtions_DEBUG.o projet_1_DEBUG.o projet_1

projet_1: projet_1.o functions.o request.o table.o
	$(CC) -g -o $@ $^ $(CFLAGS)

projet_1.o: projet_1.c request.h table.h functions.h
	$(CC) -c $< $(CFLAGS)

projet_1_DEBUG.o: projet_1.c request.h table.h functions.h
	$(CC) -DDEBUG -c $< $(CFLAGS)

funtions.o: functions.c request.h table.h
	$(CC) -c $< $(CFLAGS)

funtions_DEBUG.o: functions.c request.h table.h
	$(CC) -DDEBUG -c $< $(CFLAGS)

request.o: request.c request.h
	$(CC) -c $< $(CFLAGS)

table.o: table.c table.h
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(EXEC) *.o