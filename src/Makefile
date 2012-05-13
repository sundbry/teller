CC=gcc
CFLAGS=-Wall -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags pocketsphinx sphinxbase`
LDFLAGS=`pkg-config --libs pocketsphinx sphinxbase`


all: teller

teller: teller.o teller-listen.o
	$(CC) $(LDFLAGS) -o $@ $+

hello_ps: hello_ps.c
	$(CC) $(CFLAGS) -o $@ $+ 

%.o: %.c
	$(CC) $(CFLAGS) -c $+
	
.PHONY: clean

clean:
	rm -f hello_ps teller *.o
