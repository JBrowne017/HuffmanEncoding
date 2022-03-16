# Assignment 5 - Huffman Compression
    The following program is a C implementation of Huffman Coding, which is a form of static encoding. Huffman Coding uses something called entropy, which is a measure of the amount of information within something, in our case symbols, or characters, in a file. Static encoding is going to assign the least number of bits to the most frequently occurring symbol. Therefore we may use the ideas discussed in the sections below to implement a way to parse a file, get a histogram of its characters, and assign the amount of bits to each according to their frequency.\
This is achieved through using the `encode` executable to encode a provided file, and `decode` to decode it. This program uses a myriad of data-structures such as a min-heap priority queue, stacks, and bit-vectors to achieve the implementation. The I/O functionality of this program uses the standard C `open() close() read() write()` functions. The program also utilizes buffer sizes of 4KB per read and write call.

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