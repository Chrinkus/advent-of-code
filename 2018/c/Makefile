# Compiler vars
CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -O3
CPPFLAGS = 
LDFLAGS = -Wl,-rpath=/usr/local/lib -lcgs

# Project locations
BINDIR = $${PWD}/build

all: \
	$(BINDIR)/1803	\
	$(BINDIR)/1802	\
	$(BINDIR)/1801

$(BINDIR)/1803: 1803.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/1802: 1802.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

$(BINDIR)/1801: 1801.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $< $(LDFLAGS)

clean:
	-rm $(BINDIR)/1803
	-rm $(BINDIR)/1802
	-rm $(BINDIR)/1801

.PHONY: all clean

