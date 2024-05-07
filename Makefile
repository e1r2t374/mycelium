CC = gcc
LDLIBS = -pthread -lpthread

DCFLAGS = -Wall -Wextra -Wpedantic
-std=c99 -Wformat=2 -Wformat-overflow -Wformat-truncation -O2 -Wconversion -D_FORTIFY_SOURCE=2 -fstack-clash-protection -fstack-protector-strong -fstack-protector-all -Wdouble-promotion -pipe -fcf-protection -fpie -pedantic-errors -m64 -g3

CFLAGS= -O2 -std=c99 -m64 -pipe -fcf-protection -fstack-protector-strong -fstack-protector-all -s -fpic -shared -fpie -Wl,-pie -Wl,-z,now -Wl,-z,relro -Wl,-z,defs 

.PHONY: default all clean
mycel: mycel.o
mycel.o: mycel.c
	$(CC) $(CFLAGS) $(LDLIBS) -c mycel.c -o mycel.o
	install -m 0777 mycel.o mycel
clean:
	-rm -rf *.o *.s mycel mycel_debug
debug: 
	$(CC) $(DCFLAGS) $(LDLIBS) mycel.c -o mycel_debug.o
	install -m 0777 mycel_debug.o mycel_debug 
