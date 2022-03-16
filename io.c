#include "code.h"
#include "io.h"
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>

uint64_t bytes_read = 0;
uint64_t bytes_written = 0;
static uint32_t local_bytes_read = 0;
static uint32_t local_bytes_written = 0;

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    local_bytes_read = 0;
    while (
        (local_bytes_read += read(infile, buf + local_bytes_read, nbytes - local_bytes_read)) > 0) {
        if ((bytes_read += local_bytes_read) == (uint64_t) nbytes) {
            return bytes_read;
        }
        return local_bytes_read;
    }
    return 0;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    local_bytes_written = 0;
    while ((local_bytes_written
               += write(outfile, buf + local_bytes_written, nbytes - local_bytes_written))
           > 0) {
        if ((bytes_written += local_bytes_written) == (uint64_t) nbytes) {
            return bytes_written;
        }
        return local_bytes_written;
    }
    return 0;
}

bool read_bit(int infile, uint8_t *bit) {
    static uint8_t read_bit_buffer[BLOCK] = { 0 };
    static uint32_t read_bit_top = 0;
    static uint32_t read_bit_buffer_size = 0;

    if (read_bit_top == 0) {
        if ((read_bit_buffer_size = read_bytes(infile, read_bit_buffer, BLOCK)) < 1) {
            return false;
        }
    }
    *bit = (read_bit_buffer[read_bit_top / 8] >> read_bit_top % 8) & 0x1;
    read_bit_top += 1;

    if (read_bit_top == read_bit_buffer_size * 8) {
        read_bit_top = 0;
    }

    return true;
}

// Write_bits and flush_codes share a buffer
static uint8_t write_bit_buffer[BLOCK] = { 0 };
static uint32_t write_bit_index = 0;
void write_code(int outfile, Code *c) {
    // Each bit in the Code will be buffered into the bufer
    // Buffered starting from zero
    // When BUFFER is FULL
    //      WRITE BUFER to OUTFILE
    // Buffer each bit into the buffer
    //
    // BUFFER a CODE
    // BUFFER IS WRITTEN when FULL
    //
    // Buffer Each Bit Into BUFFER
    uint32_t bit;
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        bit = code_get_bit(c, i);
        if (bit == 1) {
            // set bit at index
            write_bit_buffer[write_bit_index / 8] |= (0x1 << write_bit_index % 8);
            write_bit_index += 1;
        } else {
            // clear bit at index
            write_bit_buffer[write_bit_index / 8] &= ~(0x1 << write_bit_index % 8);
            write_bit_index += 1;
        }
        if (write_bit_index == BLOCK) {
            flush_codes(outfile);
        }
    }
    return;
}

void flush_codes(int outfile) {
    // flush bits out of buffer if not empty
    if (write_bit_index > 0) {
        uint32_t bytes = 1 + ((write_bit_index - 1) / 8);
        write_bytes(outfile, write_bit_buffer, bytes);
    }
    for (uint64_t i = 0; i < BLOCK; i += 1) {
        write_bit_buffer[i] = 0;
    }
    write_bit_index = 0;
    return;
}
