#include "memory.h"

#define DEFAULT_PAGE_SIZE (1024) /* cell num */

MEM_Storage
MEM_open_storage_func(MEM_Controller controller, char *filename, int line, int page_size) {
    MEM_Storage storage;
    storage = MEM_malloc_func(controller, filename, line, sizeof(struct MEM_Storage_tag));
    storage->page_list = NULL;
    /* assert(page_size >= 0) */
    if (page_size > 0) {
        storage->current_page_size = page_size;
    } else {
        storage->current_page_size = DEFAULT_PAGE_SIZE;
    }

    return storage;
}