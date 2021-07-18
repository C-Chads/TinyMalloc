#include <stddef.h>

#ifdef TALLOCATOR_DEBUG
#include <stdio.h>
#endif

#define MANAGED_SIZE ((size_t)1024 * 1024 * 1024 * 1)
/*4 kilobytes*/
#define BLOCK_SIZE 4096


#ifdef USE_ALIGNED
#include <stdalign.h>
#define tallocator_alignment alignas(BLOCK_SIZE)
#else
#define tallocator_alignment /*a comment.*/
#endif



/*Our current managed buffer. You can just delete this and make it a fixed address if you want.*/

extern tallocator_alignment unsigned char tallocator_managed_buffer[MANAGED_SIZE];
#define MANAGED_BUFFER_PTR tallocator_managed_buffer





void* tiny_malloc(size_t nbytes);

int tiny_free(void* ptr);
