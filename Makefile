all: sistem

sistem: main.c trie.c LinkedList.c lcs.c
	gcc -Wall main.c trie.c LinkedList.c lcs.c -o sistem.exe

clean: 
	rm -f sistem.exe
	rm -rf *o sistem
