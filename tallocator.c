#include "tallocator.h"

#ifdef USE_ALIGNED
#include <stdalign.h>
#define tallocator_alignment alignas(BLOCK_SIZE)
#else
#define tallocator_alignment /*a comment.*/
#endif


/*Our current managed buffer. You can just delete this and make it a fixed address if you want.*/
#define MANAGED_BUFFER_PTR tallocator_managed_buffer
tallocator_alignment unsigned char tallocator_managed_buffer[MANAGED_SIZE];
unsigned char tallocator_metadata[ (MANAGED_SIZE / BLOCK_SIZE) / 4] = {0};
/*Alter these for thread safety. Re-entrancy safety also guaranteed... but it may still be possible to experience deadlock.*/
#ifndef TALLOCATOR_MUTEX_LOCK
#define TALLOCATOR_MUTEX_LOCK /*a comment.*/
#define TALLOCATOR_MUTEX_UNLOCK /*a comment.*/
#endif

static void tiny_mark_blocks(size_t start, size_t nblocks){
	tallocator_metadata[start>>2] |= (3<<((start&3)*2));
	nblocks--;start++;
	for(;nblocks > 0; nblocks--, start++){
		tallocator_metadata[start>>2] &= ~(unsigned char)(3<<((start&3)*2));
		tallocator_metadata[start>>2] |= (unsigned char)(1<<((start&3)*2));
	}
}
static void tiny_unmark_blocks(size_t i){
	char have_started = 0;
	for(;;){
		if(have_started &&
			(((tallocator_metadata[i/4] & (3<<((i&3)*2)))>>((i&3)*2)) != 1) 
		) break;
		else if(!have_started && ((tallocator_metadata[i/4] & (3<<((i&3)*2)))>>((i&3)*2)) != 3) break;
		tallocator_metadata[i/4] &= ~(3<<((i&3)*2)); have_started = 1;
		i++;
	}
}

void* tiny_malloc(size_t nbytes){
	size_t current_len = 0;
	size_t i;
	if(nbytes > MANAGED_SIZE) return NULL;
	if(nbytes == 0) return NULL;
	nbytes += (BLOCK_SIZE - 1);
	nbytes /= (BLOCK_SIZE);
	if(nbytes == 0) return NULL;
	TALLOCATOR_MUTEX_LOCK
	for(i = 0; i < (MANAGED_SIZE / BLOCK_SIZE); i++){
		if((tallocator_metadata[i/4] & (3<<((i&3)*2))) == 0) current_len++;
		else /*Occupied!*/ current_len = 0;
		if(current_len == nbytes) { /*Success!*/
			size_t start = i - (current_len-1);
			tiny_mark_blocks(start, current_len);
			start *= BLOCK_SIZE;
			start += (size_t)MANAGED_BUFFER_PTR;
			TALLOCATOR_MUTEX_UNLOCK
			return (void*)start;
		}
	}
	TALLOCATOR_MUTEX_UNLOCK
	return NULL;
}
int tiny_free(void* ptr){
	size_t i = (size_t)ptr; if(i == 0) return 0;
	if (i < (size_t)MANAGED_BUFFER_PTR) return 0;
	i -= (size_t)MANAGED_BUFFER_PTR;
	if(i % BLOCK_SIZE) return 0; /*This was not a pointer we handed out.*/
	i /= BLOCK_SIZE;
	TALLOCATOR_MUTEX_LOCK
	if(  ((tallocator_metadata[i/4] & (3<<((i&3)*2)) )>>((i&3)*2)) == 3){
		tiny_unmark_blocks(i); TALLOCATOR_MUTEX_UNLOCK; return 1;
	}
	TALLOCATOR_MUTEX_UNLOCK
	return 0;
}
