CC=gcc
CFLAGS=-Wall -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" -DBASEDIR=\"`pwd`/..\" `pkg-config --cflags pocketsphinx sphinxbase`
LDFLAGS=`pkg-config --libs pocketsphinx sphinxbase`


all: teller

teller: teller-main.o teller.o teller-listen.o teller-hyp.o teller-action.o
	$(CC) $(LDFLAGS) -o $@ $+

teller-test: teller-test.o teller.o teller-listen.o teller-hyp.o teller-action.o
	$(CC) $(LDFLAGS) -o $@ $+

hello_ps: hello_ps.c
	$(CC) $(CFLAGS) -o $@ $+ 

%.o: %.c
	$(CC) $(CFLAGS) -c $+
	
.PHONY: test clean

test: teller-test
	./teller-test

clean:
	rm -f hello_ps teller *.o *.log
