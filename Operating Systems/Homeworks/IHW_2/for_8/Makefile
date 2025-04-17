CC = gcc
CFLAGS = -Wall

all: init student controller

init: init.c
	$(CC) $(CFLAGS) init.c -o init

student: student.c
	$(CC) $(CFLAGS) student.c -o student

controller: controller.c
	$(CC) $(CFLAGS) controller.c -o controller

clean:
	rm -f init student controller
