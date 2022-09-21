#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

static void
error_handler(MEM_Controller controller, char *filename, int line, char *msg) {
    if (controller->error_fp == NULL) {
        controller->error_fp = stderr;
    }
    controller->error_handler(controller, filename, line, msg);

    if (controller->fail_mode == MEM_FAIL_AND_EXIT) {
        exit(1);
    }
}

void *
MEM_malloc_func(MEM_Controller controller, char *filename, int line, size_t size) {
    void *ptr;
    size_t alloc_size;

    alloc_size = size;
    ptr = malloc(alloc_size);
    if (ptr == NULL) {
        error_handler(controller, filename, line, "malloc");
    }

    return ptr;
}