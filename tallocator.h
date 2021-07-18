#include <stddef.h>
/*4 kilobytes block size*/
#define BLOCK_SIZE 0x1000
/*The managed buffer's size. For speed, this should be a compiletime constant.*/
#define MANAGED_SIZE ((size_t)BLOCK_SIZE * 1024 * 1024)
void* tiny_malloc(size_t nbytes);
int tiny_free(void* ptr);
