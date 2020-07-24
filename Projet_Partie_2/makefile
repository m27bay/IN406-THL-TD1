CFLAGS= -Wall -g
CC = gcc

run: clean automate
	./automate "(a+b)*.a#"
	./automate "(a+b)*.a"
	./automate "a+b)*.a#"
	./automate "(a+)*.a#"
	./automate "(a+b).*a#"
	./automate "a+b**.(a.b.z)+d#"

run2: clean automate
	./automate "(a+b)*.a#"

automate: test.o automate.o regexp.o pile.o
	$(CC) test.o automate.o regexp.o pile.o -o automate

%.o: %.c automate.h regexp.h
	$(CC) -c $< $(CFLAGS)

valgrind: automate
	valgrind --leak-check=full ./automate "(a+b)*.a#"

valgrind2 : automate
	valgrind --leak-check=full ./automate "(a+b)*.a#"
	valgrind --leak-check=full ./automate "(a+)*.a#"
	valgrind --leak-check=full ./automate "a+b)*.a#"

clean:
	rm -f *.o
	rm -f automate
	rm -f test_*
