#include "tallocator.h"
#include <stdio.h>

int main(){
	void* p;
	printf("\r\n%zx", (size_t)tiny_malloc(8901489));
	printf("\r\n%zx", (size_t)(p = tiny_malloc(8192)));
	printf("\r\n%zx", (size_t)tiny_malloc(90889918590)); /*A number far too large to possiblye ever be malloced.*/
	printf("\r\n%zx", (size_t)tiny_malloc(9585590));
	printf("\r\n%zx", (size_t)tiny_malloc(7289578));
	if(tiny_free(p) == 0)printf("\r\n<ERROR>\r\n");
	printf("\r\n%zx", (size_t)tiny_malloc(8192)); /*This one will produce the same number as the second allocation.*/
	printf("\r\n%zx", (size_t)tiny_malloc(8192)); /**/
}
