CC = gcc
CFLAGS = -w -g 

SRCS = main.c lexer.c parser.c semantic.c icg.c codegen.c symbol_table.c
OBJS = $(SRCS:.c=.o)

mini_c: $(OBJS)
	$(CC) $(CFLAGS) -o mini_c $(OBJS)

%.o: %.c mini_c.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o mini_c