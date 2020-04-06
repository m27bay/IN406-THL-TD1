CFLAGS = -Wall -g
CC = gcc

aut_directory = aut

run: clean automate
	mkdir $(aut_directory)
	./automate
# 	sublime $(aut_directory)/*.aut

open:
	sublime $(aut_directory)/*.aut

automate: test.o automate.o
	$(CC) test.o automate.o -o automate

%.o: %.c automate.h
	$(CC) -c $< $(CFLAGS)

valgrind: automate
	valgrind --leak-check=full ./automate

clean:
	rm -f *.o
	rm -f automate
	rm -f $(aut_directory)/*.aut
	rm -df $(aut_directory)
