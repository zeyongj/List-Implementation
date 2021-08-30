CC=gcc
CFLAGS=-O0 -Wall -Werror=vla -std=gnu11 -g -fsanitize=address  -lrt -lm
TEST_EXE=test

all:  list.h list.c test.c
	$(CC) $(TESTFLAGS) $^  -o $(TEST_EXE)
	./$(TEST_EXE) 
