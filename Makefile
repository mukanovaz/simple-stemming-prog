all: sistem

sistem: main.c trie.c LinkedList.c lcs.c
	gcc main.c trie.c LinkedList.c lcs.c -o sistem.exe

clean: 
	rm -f sistem.exe
	rm -rf *o sistem