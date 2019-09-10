CC = gcc
CFLAGS = -W
TARGET = diary_exe

$(TARGET) : memo.o calendar.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) memo.o calendar.o main.o

memo.o : memo.c
	$(CC) $(CFLAGS) -c -o memo.o memo.c

calendar.o : calendar.c
	$(CC) $(CFLAGS) -c -o calendar.o calendar.c

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

clean :
	rm *.o diary_exe
