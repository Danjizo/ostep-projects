CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -Werror
STD=c99

TARG=reverse
SRCS=reverse.c
OBJS=$(SRCS:.c=.o)

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARG) $(OBJS)
