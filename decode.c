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
        "	A Huffman decoder.\n"
        "	Decompress a file using the Huffman coding algorithm.\n"
        "	\n"
        "USAGE\n"
        "	%s [-h] [-i infile] [-o outfile]"
        "\n"
        "OPTIONS\n"
        "	-h		Program usage and help.\n"
        "	-v		Print compression statistics.\n"
        "	-i		Input file to decompress.\n"
        "	-o		Output of decompressed data.\n",
        exec);
}

int main(int argc, char **argv) {
    static uint8_t buf[BLOCK] = { 0 };
    bool stats = false;
    int in = STDIN_FILENO;
    int out = STDOUT_FILENO;
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in = open(optarg, O_RDONLY); break;
        case 'o': out = open(optarg, O_CREAT | O_WRONLY | O_TRUNC); break;
        case 'v': stats = true; break;
        case 'h': {
            help(argv[0]);
            return EXIT_FAILURE;
        }
        }
    }

    // READ HEADER
    Header *header;
    header = (Header *) malloc(sizeof(Header));
    read(in, header, sizeof(Header));

    // VERIFY MAGIC
    if (header->magic != MAGIC) {
        fprintf(stderr, "Incorrect magic number.\n");
        return EXIT_FAILURE;
    }

    // MATCH PERMISSIONS chmod()
    fchmod(out, header->permissions);
    // REBUILD TREE
    read_bytes(in, buf, header->tree_size);
    Node *tree;
    tree = rebuild_tree(header->tree_size, buf);

    // REFILL TREE
    uint8_t bit = 0;
    Node *child = tree;
    // TRAVERSE TREE WITH CODES
    while (read_bit(in, &bit)) {
        if (child != NULL) {
            if (child->left != NULL && child->right != NULL) {
                bit ? (child = child->right) : (child = child->left);
            }
            //  IF LEAF, WRITE SYMBOL
            if (!child->left && !child->right) {
                if (bytes_written != header->file_size) {
                    write_bytes(out, &(child->symbol), 1);
                    child = tree;
                }
            }
        }
    }

    if (stats == true) {
        struct stat finfo;
        fstat(in, &finfo);
        double out_size = finfo.st_size;
        fstat(out, &finfo);
        double in_size = finfo.st_size;
        double difference = (in_size - out_size) / in_size;
        fprintf(stderr,
            "Uncompressed file size: %u bytes\n"
            "Compressed file size: %u bytes\n"
            "Space saving: %.2f%c\n",
            (uint32_t) out_size, (uint32_t) in_size, difference * 100, '%');
    }

    delete_tree(&tree);
    free(header);
    close(in);
    close(out);

    return 0;
}
