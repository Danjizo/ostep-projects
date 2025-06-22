CC=gcc
ERRFLAGS=-Wall -Wextra -Wpedantic -Werror
SANFLAGS=-fsanitize=address
TARG=wzip
SRCS=wzip.c
OBJS=$(SRCS:.c=.o)

all: release

release: $(OBJS)
	$(CC) -o $(TARG) $(OBJS)

build: $(OBJS)
	$(CC) -o $(TARG) $(OBJS) $(SANFLAGS)

%.o: %.c
	$(CC) $(ERRFLAGS) -c  $< -o $@

test: build
	./test-wzip.sh

clean:
	rm -f $(TARG) $(OBJS)
