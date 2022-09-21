#ifndef PUBLIC_MEMORY_H
#define PUBLIC_MEMORY_H

#include <stdio.h>
#include <stdlib.h>

typedef enum {
    MEM_FAIL_AND_EXIT,
    MEM_FILE_AND_RETURN
} MEM_FailMode;

typedef struct MEM_Controller_tag *MEM_Controller;
typedef void (*MEM_ErrorHandler)(MEM_Controller, char *, int, char *);
typedef struct MEM_Storage_tag *MEM_Storage;

void *MEM_malloc_func(MEM_Controller controller, char *filename, int line, size_t size);
MEM_Storage MEM_open_storage_func(MEM_Controller controller, char *filename, int line, int page_size);

#endif /* PUBLIC_MEMORY_H */
