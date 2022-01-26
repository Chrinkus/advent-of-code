# Compiler vars
CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -O0

# Project locations
BINDIR = $${PWD}/build

all: \
	$(BINDIR)/1803	\
	$(BINDIR)/1802	\
	$(BINDIR)/1801

$(BINDIR)/1803: 1803.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(BINDIR)/1802: 1802.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

$(BINDIR)/1801: 1801.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	-rm $(BINDIR)/1803
	-rm $(BINDIR)/1802
	-rm $(BINDIR)/1801

.PHONY: all clean

