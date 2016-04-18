EMACS_ROOT ?= ../..
EMACS ?= emacs

CC      = gcc
LD      = gcc
CPPFLAGS = -I$(EMACS_ROOT)/src
CFLAGS = -std=gnu99 -ggdb3 -Wall -fPIC $(CPPFLAGS) $(shell pkg-config --cflags uuid)

.PHONY : test

all: uuid-core.so

uuid-core.so: uuid-core.o
	$(LD) -shared $(LDFLAGS) -o $@ $^ $(shell pkg-config --libs uuid)

uuid-core.o: uuid-core.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f uuid-core.so uuid-core.o

test:
	$(EMACS) -Q -batch -L . $(LOADPATH) \
		-l test/test.el \
		-f ert-run-tests-batch-and-exit
