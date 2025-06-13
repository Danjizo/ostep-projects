CC=gcc
ERRFLAGS=-Wall -Wpedantic -Wextra -Werror
SANFLAGS=-fsanitize=address
TARG=wcat
SRCS=wcat.c
OBJS=$(SRCS:.c=.o)

all: $(TARG)

$(TARG): $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(SANFLAGS)

%.o: %.c
	$(CC) $(ERRFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARG)
