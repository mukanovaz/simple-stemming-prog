all: sistem

sistem: main.o trie.o LinkedList.o lcs.o
	gcc main.o trie.o LinkedList.o lcs.o -o sistem

main: main.c
	gcc -c main.c

trie: trie.c
	gcc -c trie.c

linkedlist: LinkedList.c
	gcc -c LinkedList.c

lcs: lcs.c
	gcc -c lcs.c

clean: 
	rm -f sistem
	rm -rf *o sistem