# Huffman Compression
The program included, a combination of `encode` and `decode`, is an implementation of the Huffman Compression Algorithm in C.\
The following program uses data-structures such as a min-heap priority queue, stacks, and bit-vectors in order to implement the Huffman encoder and decoder.\
The I/O functionality of this program uses the standard C `open() close() read() write()` functions.  The program also utilizes buffer sizes of 4KB per read and write call.

## Building
`make`          Equivelent to `make all`.\
`make all`      Makes both `encode` and `decode`.\
`make encode`   Makes just the `encode` executable.\
`make decode`   Makes just the `decode` executable.\
`make clean`    Removes all executable and object files.\
`make format`   Clang formats all `.c` and `.h` files.\
`make debug`    Makes `encode/decode` with debug flags.

## Running
`./encode -[vh] -[i infile] -[o outfile]`\
`./decode -[vh] -[i infile] -[o outfile]`\
Leaving the infile or outfile with default to `stdin`/`stdout`.\

# Arguments List
```
-h  Outputs information on how to run the respective program.
-v  Outputs compression/decompression statistics of the respective program.
-i  The argument following is the file to take input from, default is stdin.
-o  The argument following is the file to output to, default is stdout.
```
