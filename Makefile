CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=*.o)
EXECBIN = encode decode

ENC_SRC = encode.c pq.c node.c stack.c huffman.c io.c code.c
ENC_OBJ = $(ENC_SRC:.c=*.o)
DEC_SRC = decode.c pq.c node.c stack.c huffman.c io.c code.c
DEC_OBJ = $(DEC_SRC:.c=*.o)

.PHONY: all clean format debug

all: $(EXECBIN)

debug: CFLAGS += -g
debug: all

clean:
	rm -f $(OBJ) $(EXECBIN)

format:
	clang-format -i -style=file *.[c,h]

#$(EXECBIN): $(OBJ)
#	$(CC) $(CFLAGS) $(OBJ) -o $(EXECBIN) -lm

encode: $(ENC_OBJ)
	$(CC) $(CFLAGS) $(ENC_OBJ) -o encode

decode: $(DEC_OBJ)
	$(CC) $(CFLAGS) $(DEC_OBJ) -o decode

%.o: %.c
	$(CC) $(CFLAGS) -c $<
