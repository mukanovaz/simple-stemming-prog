all: sistem

sistem: main.o trie.o
	gcc main.o trie.o -o sistem

main: main.c
	gcc -c main.c

trie: trie.c
	gcc -c trie.c

clean: 
	rm -f sistem
	rm -rf *o sistem