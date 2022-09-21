#ifndef PRIVATE_MEMORY_H
#define PRIVATE_MEMORY_H
#include "../_MEMORY.h"

typedef union {
    long l_dummy;
    double d_dummy;
    void *p_dummy;
} Align;

#define MARK_SIZE (4)

typedef union Header_tag Header;

typedef struct {
    int size;
    char *filename;
    int line;
    Header *prev;
    Header *next;
    unsigned char mark[MARK_SIZE];
} HeaderStruct;

#define ALIGN_SIZE (sizeof(Align))
#define revalue_up_align(val) ((val) ? (((val) - 1) / ALIGN_SIZE + 1) : 0)
#define HEADER_ALIGN_SIZE (revalue_up_align(sizeof(HeaderStruct)))
#define MARK (0xCD)

union Header_tag {
    HeaderStruct s;
    Align u[];
};

struct MEM_Controller_tag {
    FILE *error_fp;
    MEM_ErrorHandler error_handler;
    MEM_FailMode fail_mode;
    Header *block_header;
};

typedef union {
    long l_dummy;
    double d_dummy;
    void *p_dummy;
} Cell;

typedef struct MemoryPage_tag {
    int cell_num;
    int use_cell_num;
    MemoryPage_tag *next;
    Cell cell[1];
} MemoryPage;

typedef MemoryPage *MemoryPageList;

struct MEM_Storage_tag {
    MemoryPageList page_list;
    int current_page_size;
};

#endif /* PRIVATE_MEMORY_H */
