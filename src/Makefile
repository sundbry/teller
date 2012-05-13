CC=gcc
CFLAGS=-Wall -DMODELDIR=\"`pkg-config --variable=modeldir pocketsphinx`\" `pkg-config --cflags --libs pocketsphinx sphinxbase`


all: teller

teller: teller.c
	$(CC) $(CFLAGS) -o $@ $+

hello_ps: hello_ps.c
	$(CC) $(CFLAGS) -o $@ $+ 
	
.PHONY: clean

clean:
	rm -f hello_ps teller *.o
