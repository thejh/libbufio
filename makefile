CFLAGS := -Wall -Werror -Wno-strict-aliasing -std=c99 -fPIC -g -O2

.PHONY: clean install uninstall

libbufio.so: bufchain.o netconn.o
	$(CC) -shared -o libbufio.so bufchain.o netconn.o

bufchain.o: bufchain.c bufio.h
netconn.o: netconn.c bufio.h

clean:
	rm -f *.o *.so

install:
	install libbufio.so /usr/local/lib/
	install bufio.h /usr/local/include/

uninstall:
	rm /usr/local/lib/libbufio.so
	rm /usr/local/include/bufio.h