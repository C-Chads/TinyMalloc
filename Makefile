CFLAGS= -O3

all:
	$(CC) $(CFLAGS) -DUSE_ALIGNED *.c

clean:
	rm -f *.exe *.out *.o
