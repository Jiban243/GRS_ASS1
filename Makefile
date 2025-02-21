
CC = gcc
CFLAGS = -Wall -Wextra -O2


EXE1 = my_shell
EXE2 = fact


SRC1 = my_shell.c
SRC2 = fact.c


all: $(EXE1) $(EXE2)


$(EXE1): $(SRC1)
	$(CC) $(CFLAGS) -o $@ $< -lreadline


$(EXE2): $(SRC2)
	$(CC) $(CFLAGS) -o $@ $<


clean:
	rm -f $(EXE1) $(EXE2)


.PHONY: all clean
