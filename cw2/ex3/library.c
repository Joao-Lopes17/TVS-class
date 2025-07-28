#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PG_SIZE 4 * 1024

void *alloc_text(size_t npages) {
    return mmap(NULL, npages * PG_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void *alloc_data(size_t npages) {
    return mmap(NULL, npages * PG_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void free(void *ptr) {
    munmap(ptr, PG_SIZE);
}
