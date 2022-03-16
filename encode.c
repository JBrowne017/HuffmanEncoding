#include "pq.h"
#include "node.h"
#include "stack.h"
#include "io.h"
#include "defines.h"
#include "huffman.h"
#include "header.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define OPTIONS "hvi:o:"

void help(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "	A Huffman encoder.\n"
        "	Compress a file using the Huffman coding algorithm.\n"
        "	\n"
        "USAGE\n"
        "	%s [-h] [-i infile] [-o outfile]"
        "\n"
        "OPTIONS\n"
        "	-h		Program usage and help.\n"
        "	-v		Print compression statistics.\n"
        "	-i		Input file to compress.\n"
        "	-o		Output of compressed data.\n",
        exec);
}

int main(int argc, char **argv) {
    static uint8_t buf[BLOCK] = { 0 };
    static uint64_t histogram[ALPHABET] = { 0 };
    int in = STDIN_FILENO;
    bool stats = false;
    int out = STDOUT_FILENO;
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in = open(optarg, O_RDONLY, 0); break;
        case 'o': out = open(optarg, O_CREAT | O_WRONLY | O_TRUNC, 0); break;
        case 'v': stats = true; break;
        case 'h': {
            help(argv[0]);
            return EXIT_FAILURE;
        }
        }
    }

    // SET HEADER
    Header *header;
    header = (Header *) malloc(sizeof(Header));
    header->magic = MAGIC; // Set magic number
    struct stat finfo;
    fstat(in, &finfo); // declare fstat struct
    header->permissions = finfo.st_mode; // Get file permissions using fstat
    header->file_size = finfo.st_size; // Get fie size using fstat
    fchmod(out, header->permissions); // set permissions of the outfile

    // BUFFER TO AVOID USING LSEEK
//    static uint8_t *file_buf;
//    file_buf = malloc(header->file_size * sizeof(uint8_t));
//    uint32_t file_buf_index = 0;

    // READ and HISTOGRAM
    histogram[0] += 1;
    histogram[255] += 1;
    uint64_t some_bytes = 0;
    while ((some_bytes = read_bytes(in, buf, BLOCK)) > 0) {
        for (uint64_t i = 0; i < some_bytes; i += 1) {
//            file_buf[file_buf_index] = buf[i];
//            file_buf_index += 1;
            histogram[buf[i]] += 1;
        }
    }

    // CREATE HUFFMAN TREE
    Node *huffman_tree;
    huffman_tree = build_tree(histogram);

    // GET TREE SIZE
    header->tree_size = 0;
    for (uint32_t i = 0; i < 256; i += 1) {
        if (histogram[i] > 0) {
            header->tree_size += 1;
        }
    }
    header->tree_size *= 3;
    header->tree_size -= 1;

    // WRITE HEADER
    write_bytes(out, (uint8_t *) header, sizeof(Header));
    free(header);

    // DUMP HUFFMAN TREE
    dump_tree(out, huffman_tree);

    // EMIT CODES
    Code table[ALPHABET];
    build_codes(huffman_tree, table);
    delete_tree(&huffman_tree);

    // CHECK IF STDIN/OUT LSEEK
    //    if (out != STDIN_FILENO) {
    //        lseek(in, 0, SEEK_SET); // SET PTR TO B.O.F
    //    }

    bytes_read = 0; // RESET BYTES READ TO ZERO

    // READ AND WRITE CODES ACCORDINGLY ( LSEEK VERSION )
        lseek(in, 0, SEEK_SET);
        while ((some_bytes = read_bytes(in, buf, BLOCK)) > 0) {
            for (uint32_t i = 0; i < some_bytes; i += 1) {
                write_code(out, &(table[buf[i]]));
            }
        }
        flush_codes(out);

    // READ AND WRITE CODES ACCORDINGLY ( BUFFER VERSION )
/*    for (uint32_t i = 0; i < file_buf_index; i += 1) {
        write_code(out, &(table[file_buf[i]]));
    }
    flush_codes(out);
*/
    if (stats == true) {
        double in_size = finfo.st_size;
        fstat(out, &finfo);
        double out_size = finfo.st_size;
        double difference = (in_size - out_size) / in_size;
        fprintf(stderr,
            "Uncompressed file size: %u bytes\n"
            "Compressed file size: %u bytes\n"
            "Space saving: %.2f%c\n",
            (uint32_t) in_size, (uint32_t) out_size, difference * 100, '%');
    }

//    free(file_buf);
    close(in);
    close(out);

    return 0;
}
