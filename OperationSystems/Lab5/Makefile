CC = clang
CFLAGS = -Wall -g

all:
	LD_LIBRARY_PATH=.
	$(CC) $(CFLAGS) program1.c libmath1.c -o program1
	$(CC) $(CFLAGS) program2.c -L. -lmath1 -lmath2 -o program2

libmath1.o: libmath1.c math1.h
	$(CC) $(CFLAGS) -c libmath1.c

libmath2.o: libmath2.c math2.h
	$(CC) $(CFLAGS) -c libmath2.c

libmath1.so: libmath1.c math1.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libmath1.c -lc

libmath2.so: libmath2.c math2.h
	$(CC) $(CFLAGS) -fPIC -shared -o $@ libmath2.c -lc
