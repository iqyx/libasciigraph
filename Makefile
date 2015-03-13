all: test

test:
	$(CC) --std=c99 asciigraph.c test1.c -o asciigraph

clean:
	rm asciigraph
