#ifndef PUBLIC_NL_H
#define PUBLIC_NL_H
#include <stdio.h>

typedef struct King_tag King;

King * NL_create_king(void);
void NL_compile(King *king, FILE *fp);

#endif /* PUBLIC_NL_H */
